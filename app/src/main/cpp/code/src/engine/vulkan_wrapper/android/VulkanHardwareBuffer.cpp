//
// Created by leixing on 2025/1/19.
//
#include "VulkanHardwareBuffer.h"

#include "engine/VkCheckCpp.h"
#include "engine/VkCheck.h"
#include "engine/VulkanUtil.h"

#include "engine/vulkan_wrapper/android/AndroidVulkanWrapper.h"

namespace engine {

    VulkanHardwareBuffer::VulkanHardwareBuffer(const VulkanInstance &vulkanInstance, const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool,
                                               AHardwareBuffer *hardwareBuffer, uint32_t binding, uint32_t index, const vk::DescriptorSet &descriptorSet)
            : mVulkanInstance(vulkanInstance), mVulkanDevice(vulkanDevice), mCommandPool(commandPool), mDescriptorSet(descriptorSet),
              VulkanBuffer(binding, VulkanBufferType::ANDROID_HARDWARE_BUFFER, index) {

        const vk::Device &device = mVulkanDevice.getDevice();
        const vk::Instance &instance = mVulkanInstance.getInstance();
        initAndroidVulkanWrapper(instance);

        AHardwareBuffer_Desc hardwareBufferDescription;
        AHardwareBuffer_describe(hardwareBuffer, &hardwareBufferDescription);

        mDataSize = hardwareBufferDescription.width * hardwareBufferDescription.height * hardwareBufferDescription.layers;

        // 获取 HardwareBuffer 属性
        vk::AndroidHardwareBufferPropertiesANDROID propertiesInfo;
        vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo;
        propertiesInfo.pNext = &formatInfo;

        CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID(device, hardwareBuffer, reinterpret_cast<VkAndroidHardwareBufferPropertiesANDROID *>(&propertiesInfo)));

        vk::SamplerYcbcrConversionCreateInfo conversionCreateInfo;
        vk::ExternalFormatANDROID externalFormat;
        conversionCreateInfo.pNext = &externalFormat;

        if (formatInfo.format == vk::Format::eUndefined) {
            externalFormat.externalFormat = formatInfo.externalFormat;
        }
        conversionCreateInfo.format = formatInfo.format;
        conversionCreateInfo.ycbcrModel = formatInfo.suggestedYcbcrModel;
        conversionCreateInfo.ycbcrRange = formatInfo.suggestedYcbcrRange;
        conversionCreateInfo.components = formatInfo.samplerYcbcrConversionComponents;
        conversionCreateInfo.xChromaOffset = formatInfo.suggestedXChromaOffset;
        conversionCreateInfo.yChromaOffset = formatInfo.suggestedYChromaOffset;
        conversionCreateInfo.chromaFilter = vk::Filter::eNearest;
        conversionCreateInfo.forceExplicitReconstruction = false;

        // mConversion = device.createSamplerYcbcrConversion(conv_info); // link error
        CALL_VK(vkCreateSamplerYcbcrConversion(device, reinterpret_cast<VkSamplerYcbcrConversionCreateInfo *>(&conversionCreateInfo), nullptr,
                                               reinterpret_cast<VkSamplerYcbcrConversion *>(&mConversion)));


        vk::SamplerCreateInfo samplerCreateInfo;
        vk::SamplerYcbcrConversionInfo conversionInfo;
        samplerCreateInfo.pNext = &conversionInfo;

        conversionInfo.conversion = mConversion;

        samplerCreateInfo.magFilter = vk::Filter::eNearest;
        samplerCreateInfo.minFilter = vk::Filter::eNearest;
        samplerCreateInfo.mipmapMode = vk::SamplerMipmapMode::eNearest;
        samplerCreateInfo.addressModeU = vk::SamplerAddressMode::eClampToEdge;
        samplerCreateInfo.addressModeV = vk::SamplerAddressMode::eClampToEdge;
        samplerCreateInfo.addressModeW = vk::SamplerAddressMode::eClampToEdge;
        samplerCreateInfo.mipLodBias = 0.0f;
        samplerCreateInfo.anisotropyEnable = false;
        samplerCreateInfo.maxAnisotropy = 1.0f;
        samplerCreateInfo.compareEnable = false;
        samplerCreateInfo.compareOp = vk::CompareOp::eNever;
        samplerCreateInfo.minLod = 0.0f;
        samplerCreateInfo.maxLod = 0.0f;
        samplerCreateInfo.borderColor = vk::BorderColor::eFloatOpaqueWhite;
        samplerCreateInfo.unnormalizedCoordinates = false;

        mSampler = device.createSampler(samplerCreateInfo);

