//
// Created by leixing on 2025/1/19.
//
#include "VulkanHardwareBuffer.h"

#include "engine/VkCheckCpp.h"
#include "engine/VkCheck.h"
#include "engine/VulkanUtil.h"

#include "AndroidVulkanWrapper.h"

namespace engine {

    VulkanHardwareBuffer::VulkanHardwareBuffer(const VulkanInstance &vulkanInstance, const VulkanDevice &vulkanDevice,
                                               const VulkanCommandPool &commandPool, AHardwareBuffer *hardwareBuffer,
                                               const vk::Sampler &sampler,
                                               const vk::SamplerYcbcrConversion &conversion,
                                               uint32_t binding, uint32_t index, const vk::DescriptorSet &descriptorSet)
            : mVulkanInstance(vulkanInstance), mVulkanDevice(vulkanDevice), mCommandPool(commandPool), mSampler(sampler), mConversion(conversion),
              mDescriptorSet(descriptorSet),
              VulkanHostVisibleBuffer(binding, VulkanBufferType::ANDROID_HARDWARE_BUFFER, index) {

        const vk::Instance &instance = mVulkanInstance.getInstance();
        initAndroidVulkanWrapper(instance);

        AHardwareBuffer_Desc hardwareBufferDescription;
        AHardwareBuffer_describe(hardwareBuffer, &hardwareBufferDescription);

        mDataSize = hardwareBufferDescription.width * hardwareBufferDescription.height * hardwareBufferDescription.layers;

        updateBuffer(hardwareBuffer, 0);
    }

    VulkanHardwareBuffer::~VulkanHardwareBuffer() {
        vk::Device device = mVulkanDevice.getDevice();

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

        device.waitIdle();
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
        vk::ImageCreateInfo imageCreateInfo{};
        imageCreateInfo
                .setPNext(&externalFormat)
                .setFormat(formatInfo.format)
                .setFlags(vk::ImageCreateFlags{0})
                .setImageType(vk::ImageType::e2D)
                .setExtent(vk::Extent3D(hardwareBufferDescription.width, hardwareBufferDescription.height, 1))
                .setMipLevels(1)
                .setArrayLayers(hardwareBufferDescription.layers)
                .setSamples(vk::SampleCountFlagBits::e1)
                .setTiling(vk::ImageTiling::eOptimal)
                .setUsage(vk::ImageUsageFlagBits::eSampled)
                .setSharingMode(vk::SharingMode::eExclusive)
                .setQueueFamilyIndices({})
                .setInitialLayout(vk::ImageLayout::eUndefined);

        mImage = device.createImage(imageCreateInfo);

        vk::ImportAndroidHardwareBufferInfoANDROID hardwareBufferInfo;
        hardwareBufferInfo.setBuffer(hardwareBuffer);

        /**
         * Dedicated: "专用的" / "专属于"
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
        uint32_t memoryType = VulkanUtil::findMemoryTypeExternal(memProperties, hardwareBufferProperties.memoryTypeBits);

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

        vk::ImageSubresourceRange imageSubresourceRange{};
        imageSubresourceRange
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1);

        vk::ComponentMapping componentMapping{};
        componentMapping
                .setA(vk::ComponentSwizzle::eIdentity)
                .setR(vk::ComponentSwizzle::eIdentity)
                .setG(vk::ComponentSwizzle::eIdentity)
                .setB(vk::ComponentSwizzle::eIdentity);

        vk::ImageViewCreateInfo imageViewCreateInfo;
        imageViewCreateInfo
                .setPNext(&samplerYcbcrConversionInfo)
                .setFormat(formatInfo.format)
                .setImage(mImage)
                .setViewType(vk::ImageViewType::e2D)
                .setComponents(componentMapping)
                .setSubresourceRange(imageSubresourceRange);

        mImageView = device.createImageView(imageViewCreateInfo);


        vk::DescriptorImageInfo descriptorImageInfo;
        descriptorImageInfo
                .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setImageView(mImageView)
                .setSampler(mSampler);

        std::array<vk::DescriptorImageInfo, 1> descriptorImageInfos = {descriptorImageInfo};

        vk::WriteDescriptorSet writeDescriptorSet{};
        writeDescriptorSet
                .setDstSet(mDescriptorSet)
                .setDstBinding(getBinding())
                .setDstArrayElement(getIndex())
                .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                .setImageInfo(descriptorImageInfos);

        std::array<vk::WriteDescriptorSet, 1> writeDescriptorSets = {writeDescriptorSet};
        device.updateDescriptorSets(writeDescriptorSets, nullptr);
    }

} // engine