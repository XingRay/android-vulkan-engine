//
// Created by leixing on 2025/3/13.
//

#include "VulkanImage.h"
#include "engine/VulkanUtil.h"

namespace engine {

    VulkanImage::VulkanImage(const VulkanDevice &vulkanDevice, uint32_t width, uint32_t height)
            : mVulkanDevice(vulkanDevice), mWidth(width), mHeight(height) {

        const vk::Device &device = mVulkanDevice.getDevice();

        mMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        vk::PhysicalDeviceMemoryProperties properties = vulkanDevice.getPhysicalDevice().getMemoryProperties();
        vk::MemoryPropertyFlagBits memoryProperty = vk::MemoryPropertyFlagBits::eDeviceLocal;
        mImageFormat = vk::Format::eR8G8B8A8Srgb;

        vk::Extent3D extent3D;
        extent3D
                .setWidth(width)
                .setHeight(height)
                .setDepth(1);

        vk::ImageCreateInfo imageCreateInfo;
        imageCreateInfo
                .setImageType(vk::ImageType::e2D)
                .setExtent(extent3D)
                .setMipLevels(mMipLevels)
                .setArrayLayers(1)
                .setFormat(mImageFormat)
                        //VK_IMAGE_TILING_LINEAR texel按行的顺序排列
                        //VK_IMAGE_TILING_OPTIMAL texel按实现定义的顺序排列
                        //the tiling mode cannot be changed at a later time.
                        //如果希望能够直接访问图像内存中的texel，则必须使用VK_IMAGE_TILING_LINEAR
                .setTiling(vk::ImageTiling::eOptimal)
                        //VK_IMAGE_LAYOUT_UNDEFINED 不能被 GPU 使用，并且第一个转换将丢弃texel
                        //VK_IMAGE_TILING_OPTIMAL 不能被 GPU 使用，并且第一个转换将保留texel
                .setInitialLayout(vk::ImageLayout::eUndefined)
                .setUsage(vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled)
                .setSharingMode(vk::SharingMode::eExclusive)
                .setSamples(vk::SampleCountFlagBits::e1)
                        //用于稀疏纹理相关的标志位
                .setFlags(vk::ImageCreateFlags{});

        mImage = device.createImage(imageCreateInfo);

        vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(mImage);
        uint32_t memoryTypeIndex = VulkanUtil::findMemoryType(properties, memoryRequirements.memoryTypeBits, memoryProperty);

        vk::MemoryAllocateInfo memoryAllocateInfo;
        memoryAllocateInfo
                .setAllocationSize(memoryRequirements.size)
                .setMemoryTypeIndex(memoryTypeIndex);

        mDeviceMemory = device.allocateMemory(memoryAllocateInfo);

        device.bindImageMemory(mImage, mDeviceMemory, 0);

        mImageView = VulkanUtil::createImageView(device, mImage, mImageFormat, vk::ImageAspectFlagBits::eColor, mMipLevels);
    }

    VulkanImage::~VulkanImage() {
        vk::Device device = mVulkanDevice.getDevice();

        device.destroy(mImageView);
        device.destroy(mImage);
        device.unmapMemory(mDeviceMemory);
    }

    const vk::Image &VulkanImage::getImage() const {
        return mImage;
    }

    const vk::DeviceMemory &VulkanImage::getDeviceMemory() const {
        return mDeviceMemory;
    }

    const vk::ImageView &VulkanImage::getImageView() const {
        return mImageView;
    }

    vk::Format VulkanImage::getImageFormat() const {
        return mImageFormat;
    }

    uint32_t VulkanImage::getMipLevels() const {
        return mMipLevels;
    }

    uint32_t VulkanImage::getWidth() const {
        return mWidth;
    }

    uint32_t VulkanImage::getHeight() const {
        return mHeight;
    }

    void VulkanImage::transitionImageLayout(const VulkanCommandPool &commandPool) {
        commandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) -> void {
            recordCommandTransitionImageLayout(commandBuffer);
        });
    }

    void VulkanImage::recordCommandTransitionImageLayout(const vk::CommandBuffer &commandBuffer) {

        vk::ImageSubresourceRange imageSubresourceRange;
        imageSubresourceRange
                .setBaseMipLevel(0)
                .setLevelCount(mMipLevels)
                .setBaseArrayLayer(0)
                .setLayerCount(1)
                .setAspectMask(vk::ImageAspectFlagBits::eColor);

        vk::ImageMemoryBarrier imageMemoryBarrier;
        imageMemoryBarrier
                .setOldLayout(vk::ImageLayout::eUndefined)
                .setNewLayout(vk::ImageLayout::eTransferDstOptimal)
                .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
                .setDstQueueFamilyIndex(vk::QueueFamilyIgnored)
                .setImage(mImage)
                .setSubresourceRange(imageSubresourceRange)
                .setSrcAccessMask(vk::AccessFlags{})
                .setDstAccessMask(vk::AccessFlagBits::eTransferWrite);

        // 内存屏障
        std::vector<vk::MemoryBarrier> memoryBarriers = {};

        // 缓冲区内存屏障
        std::vector<vk::BufferMemoryBarrier> bufferMemoryBarriers = {};

        // 图像内存屏障
        std::array<vk::ImageMemoryBarrier, 1> imageMemoryBarriers = {imageMemoryBarrier};

        commandBuffer.pipelineBarrier(
                vk::PipelineStageFlagBits::eTopOfPipe,
                vk::PipelineStageFlagBits::eTransfer,
                vk::DependencyFlags{},
                memoryBarriers,
                bufferMemoryBarriers,
                imageMemoryBarriers
        );
    }

    void VulkanImage::copyFromBuffer(const VulkanCommandPool &vulkanCommandPool, vk::Buffer buffer) {
        vulkanCommandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandCopyFromBuffer(commandBuffer, buffer);
        });
    }

    void VulkanImage::recordCommandCopyFromBuffer(const vk::CommandBuffer &commandBuffer, vk::Buffer buffer) {
        vk::ImageSubresourceLayers imageSubresourceLayers;
        imageSubresourceLayers
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setMipLevel(0)
                .setBaseArrayLayer(0)
                .setLayerCount(1);

        vk::Offset3D offset{0, 0, 0};
        vk::Extent3D extent{mWidth, mHeight, 1};

        vk::BufferImageCopy bufferImageCopy;
        bufferImageCopy
                .setBufferOffset(0)
                .setBufferRowLength(0)
                .setBufferImageHeight(0)
                .setImageSubresource(imageSubresourceLayers)
                .setImageOffset(offset)
                .setImageExtent(extent);

        std::array<vk::BufferImageCopy, 1> regions = {bufferImageCopy};

        commandBuffer.copyBufferToImage(buffer, mImage, vk::ImageLayout::eTransferDstOptimal, regions);
    }
} // engine