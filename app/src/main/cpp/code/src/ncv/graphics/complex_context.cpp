/*
 * Copyright 2020 Konstantinos Tzevanidis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <graphics/complex_context.hpp>
#include <graphics/pipeline.hpp>

#include <graphics/resources/image.hpp>
#include <graphics/resources/buffer.hpp>

#include <graphics/data/index.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/data/texture.hpp>

#include "game_activity/native_app_glue/android_native_app_glue.h"

#include "engine/vulkan_wrapper/android/AndroidVulkanWrapper.h"
#include "engine/VulkanPhysicalDeviceCandidate.h"
#include "engine/VulkanPhysicalDeviceProvider.h"

#include "engine/common/StringListSelector.h"
#include "engine/common/StringUtil.h"


using namespace ::std;
using namespace ::vk;
using namespace ::utilities;
using namespace ::metadata;

namespace graphics {

    std::vector<std::string> instanceExtensions = {
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,

            // old version
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
            // new version
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,

            VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME,
            VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME,
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
    };

    std::vector<std::string> layers = {
            "VK_LAYER_KHRONOS_validation"
    };

    std::vector<std::string> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_MAINTENANCE1_EXTENSION_NAME,
            VK_KHR_BIND_MEMORY_2_EXTENSION_NAME,
            VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
            VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME,
            VK_EXT_QUEUE_FAMILY_FOREIGN_EXTENSION_NAME,
            VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME,
            VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME,
            VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME,
            VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
            VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME
    };

    complex_context::complex_context(const std::string &a_app_name, const android_app &app)
            : vulkan_context{}, m_app(app) {
        using ::vk_util::message_callback;


        mInstance = std::make_unique<engine::VulkanInstance>(
                a_app_name, 1, "AndroidVulkanEngine", 1,
                common::RequiredAndOptionalStringListSelector({}, instanceExtensions),
                common::RequiredAndOptionalStringListSelector({}, layers)
        );

        engine::initAndroidVulkanWrapper(mInstance->getInstance());

        m_devices = mInstance->getInstance().enumeratePhysicalDevices();
    }

    complex_context::~complex_context() {
        mDevice->getDevice().waitIdle();

        if (is_initialized) {
            for (auto &sampler: m_samplers)
                mDevice->getDevice().destroySampler(sampler);
            m_samplers.clear();
            mDevice->getDevice().destroyDescriptorPool(m_desc_pool.release());
            m_graphics_pipeline.reset();
            m_camera_image.reset();
            m_vertex_data.reset();
            m_index_data.reset();
            release_rendering_resources();
            mDevice->getDevice().destroyRenderPass(m_render_pass);
            for (auto &semaphore: m_proc_semaphores)
                mDevice->getDevice().destroySemaphore(semaphore);
            m_proc_semaphores.clear();
            for (auto &semaphore: m_pres_semaphores)
                mDevice->getDevice().destroySemaphore(semaphore);
            m_pres_semaphores.clear();
            for (auto &fence: m_cmd_fences)
                mDevice->getDevice().destroyFence(fence);
            m_cmd_fences.clear();
            mDevice->getDevice().freeCommandBuffers(m_cmd_pool.get(),
                                                    m_cmd_buffers);
            m_cmd_buffers.clear();
            mDevice->getDevice().destroyCommandPool(m_cmd_pool.release());
        }
    }

    void complex_context::reset_surface(ANativeWindow *a_window) {
        // Create a presentation surface or reset existing one

        AndroidSurfaceCreateInfoKHR surf_info;

        surf_info.flags = static_cast<AndroidSurfaceCreateFlagsKHR>(0);
        surf_info.window = a_window;

        mSurface.reset(new engine::AndroidVulkanSurface(mInstance->getInstance(), a_window));
    }

    void complex_context::select_device_and_qfamily() {
        pair<uint32_t, uint32_t> result;

        // Select an appropriate device based on queue family support and presentation capabilities

        int32_t selected_device_index{-1}, selected_queue_family_index{-1};

        for (uint32_t i = 0; i < m_devices.size(); ++i) {
            // Select the first device with a queue family that supports graphics

            auto queue_props = m_devices[i].getQueueFamilyProperties();

            for (size_t j = 0; j < queue_props.size(); ++j) {
                auto &props = queue_props[j];
                bool presentation_flag = m_devices[i].getSurfaceSupportKHR(j, mSurface->getSurface());
                if ((props.queueFlags & QueueFlagBits::eGraphics) && presentation_flag &&
                    selected_queue_family_index < 0)
                    selected_queue_family_index = j;
            }

            if (selected_queue_family_index > -1 && selected_device_index < 0) {
                selected_device_index = i;
                break;
            }
        }

        if (selected_device_index == -1)
            throw runtime_error{"Initialization failed, no Appropriate Device Found."};

        m_gpu = m_devices[selected_device_index];
        m_qfam_index = selected_queue_family_index;
    }

    void complex_context::create_logical_device() {
        // Create logical device
//
//        std::vector<const char *> enabledDeviceExtensionNames = common::StringUtil::toStringPtrArray(deviceExtensions);
//
//        vector<DeviceQueueCreateInfo> queue_infos = {DeviceQueueCreateInfo()};
//        vector<float> queue_priorities = {1.0f};
//
//        queue_infos[0].queueFamilyIndex = m_qfam_index;
//        queue_infos[0].queueCount = 1;
//        queue_infos[0].pQueuePriorities = queue_priorities.data();
//        queue_infos[0].flags = static_cast<DeviceQueueCreateFlags>(0);
//
//        PhysicalDeviceFeatures2 dev_features;
//        PhysicalDeviceSamplerYcbcrConversionFeatures ycbcr_features;
//
//        ycbcr_features.samplerYcbcrConversion = true;
//
//        dev_features.pNext = &ycbcr_features;
//        dev_features.features.samplerAnisotropy = true;
//
//        DeviceCreateInfo dev_info;
//
//        dev_info.pNext = &dev_features;
//        dev_info.flags = static_cast<DeviceCreateFlags>(0);
//        dev_info.queueCreateInfoCount = 1;
//        dev_info.pQueueCreateInfos = queue_infos.data();
//        dev_info.enabledLayerCount = 0;
//        dev_info.ppEnabledLayerNames = nullptr;
//        dev_info.enabledExtensionCount = enabledDeviceExtensionNames.size();
//        dev_info.ppEnabledExtensionNames = enabledDeviceExtensionNames.data();


        std::unique_ptr<engine::VulkanPhysicalDeviceProvider> vulkanPhysicalDeviceProvider = std::make_unique<engine::DefaultVulkanPhysicalDeviceProvider>(
                *mInstance,
                *mSurface,
                deviceExtensions
        );
        std::unique_ptr<engine::VulkanPhysicalDeviceCandidate> candidate = std::move(vulkanPhysicalDeviceProvider->provide());
        std::unique_ptr<engine::VulkanPhysicalDevice> &vulkanPhysicalDevice = candidate->getPhysicalDevice();

        mDevice = std::make_unique<engine::VulkanDevice>(
                *vulkanPhysicalDevice,
                candidate->getSurfaceSupport().value(),
                deviceExtensions,
                mInstance->getEnabledLayers(),
                2
        );

        // Acquire Presentation Queue

        m_pres_queue = mDevice->getDevice().getQueue(m_qfam_index, 0);
    }

    void complex_context::create_render_pass() {
        // Create render pass

        auto img_format = Format::eR8G8B8A8Unorm;

        AttachmentDescription color_attachment;

        color_attachment.flags = AttachmentDescriptionFlags(0);
        color_attachment.format = img_format;
        color_attachment.samples = SampleCountFlagBits::e1;
        color_attachment.loadOp = AttachmentLoadOp::eClear;
        color_attachment.storeOp = AttachmentStoreOp::eStore;
        color_attachment.stencilLoadOp = AttachmentLoadOp::eDontCare;
        color_attachment.stencilStoreOp = AttachmentStoreOp::eDontCare;
        color_attachment.initialLayout = ImageLayout::eUndefined;
        color_attachment.finalLayout = ImageLayout::ePresentSrcKHR;

        AttachmentReference color_attachment_ref;

        color_attachment_ref.attachment = 0;
        color_attachment_ref.layout = ImageLayout::eColorAttachmentOptimal;

        AttachmentDescription depth_attachment;

        depth_attachment.format = Format::eD32Sfloat;
        depth_attachment.samples = SampleCountFlagBits::e1;
        depth_attachment.loadOp = AttachmentLoadOp::eClear;
        depth_attachment.storeOp = AttachmentStoreOp::eDontCare;
        depth_attachment.stencilLoadOp = AttachmentLoadOp::eDontCare;
        depth_attachment.stencilStoreOp = AttachmentStoreOp::eDontCare;
        depth_attachment.initialLayout = ImageLayout::eUndefined;
        depth_attachment.finalLayout = ImageLayout::eDepthStencilAttachmentOptimal;

        AttachmentReference depth_attachment_ref;

        depth_attachment_ref.attachment = 1;
        depth_attachment_ref.layout = ImageLayout::eDepthStencilAttachmentOptimal;

        SubpassDescription subpass_description;

        subpass_description.flags = SubpassDescriptionFlags(0);
        subpass_description.pipelineBindPoint = PipelineBindPoint::eGraphics;
        subpass_description.inputAttachmentCount = 0;
        subpass_description.pInputAttachments = nullptr;
        subpass_description.colorAttachmentCount = 1;
        subpass_description.pColorAttachments = &color_attachment_ref;
        subpass_description.pResolveAttachments = nullptr;
        subpass_description.pDepthStencilAttachment = &depth_attachment_ref;
        subpass_description.preserveAttachmentCount = 0;
        subpass_description.pPreserveAttachments = nullptr;

        // Create subpass dependencies to define implicit start and end pipeline barriers

        vector<SubpassDependency> subpass_deps;

        SubpassDependency starting_pass, ending_pass;

        starting_pass.srcSubpass = VK_SUBPASS_EXTERNAL;
        starting_pass.dstSubpass = 0;
        starting_pass.srcStageMask = PipelineStageFlagBits::eBottomOfPipe;
        starting_pass.dstStageMask = PipelineStageFlagBits::eColorAttachmentOutput |
                                     PipelineStageFlagBits::eEarlyFragmentTests;
        starting_pass.srcAccessMask = AccessFlagBits::eMemoryRead;
        starting_pass.dstAccessMask = AccessFlagBits::eColorAttachmentWrite |
                                      AccessFlagBits::eDepthStencilAttachmentWrite;
        starting_pass.dependencyFlags = DependencyFlagBits::eByRegion;

        ending_pass.srcSubpass = 0;
        ending_pass.dstSubpass = VK_SUBPASS_EXTERNAL;
        ending_pass.srcStageMask = PipelineStageFlagBits::eColorAttachmentOutput |
                                   PipelineStageFlagBits::eEarlyFragmentTests;
        ending_pass.dstStageMask = PipelineStageFlagBits::eBottomOfPipe;
        ending_pass.srcAccessMask = AccessFlagBits::eColorAttachmentWrite |
                                    AccessFlagBits::eDepthStencilAttachmentWrite;
        ending_pass.dstAccessMask = AccessFlagBits::eMemoryRead;
        ending_pass.dependencyFlags = DependencyFlagBits::eByRegion;

        subpass_deps.push_back(starting_pass);
        subpass_deps.push_back(ending_pass);

        array<AttachmentDescription, 2> attachments = {
                color_attachment,
                depth_attachment
        };

        RenderPassCreateInfo render_pass_info;

        render_pass_info.flags = RenderPassCreateFlags(0);
        render_pass_info.attachmentCount = attachments.size();
        render_pass_info.pAttachments = attachments.data();
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass_description;
        render_pass_info.dependencyCount = subpass_deps.size();
        render_pass_info.pDependencies = subpass_deps.data();

        m_render_pass = mDevice->getDevice().createRenderPass(render_pass_info);
    }

    void complex_context::create_data_buffers(AHardwareBuffer *a_buffer) {
        m_index_data = make_shared<index_data>(m_gpu, mDevice->getDevice(), BufferUsageFlagBits::eIndexBuffer,
                                               SharingMode::eExclusive, data::index_set);
        m_vertex_data = make_shared<vertex_data>(m_gpu, mDevice->getDevice(), BufferUsageFlagBits::eVertexBuffer,
                                                 SharingMode::eExclusive, data::vertex_set);
        m_camera_image = make_shared<camera_data>(m_gpu, mDevice->getDevice(), a_buffer);
    }

    void complex_context::create_graphics_pipeline() {
        auto surf_caps = m_gpu.getSurfaceCapabilitiesKHR(mSurface->getSurface());

        // Create graphics pipeline

        auto glpipe_params = pipeline::parameters{
                m_app.activity->assetManager,
                mDevice->getDevice(),
                surf_caps.currentExtent,
                m_render_pass,
                m_camera_image != nullptr ? m_camera_image->get_sampler() : nullptr
        };

        auto glpipe_shader_info = pipeline::shaders_info{
                "simple.vert",
                "camera-mapping.frag"
        };

        m_graphics_pipeline.reset(new pipeline(glpipe_params, glpipe_shader_info));
    }

    void complex_context::reset_swapchain() {
        // Get surface capabilities and create a swapchain or reset existing one

        auto surf_caps = m_gpu.getSurfaceCapabilitiesKHR(mSurface->getSurface());
        auto surf_fmts = m_gpu.getSurfaceFormatsKHR(mSurface->getSurface());
        auto pres_modes = m_gpu.getSurfacePresentModesKHR(mSurface->getSurface());

        SwapchainCreateInfoKHR swap_chain_info;

        m_surface_extent = surf_caps.currentExtent;

        swap_chain_info.flags = static_cast<SwapchainCreateFlagsKHR>(0);
        swap_chain_info.surface = mSurface->getSurface();
        swap_chain_info.minImageCount = 3;
        swap_chain_info.imageFormat = Format::eR8G8B8A8Unorm;
        swap_chain_info.imageColorSpace = ColorSpaceKHR::eSrgbNonlinear;
        swap_chain_info.imageExtent = m_surface_extent;
        swap_chain_info.imageArrayLayers = 1;
        swap_chain_info.imageUsage = ImageUsageFlagBits::eColorAttachment | ImageUsageFlagBits::eTransferDst;
        swap_chain_info.imageSharingMode = SharingMode::eExclusive;
        swap_chain_info.queueFamilyIndexCount = 0;
        swap_chain_info.pQueueFamilyIndices = nullptr;
        swap_chain_info.preTransform = surf_caps.currentTransform;
        swap_chain_info.compositeAlpha = CompositeAlphaFlagBitsKHR::eInherit;
        swap_chain_info.presentMode = PresentModeKHR::eMailbox;
        swap_chain_info.clipped = true;
        swap_chain_info.oldSwapchain = nullptr;

        m_swap_chain.reset(mDevice->getDevice().createSwapchainKHR(swap_chain_info));
    }

    void complex_context::reset_framebuffer_and_zbuffer() {
        auto img_format = Format::eR8G8B8A8Unorm;

        m_images = mDevice->getDevice().getSwapchainImagesKHR(m_swap_chain.get());

        // Create swapchain image views

        for (auto &image: m_images) {
            ImageViewCreateInfo img_view_info;

            img_view_info.image = image;
            img_view_info.viewType = ImageViewType::e2D;
            img_view_info.format = img_format;
            img_view_info.components.r = ComponentSwizzle::eIdentity;
            img_view_info.components.g = ComponentSwizzle::eIdentity;
            img_view_info.components.b = ComponentSwizzle::eIdentity;
            img_view_info.components.a = ComponentSwizzle::eIdentity;
            img_view_info.subresourceRange.aspectMask = ImageAspectFlagBits::eColor;
            img_view_info.subresourceRange.baseMipLevel = 0;
            img_view_info.subresourceRange.levelCount = 1;
            img_view_info.subresourceRange.baseArrayLayer = 0;
            img_view_info.subresourceRange.layerCount = 1;

            m_swapchain_img_views.push_back(mDevice->getDevice().createImageView(img_view_info));
        }

        m_depth_buffer = make_shared<depth_data>(m_gpu, mDevice->getDevice(), ImageUsageFlagBits::eDepthStencilAttachment,
                                                 SharingMode::eExclusive, m_surface_extent);

        // Create framebuffers
        for (auto &image_view: m_swapchain_img_views) {
            array<ImageView, 2> attachments = {
                    image_view,
                    m_depth_buffer->get_img_view()
            };

            FramebufferCreateInfo framebuffer_info;
            framebuffer_info.flags = FramebufferCreateFlags(0);
            framebuffer_info.renderPass = m_render_pass;
            framebuffer_info.attachmentCount = attachments.size();
            framebuffer_info.pAttachments = attachments.data();
            framebuffer_info.width = m_surface_extent.width;
            framebuffer_info.height = m_surface_extent.height;
            framebuffer_info.layers = 1;

            m_framebuffers.push_back(mDevice->getDevice().createFramebuffer(framebuffer_info));
        }
    }

    void complex_context::reset_sync_and_cmd_resources() {
        // Release previously allocated pool

        if (is_initialized) {
            for (auto &semaphore: m_proc_semaphores)
                mDevice->getDevice().destroySemaphore(semaphore);
            m_proc_semaphores.clear();
            m_proc_si = 0;
            for (auto &semaphore: m_pres_semaphores)
                mDevice->getDevice().destroySemaphore(semaphore);
            m_pres_semaphores.clear();
            for (auto &fence: m_cmd_fences)
                mDevice->getDevice().destroyFence(fence);
            m_cmd_fences.clear();
            mDevice->getDevice().freeCommandBuffers(m_cmd_pool.get(), m_cmd_buffers);
            m_cmd_buffers.clear();
            mDevice->getDevice().destroyCommandPool(m_cmd_pool.release());
        }

        // Create command pool

        CommandPoolCreateInfo pool_info;

        pool_info.flags = CommandPoolCreateFlagBits::eResetCommandBuffer | CommandPoolCreateFlagBits::eTransient;
        pool_info.queueFamilyIndex = m_qfam_index;

        m_cmd_pool.reset(mDevice->getDevice().createCommandPool(pool_info));

        // Allocate command buffers one for each chain image

        CommandBufferAllocateInfo cmd_buf_info;

        cmd_buf_info.commandPool = m_cmd_pool.get();
        cmd_buf_info.level = CommandBufferLevel::ePrimary;
        cmd_buf_info.commandBufferCount = m_images.size();

        m_cmd_buffers = mDevice->getDevice().allocateCommandBuffers(cmd_buf_info);

        // Create command buffer fences and rendering semaphores

        SemaphoreCreateInfo semaphore_info;
        FenceCreateInfo cmd_fence_info;

        cmd_fence_info.flags = FenceCreateFlagBits::eSignaled;

        for (uint32_t i = 0; i < m_cmd_buffers.size(); ++i) {
            m_cmd_fences.push_back(mDevice->getDevice().createFence(cmd_fence_info));
            m_proc_semaphores.push_back(mDevice->getDevice().createSemaphore(semaphore_info));
            m_pres_semaphores.push_back(mDevice->getDevice().createSemaphore(semaphore_info));
        }
    }

    void complex_context::reset_ubos_samplers_and_descriptors() {
        // Release previously allocated uniform buffers

        if (is_initialized) {
            for (auto &sampler: m_samplers)
                mDevice->getDevice().destroySampler(sampler);
            m_samplers.clear();
            mDevice->getDevice().destroyDescriptorPool(m_desc_pool.release());
        }

        // Create or reset descriptor pool

        m_desc_configs = vector<descriptor_configuration>(m_cmd_buffers.size());

        array<DescriptorPoolSize, 2> pool_sizes;

        pool_sizes[0].type = DescriptorType::eUniformBuffer;
        pool_sizes[0].descriptorCount = m_cmd_buffers.size();
        pool_sizes[1].type = DescriptorType::eCombinedImageSampler;
        pool_sizes[1].descriptorCount = m_cmd_buffers.size() * m_desc_configs[0].writes.size();

        DescriptorPoolCreateInfo pool_info;

        pool_info.poolSizeCount = pool_sizes.size();
        pool_info.pPoolSizes = pool_sizes.data();
        pool_info.maxSets = m_cmd_buffers.size();

        m_desc_pool.reset(mDevice->getDevice().createDescriptorPool(pool_info));

        vector<DescriptorSetLayout> desc_layouts(m_cmd_buffers.size(), m_graphics_pipeline->get_desc_set());

        DescriptorSetAllocateInfo desc_set_alloc_info;

        desc_set_alloc_info.descriptorPool = m_desc_pool.get();
        desc_set_alloc_info.descriptorSetCount = m_cmd_buffers.size();
        desc_set_alloc_info.pSetLayouts = desc_layouts.data();

        m_desc_sets = mDevice->getDevice().allocateDescriptorSets(desc_set_alloc_info);

        SamplerCreateInfo sampler_info;

        sampler_info.magFilter = Filter::eLinear;
        sampler_info.minFilter = Filter::eLinear;
        sampler_info.addressModeU = SamplerAddressMode::eRepeat;
        sampler_info.addressModeV = SamplerAddressMode::eRepeat;
        sampler_info.addressModeW = SamplerAddressMode::eRepeat;
        sampler_info.anisotropyEnable = true;
        sampler_info.maxAnisotropy = 4.0f;
        sampler_info.borderColor = BorderColor::eIntOpaqueBlack;
        sampler_info.unnormalizedCoordinates = false;
        sampler_info.compareEnable = false;
        sampler_info.compareOp = CompareOp::eAlways;
        sampler_info.mipmapMode = SamplerMipmapMode::eLinear;
        sampler_info.mipLodBias = 0.0f;
        sampler_info.minLod = 0.0f;
        sampler_info.maxLod = 0.0f;

        for (uint32_t i = 0; i < m_cmd_buffers.size(); ++i) {
            m_samplers.push_back(mDevice->getDevice().createSampler(sampler_info));

            m_desc_configs[i].image_infos[0].imageLayout = ImageLayout::eShaderReadOnlyOptimal;
            m_desc_configs[i].image_infos[0].imageView = m_camera_image->get_img_view();
            m_desc_configs[i].image_infos[0].sampler = m_camera_image->get_sampler();

            m_desc_configs[i].writes[0].dstSet = m_desc_sets[i];
            m_desc_configs[i].writes[0].dstBinding = 0;
            m_desc_configs[i].writes[0].dstArrayElement = 0;
            m_desc_configs[i].writes[0].descriptorType = DescriptorType::eCombinedImageSampler;
            m_desc_configs[i].writes[0].descriptorCount = 1;
            m_desc_configs[i].writes[0].pImageInfo = &m_desc_configs[i].image_infos[0];
        }
    }

    void complex_context::reset_camera() {
//        auto surf_caps = m_gpu.getSurfaceCapabilitiesKHR(m_surface.get());
//        float y = 1.0f;
//
//        if (surf_caps.currentTransform == SurfaceTransformFlagBitsKHR::eRotate270)
//            y = -1.0f;
//
//        for (uint32_t i = 0; i < m_uniform_data.size(); ++i) {
//            m_mvp_data[i].set_camera_y(y);
//            m_uniform_data[i]->update(vector{m_mvp_data[i]});
//        }
    }

    void complex_context::release_rendering_resources() {
        mDevice->getDevice().waitIdle();

        for (auto &framebuffer: m_framebuffers) {
            mDevice->getDevice().destroyFramebuffer(framebuffer);
            mDevice->getDevice().waitIdle();
        }
        m_framebuffers.clear();
        m_depth_buffer.reset();
        for (auto &image_view: m_swapchain_img_views) {
            mDevice->getDevice().destroyImageView(image_view);
            mDevice->getDevice().waitIdle();
        }
        m_swapchain_img_views.clear();
        mDevice->getDevice().destroySwapchainKHR(m_swap_chain.release());
    }

    void complex_context::initialize_graphics(AHardwareBuffer *a_buffer) {
        if (is_initialized)
            release_rendering_resources();

        reset_surface(m_app.window);

        if (!is_initialized) {
            select_device_and_qfamily();
            create_logical_device();
            create_render_pass();
            create_data_buffers(a_buffer);
            create_graphics_pipeline();
        }

        reset_swapchain();
        reset_framebuffer_and_zbuffer();

        if (m_cmd_buffers.size() != m_images.size()) {
            reset_sync_and_cmd_resources();
            reset_ubos_samplers_and_descriptors();
        }

        reset_camera();

        is_initialized = true;
    }

    void complex_context::render_frame(const std::any &a_params, AHardwareBuffer *a_buffer) {
        glm::vec4 a_rgba = any_cast<glm::vec4>(a_params);

        auto img_idx = mDevice->getDevice().acquireNextImageKHR(m_swap_chain.get(), UINT64_MAX,
                                                                m_proc_semaphores[m_proc_si]);

        if (a_buffer) {
            m_camera_image->update(ImageUsageFlagBits::eSampled, SharingMode::eExclusive, a_buffer);
            m_desc_configs[img_idx.value].image_infos[0].imageView = m_camera_image->get_img_view();
        }

        CommandBufferBeginInfo begin_info;

        begin_info.flags = CommandBufferUsageFlagBits::eSimultaneousUse;
        begin_info.pInheritanceInfo = nullptr;

        ClearColorValue clear_color;
        memcpy(&clear_color.float32, &a_rgba, sizeof(clear_color.float32));

        array<ClearValue, 2> clear_values;

        clear_values[0].color = clear_color;
        clear_values[1].depthStencil = ClearDepthStencilValue{1.0f, 0};

        RenderPassBeginInfo render_pass_begin_info;

        render_pass_begin_info.renderPass = m_render_pass;
        render_pass_begin_info.framebuffer = m_framebuffers[img_idx.value];
        render_pass_begin_info.renderArea = Rect2D{{0, 0}, m_surface_extent};
        render_pass_begin_info.clearValueCount = clear_values.size();
        render_pass_begin_info.pClearValues = clear_values.data();

        DeviceSize buf_offset = 0;

        vector<BufferCopy> copy_vertex_regions{{0, 0, m_vertex_data->data_size()}};
        vector<BufferCopy> copy_index_regions{{0, 0, m_index_data->data_size()}};

        ImageMemoryBarrier cam_frag_barrier;

        cam_frag_barrier.oldLayout = ImageLayout::eUndefined;
        cam_frag_barrier.newLayout = ImageLayout::eShaderReadOnlyOptimal;
        cam_frag_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_EXTERNAL_KHR;
        cam_frag_barrier.dstQueueFamilyIndex = m_qfam_index;
        cam_frag_barrier.image = m_camera_image != nullptr ? m_camera_image->get() : nullptr;
        cam_frag_barrier.subresourceRange.aspectMask = ImageAspectFlagBits::eColor;
        cam_frag_barrier.subresourceRange.baseMipLevel = 0;
        cam_frag_barrier.subresourceRange.levelCount = 1;
        cam_frag_barrier.subresourceRange.baseArrayLayer = 0;
        cam_frag_barrier.subresourceRange.layerCount = 1;
        cam_frag_barrier.srcAccessMask = AccessFlags{0};
        cam_frag_barrier.dstAccessMask = AccessFlagBits::eShaderRead;

        auto wait_result = mDevice->getDevice().waitForFences(m_cmd_fences[img_idx.value], VK_TRUE, UINT64_MAX);

        mDevice->getDevice().updateDescriptorSets(m_desc_configs[img_idx.value].writes, nullptr);

        m_cmd_buffers[img_idx.value].begin(begin_info);

        m_cmd_buffers[img_idx.value].pipelineBarrier(PipelineStageFlagBits::eTopOfPipe,
                                                     PipelineStageFlagBits::eFragmentShader, static_cast<DependencyFlags>(0), 0, nullptr,
                                                     0, nullptr, 1, &cam_frag_barrier);

        m_cmd_buffers[img_idx.value].copyBuffer(m_vertex_data->get_staging(), m_vertex_data->get(),
                                                copy_vertex_regions);
        m_cmd_buffers[img_idx.value].copyBuffer(m_index_data->get_staging(), m_index_data->get(),
                                                copy_index_regions);
        m_cmd_buffers[img_idx.value].bindDescriptorSets(
                PipelineBindPoint::eGraphics, m_graphics_pipeline->get_layout(), 0, m_desc_sets[img_idx.value], nullptr);
        m_cmd_buffers[img_idx.value].beginRenderPass(render_pass_begin_info, SubpassContents::eInline);
        m_cmd_buffers[img_idx.value].bindPipeline(PipelineBindPoint::eGraphics, m_graphics_pipeline->get());
        m_cmd_buffers[img_idx.value].bindVertexBuffers(0, 1, &m_vertex_data->get(), &buf_offset);
        m_cmd_buffers[img_idx.value].bindIndexBuffer(m_index_data->get(), 0, IndexType::eUint16);
        m_cmd_buffers[img_idx.value].drawIndexed(data::index_set.size(), 1, 0, 0, 0);
        m_cmd_buffers[img_idx.value].endRenderPass();
        m_cmd_buffers[img_idx.value].end();

        SubmitInfo submit_info;
        auto wdst_mask = PipelineStageFlags(PipelineStageFlagBits::eTransfer);

        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &m_proc_semaphores[m_proc_si++];
        submit_info.pWaitDstStageMask = &wdst_mask;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &m_cmd_buffers[img_idx.value];
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &m_pres_semaphores[img_idx.value];

        mDevice->getDevice().resetFences(m_cmd_fences[img_idx.value]);
        m_pres_queue.submit(submit_info, m_cmd_fences[img_idx.value]);

        PresentInfoKHR pres_info;

        pres_info.waitSemaphoreCount = 1;
        pres_info.pWaitSemaphores = &m_pres_semaphores[img_idx.value];
        pres_info.swapchainCount = 1;
        pres_info.pSwapchains = &m_swap_chain.get();
        pres_info.pImageIndices = &img_idx.value;

        auto result = m_pres_queue.presentKHR(pres_info);

        if (m_proc_si == m_proc_semaphores.size())
            m_proc_si = 0;

        if (result == Result::eSuboptimalKHR)
            initialize_graphics(a_buffer);
        else if (result != Result::eSuccess)
            throw runtime_error{"Result is: " + to_string(result) + "Failed Rendering Frame."};
    }
}