//
// Created by leixing on 2025/1/19.
//
#include "VulkanHardwareBuffer.h"

#include "engine/VkCheckCpp.h"
#include "engine/VkCheck.h"
#include "engine/VulkanUtil.h"

namespace engine {

    VulkanHardwareBuffer::VulkanHardwareBuffer(const VulkanInstance &vulkanInstance, const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool,
                                               AHardwareBuffer *hardwareBuffer, uint32_t binding, uint32_t index, const vk::DescriptorSet& descriptorSet)
            : mVulkanInstance(vulkanInstance), mVulkanDevice(vulkanDevice),mCommandPool(commandPool),mDescriptorSet(descriptorSet), VulkanBuffer(binding, VulkanBufferType::ANDROID_HARDWARE_BUFFER, index) {

        const vk::Device &device = mVulkanDevice.getDevice();
        const vk::Instance &instance = mVulkanInstance.getInstance();

        AHardwareBuffer_Desc buffer_desc;
        AHardwareBuffer_describe(hardwareBuffer, &buffer_desc);

        mDataSize = buffer_desc.width * buffer_desc.height * buffer_desc.layers;

        // 获取 HardwareBuffer 属性
        vk::AndroidHardwareBufferFormatPropertiesANDROID format_info;
        vk::AndroidHardwareBufferPropertiesANDROID properties_info;

        properties_info.pNext = &format_info;

        static auto vkGetAndroidHardwareBufferPropertiesANDROID = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID) vkGetInstanceProcAddr(
                instance, "vkGetAndroidHardwareBufferPropertiesANDROID");
        CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID(device, hardwareBuffer, reinterpret_cast<VkAndroidHardwareBufferPropertiesANDROID *>(&properties_info)));

        vk::ExternalMemoryImageCreateInfo ext_mem_info;
        ext_mem_info.handleTypes = vk::ExternalMemoryHandleTypeFlagBitsKHR::eAndroidHardwareBufferANDROID;

        vk::ExternalFormatANDROID external_format;
        vk::SamplerYcbcrConversionCreateInfo conv_info;

        if (format_info.format == vk::Format::eUndefined) {
            external_format.externalFormat = format_info.externalFormat;
            conv_info.pNext = &external_format;
            conv_info.format = vk::Format::eUndefined;
            conv_info.ycbcrModel = format_info.suggestedYcbcrModel;
        } else {
            conv_info.pNext = &external_format;
            conv_info.format = format_info.format;
            conv_info.ycbcrModel = vk::SamplerYcbcrModelConversion::eYcbcr601;
        }

        conv_info.ycbcrRange = format_info.suggestedYcbcrRange;
        conv_info.components = format_info.samplerYcbcrConversionComponents;
        conv_info.xChromaOffset = format_info.suggestedXChromaOffset;
        conv_info.yChromaOffset = format_info.suggestedYChromaOffset;
        conv_info.chromaFilter = vk::Filter::eNearest;
        conv_info.forceExplicitReconstruction = false;

        // mConversion = device.createSamplerYcbcrConversion(conv_info); // link error
        static auto vkCreateSamplerYcbcrConversion = (PFN_vkCreateSamplerYcbcrConversion) vkGetInstanceProcAddr(
                instance, "vkCreateSamplerYcbcrConversion");
        CALL_VK(vkCreateSamplerYcbcrConversion(device, reinterpret_cast<VkSamplerYcbcrConversionCreateInfo *>(&conv_info), nullptr, reinterpret_cast<VkSamplerYcbcrConversion *>(&mConversion)));


        vk::SamplerYcbcrConversionInfo conv_sampler_info;

        conv_sampler_info.conversion = mConversion;

        vk::SamplerCreateInfo sampler_info;

        sampler_info.pNext = &conv_sampler_info;
        sampler_info.magFilter = vk::Filter::eNearest;
        sampler_info.minFilter = vk::Filter::eNearest;
        sampler_info.mipmapMode = vk::SamplerMipmapMode::eNearest;
        sampler_info.addressModeU = vk::SamplerAddressMode::eClampToEdge;
        sampler_info.addressModeV = vk::SamplerAddressMode::eClampToEdge;
        sampler_info.addressModeW = vk::SamplerAddressMode::eClampToEdge;
        sampler_info.mipLodBias = 0.0f;
        sampler_info.anisotropyEnable = false;
        sampler_info.maxAnisotropy = 1.0f;
        sampler_info.compareEnable = false;
        sampler_info.compareOp = vk::CompareOp::eNever;
        sampler_info.minLod = 0.0f;
        sampler_info.maxLod = 0.0f;
        sampler_info.borderColor = vk::BorderColor::eFloatOpaqueWhite;
        sampler_info.unnormalizedCoordinates = false;

        mSampler = device.createSampler(sampler_info);

        updateBuffer(hardwareBuffer, 0);
    }

    VulkanHardwareBuffer::~VulkanHardwareBuffer() {
        vk::Device device = mVulkanDevice.getDevice();
        const vk::Instance &instance = mVulkanInstance.getInstance();

        device.destroySampler(mSampler);

        // device.destroySamplerYcbcrConversion(mConversion); // link error
        static auto vkDestroySamplerYcbcrConversion = (PFN_vkDestroySamplerYcbcrConversion) vkGetInstanceProcAddr(
                instance, "vkDestroySamplerYcbcrConversion");
        vkDestroySamplerYcbcrConversion(device, mConversion, nullptr);


        device.destroyImageView(mImageView);
        device.destroyImage(mImage);
        device.freeMemory(mMemory);
    }

    const vk::ImageView &VulkanHardwareBuffer::getImageView() const {
        return mImageView;
    }

    const vk::Sampler &VulkanHardwareBuffer::getSampler() const {
        return mSampler;
    }

    void VulkanHardwareBuffer::updateBuffer(void *data, uint32_t size) {
        AHardwareBuffer *hardwareBuffer = static_cast<AHardwareBuffer *>(data);

        const vk::Device &device = mVulkanDevice.getDevice();
        const vk::Instance &instance = mVulkanInstance.getInstance();

        // clean old resource
        if (mImageView != nullptr) {
            device.destroyImageView(mImageView);
        }
        if (mImage != nullptr) {
            device.destroyImage(mImage);
        }
        if (mMemory != nullptr) {
            device.freeMemory(mMemory);
        }

        AHardwareBuffer_Desc hardwareBufferDescription;
        AHardwareBuffer_describe(hardwareBuffer, &hardwareBufferDescription);
        if (hardwareBufferDescription.width * hardwareBufferDescription.height * hardwareBufferDescription.layers != mDataSize) {
            LOG_E("hardwareBufferDescription:[%dx%d, layers:%d] => %d", hardwareBufferDescription.width, hardwareBufferDescription.height, hardwareBufferDescription.layers,
                  hardwareBufferDescription.width * hardwareBufferDescription.height * hardwareBufferDescription.layers);
            LOG_E("mDataSize:%ld", mDataSize);
//            throw std::runtime_error{"Data size differs. Cannot update image."};
            return;
        }

        // 获取 HardwareBuffer 属性
        vk::AndroidHardwareBufferPropertiesANDROID hardwareBufferProperties;
        vk::AndroidHardwareBufferFormatPropertiesANDROID format_info;
        hardwareBufferProperties.pNext = &format_info;

        static auto vkGetAndroidHardwareBufferPropertiesANDROID = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID) vkGetInstanceProcAddr(
                instance, "vkGetAndroidHardwareBufferPropertiesANDROID");
        CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID(device, hardwareBuffer, reinterpret_cast<VkAndroidHardwareBufferPropertiesANDROID *>(&hardwareBufferProperties)));

        vk::ExternalMemoryImageCreateInfo ext_mem_info{};
        ext_mem_info.setHandleTypes(vk::ExternalMemoryHandleTypeFlagBitsKHR::eAndroidHardwareBufferANDROID);

        vk::ExternalFormatANDROID external_format{};
        external_format.setPNext(&ext_mem_info);

        if (format_info.format == vk::Format::eUndefined) {
            external_format.externalFormat = format_info.externalFormat;
        }

        // 创建 Vulkan 图像
        vk::ImageCreateInfo imageCreateInfo(
                {},                                      // 标志
                vk::ImageType::e2D,                     // 图像类型
                format_info.format,                     // 格式
                vk::Extent3D(hardwareBufferDescription.width, hardwareBufferDescription.height, 1),         // 尺寸
                1,                                      // Mip 级别
                hardwareBufferDescription.layers,                     // 数组层数
                vk::SampleCountFlagBits::e1,            // 采样数
                vk::ImageTiling::eOptimal,              // 图像布局
                vk::ImageUsageFlagBits::eSampled,       // 用途
                vk::SharingMode::eExclusive,            // 共享模式
                0,                                      // 队列族数量
                nullptr,                                // 队列族索引
                vk::ImageLayout::eUndefined,             // 初始布局
                &external_format                       // pNext
        );

        mImage = device.createImage(imageCreateInfo);

        vk::ImportAndroidHardwareBufferInfoANDROID import_info;
        import_info.setBuffer(hardwareBuffer);

        vk::MemoryDedicatedAllocateInfo memoryDedicatedAllocateInfo{};
        memoryDedicatedAllocateInfo
                .setImage(mImage)
                .setBuffer(nullptr)
                .setPNext(&import_info);

        // 获取图像的内存需求
        vk::PhysicalDeviceMemoryProperties memProperties = mVulkanDevice.getPhysicalDevice().getMemoryProperties();
        vk::MemoryRequirements memRequirements = device.getImageMemoryRequirements(mImage);
        uint32_t memoryType = VulkanUtil::findMemoryType(memProperties, hardwareBufferProperties.memoryTypeBits, vk::MemoryPropertyFlagBits{0});

        // 分配内存并绑定到图像
        vk::MemoryAllocateInfo memoryAllocateInfo{};
        memoryAllocateInfo
                .setAllocationSize(hardwareBufferProperties.allocationSize)
                .setMemoryTypeIndex(memoryType)
                .setPNext(&memoryDedicatedAllocateInfo);

        mMemory = device.allocateMemory(memoryAllocateInfo);
