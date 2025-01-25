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

#ifndef NCV_GRAPHICS_COMPLETE_CONTEXT_HPP
#define NCV_GRAPHICS_COMPLETE_CONTEXT_HPP

#include <graphics/vulkan_context.hpp>
#include <metadata/version.hpp>
#include <graphics/data/types.hpp>
#include <graphics/resources/types.hpp>

#include <map>
#include <any>

class android_app;

namespace graphics {
    class pipeline;

    class complex_context : public vulkan_context {
    public:

        typedef resources::buffer<resources::device_upload, data::index_format> index_data;
        typedef resources::buffer<resources::host, data::model_view_projection> uniform_data;
        typedef resources::buffer<resources::device_upload, data::vertex_format> vertex_data;
        typedef resources::image<resources::external> camera_data;
        typedef resources::image<resources::device> depth_data;
        typedef resources::image<resources::device_upload, data::stbi_uc> texture_data;

        struct descriptor_configuration {
            std::array<vk::DescriptorBufferInfo, 1> buffer_infos;
            std::array<vk::DescriptorImageInfo, 1> image_infos;
            std::array<vk::WriteDescriptorSet, 2> writes;
        };

        explicit complex_context(const std::string &a_app_name, const android_app &app);

        ~complex_context();

        void initialize_graphics(AHardwareBuffer *a_buffer);

        void render_frame(const std::any &a_params, AHardwareBuffer *a_buffer = nullptr);

    protected:

        void reset_surface(ANativeWindow *a_window);

        void select_device_and_qfamily();

        void create_logical_device();

        void create_render_pass();

        void create_graphics_pipeline();

        void create_data_buffers(AHardwareBuffer *a_buffer);

        void reset_swapchain();

        void reset_framebuffer_and_zbuffer();

        void reset_sync_and_cmd_resources();

        void reset_ubos_samplers_and_descriptors();

        void reset_camera();

        void release_rendering_resources();

    private:

        std::vector<const char *> m_requested_gl_extensions = {
                VK_KHR_SURFACE_EXTENSION_NAME,
                VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
                VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME,
                VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME,
                VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
        };

        std::vector<const char *> m_requested_gl_layers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::vector<const char *> m_requested_dev_extensions = {
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

        bool is_initialized = false;

#ifdef NCV_VULKAN_VALIDATION_ENABLED
        const bool m_enable_validation = true;
#else
        const bool m_enable_validation = false;
#endif

        std::vector<vk::PhysicalDevice> m_devices;

        vk::PhysicalDevice m_gpu;

        vk::UniqueInstance m_instance;
        vk::UniqueSurfaceKHR m_surface;
        vk::UniqueDevice m_device;
        vk::UniqueSwapchainKHR m_swap_chain;
        vk::UniqueCommandPool m_cmd_pool;

        std::vector<vk::CommandBuffer> m_cmd_buffers;
        std::vector<vk::Fence> m_cmd_fences;
        std::vector<vk::Image> m_images;
        std::vector<vk::ImageView> m_swapchain_img_views;
        std::vector<vk::Framebuffer> m_framebuffers;
        std::vector<vk::Semaphore> m_proc_semaphores;
        std::vector<vk::Semaphore> m_pres_semaphores;

        uint32_t m_proc_si{0};

        vk::RenderPass m_render_pass = nullptr;

        vk::Queue m_pres_queue;
        int32_t m_qfam_index = -1;

        vk::Extent2D m_surface_extent;

        std::shared_ptr<pipeline> m_graphics_pipeline = nullptr;
        std::shared_ptr<vertex_data> m_vertex_data = nullptr;
        std::shared_ptr<index_data> m_index_data = nullptr;

        std::vector<vk::Sampler> m_samplers;
        std::shared_ptr<camera_data> m_camera_image = nullptr;
        std::shared_ptr<depth_data> m_depth_buffer = nullptr;

        std::shared_ptr<data::texture> m_stbi_data = nullptr;
        vk::Extent3D m_stbi_extent;
        std::shared_ptr<texture_data> m_texture_data = nullptr;

        vk::UniqueDescriptorPool m_desc_pool;
        std::vector<vk::DescriptorSet> m_desc_sets;
        std::vector<descriptor_configuration> m_desc_configs;

        std::vector<data::model_view_projection> m_mvp_data;
        std::vector<std::shared_ptr<uniform_data>> m_uniform_data;

        vk::DebugUtilsMessengerEXT m_debug_msg;

        std::chrono::time_point<std::chrono::steady_clock> m_ref_time;

        const android_app &m_app;
    };
}

#endif //NCV_GRAPHICS_COMPLETE_CONTEXT_HPP