        updateBuffer(hardwareBuffer, 0);
    }

    VulkanHardwareBuffer::~VulkanHardwareBuffer() {
        vk::Device device = mVulkanDevice.getDevice();
        const vk::Instance &instance = mVulkanInstance.getInstance();

        device.destroySampler(mSampler);

        // device.destroySamplerYcbcrConversion(mConversion); // link error
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
            throw std::runtime_error{"Data size differs. Cannot update image."};
        }

        // 获取 HardwareBuffer 属性
        vk::AndroidHardwareBufferPropertiesANDROID hardwareBufferProperties;
        vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo;
        hardwareBufferProperties.pNext = &formatInfo;

        CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID(device, hardwareBuffer, reinterpret_cast<VkAndroidHardwareBufferPropertiesANDROID *>(&hardwareBufferProperties)));

        vk::ExternalFormatANDROID externalFormat{};
        vk::ExternalMemoryImageCreateInfo externalMemoryImageCreateInfo{};
        externalFormat.setPNext(&externalMemoryImageCreateInfo);

        externalMemoryImageCreateInfo.setHandleTypes(vk::ExternalMemoryHandleTypeFlagBitsKHR::eAndroidHardwareBufferANDROID);

        if (formatInfo.format == vk::Format::eUndefined) {
            externalFormat.externalFormat = formatInfo.externalFormat;
        }

        // 创建 Vulkan 图像
        vk::ImageCreateInfo imageCreateInfo(
                {},                                      // 标志
                vk::ImageType::e2D,                     // 图像类型
                formatInfo.format,                     // 格式
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
                &externalFormat                       // pNext
        );

        mImage = device.createImage(imageCreateInfo);

        vk::ImportAndroidHardwareBufferInfoANDROID hardwareBufferInfo;
        hardwareBufferInfo.setBuffer(hardwareBuffer);

        /**
         * Dedicated: 专用的/专属于
         * 某块内存专门服务于某个特定的 Vulkan 资源（如 Image 或 Buffer）
         * 当前分配的内存将专门用于某个特定的 Image 或 Buffer。
         * 驱动可能会根据此信息优化内存布局或访问路径。
         */
        vk::MemoryDedicatedAllocateInfo memoryDedicatedAllocateInfo{};
        memoryDedicatedAllocateInfo
                .setImage(mImage)       // 关联的 Image（二选一）
                .setBuffer(nullptr)     // 关联的 Buffer（二选一）
                .setPNext(&hardwareBufferInfo);

        // 获取图像的内存需求
        vk::PhysicalDeviceMemoryProperties memProperties = mVulkanDevice.getPhysicalDevice().getMemoryProperties();
        uint32_t memoryType = VulkanUtil::findMemoryType(memProperties, hardwareBufferProperties.memoryTypeBits, vk::MemoryPropertyFlagBits{});

        // 分配内存并绑定到图像
        vk::MemoryAllocateInfo memoryAllocateInfo{};
        memoryAllocateInfo
                .setAllocationSize(hardwareBufferProperties.allocationSize)
                .setMemoryTypeIndex(memoryType)
                .setPNext(&memoryDedicatedAllocateInfo);

        mMemory = device.allocateMemory(memoryAllocateInfo);

        vk::BindImageMemoryInfo bindImageMemoryInfo;
        bindImageMemoryInfo
                .setImage(mImage)
                .setMemory(mMemory)
                .setMemoryOffset(0);

        //device.bindImageMemory2KHR(bindImageMemoryInfo); // link error
        vkBindImageMemory2KHR(device, 1, reinterpret_cast<VkBindImageMemoryInfo *>(&bindImageMemoryInfo));

        vk::ImageMemoryRequirementsInfo2 imageMemoryRequirementsInfo{};
        imageMemoryRequirementsInfo.setImage(mImage);

        vk::MemoryRequirements2 memoryRequirements;
        vk::MemoryDedicatedRequirements memoryDedicatedRequirements;
        memoryRequirements.pNext = &memoryDedicatedRequirements;

//        device.getImageMemoryRequirements2KHR(&imageMemoryRequirementsInfo, &memoryRequirements); //link error
        vkGetImageMemoryRequirements2KHR(device, reinterpret_cast<const VkImageMemoryRequirementsInfo2 *>(&imageMemoryRequirementsInfo),
                                         reinterpret_cast<VkMemoryRequirements2 *>(&memoryRequirements));

        if (!memoryDedicatedRequirements.prefersDedicatedAllocation || !memoryDedicatedRequirements.requiresDedicatedAllocation) {
            return;
        }


        vk::SamplerYcbcrConversionInfo samplerYcbcrConversionInfo;
        samplerYcbcrConversionInfo.setConversion(mConversion);

        vk::ImageViewCreateInfo imageViewCreateInfo;

        imageViewCreateInfo.pNext = &samplerYcbcrConversionInfo;
        imageViewCreateInfo.format = formatInfo.format;
        imageViewCreateInfo.image = mImage;
        imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
        imageViewCreateInfo.components = {
                vk::ComponentSwizzle::eR,
                vk::ComponentSwizzle::eG,
                vk::ComponentSwizzle::eB,
                vk::ComponentSwizzle::eA,
        };
        imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        mImageView = device.createImageView(imageViewCreateInfo);
    }

} // engine