//        device.bindImageMemory(mImage, mMemory, 0);
        vk::BindImageMemoryInfo bindImageMemoryInfo;
        bindImageMemoryInfo
                .setImage(mImage)
                .setMemory(mMemory)
                .setMemoryOffset(0);

        //device.bindImageMemory2KHR(bindImageMemoryInfo); // link error
        static auto vkBindImageMemory2KHR = (PFN_vkBindImageMemory2KHR) vkGetInstanceProcAddr(
                instance, "vkBindImageMemory2KHR");
        vkBindImageMemory2KHR(device, 1, reinterpret_cast<VkBindImageMemoryInfo *>(&bindImageMemoryInfo));

        vk::ImageMemoryRequirementsInfo2 imageMemoryRequirementsInfo{};
        imageMemoryRequirementsInfo.setImage(mImage);

        vk::MemoryDedicatedRequirements memoryDedicatedRequirements;
        vk::MemoryRequirements2 memoryRequirements;
        memoryRequirements.pNext = &memoryDedicatedRequirements;

//        device.getImageMemoryRequirements2KHR(&imageMemoryRequirementsInfo, &memoryRequirements); //link error
        static auto vkGetImageMemoryRequirements2KHR = (PFN_vkGetImageMemoryRequirements2KHR) vkGetInstanceProcAddr(
                instance, "vkGetImageMemoryRequirements2KHR");
        vkGetImageMemoryRequirements2KHR(device, reinterpret_cast<const VkImageMemoryRequirementsInfo2 *>(&imageMemoryRequirementsInfo),
                                         reinterpret_cast<VkMemoryRequirements2 *>(&memoryRequirements));

        if (!memoryDedicatedRequirements.prefersDedicatedAllocation || !memoryDedicatedRequirements.requiresDedicatedAllocation) {
            return;
        }


        vk::SamplerYcbcrConversionInfo samplerYcbcrConversionInfo;
        samplerYcbcrConversionInfo.setConversion(mConversion);

        vk::ImageViewCreateInfo imageViewCreateInfo;

        imageViewCreateInfo.pNext = &samplerYcbcrConversionInfo;
        imageViewCreateInfo.format = format_info.format;
        imageViewCreateInfo.image = mImage;
        imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
        imageViewCreateInfo.components = {
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
        };
        imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        mImageView = device.createImageView(imageViewCreateInfo);
    }


} // engine