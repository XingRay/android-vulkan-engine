//
// Created by leixing on 2025/3/13.
//

#include "VulkanImage.h"
#include "vklite/VulkanUtil.h"

namespace vklite {

    VulkanImage::VulkanImage(const VulkanDevice &vulkanDevice, uint32_t width, uint32_t height, vk::Format format)
            : mVulkanDevice(vulkanDevice), mWidth(width), mHeight(height) {

        const vk::Device &device = mVulkanDevice.getDevice();

        mMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        vk::PhysicalDeviceMemoryProperties properties = vulkanDevice.getPhysicalDevice().getMemoryProperties();
        vk::MemoryPropertyFlagBits memoryProperty = vk::MemoryPropertyFlagBits::eDeviceLocal;
//        mImageFormat = vk::Format::eR8G8B8A8Srgb;
        mImageFormat = format;

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

        uint32_t bytesPerPixel = VulkanUtil::getImageFormatBytesPerPixel(format);
        mVulkanStagingBuffer = std::make_unique<VulkanStagingBuffer>(vulkanDevice, width * height * bytesPerPixel);
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

    const vk::ImageView &VulkanImage::getImageView() const {
        return mImageView;
    }

    uint32_t VulkanImage::getMipLevels() const {
        return mMipLevels;
    }

    const vk::DeviceMemory &VulkanImage::getDeviceMemory() const {
        return mDeviceMemory;
    }

    vk::Format VulkanImage::getImageFormat() const {
        return mImageFormat;
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

    void VulkanImage::update(const VulkanCommandPool &vulkanCommandPool, const void *data, uint32_t size) {
        mVulkanStagingBuffer->updateBuffer(data, size);
        vulkanCommandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandCopyFromBuffer(commandBuffer, mVulkanStagingBuffer->getBuffer());
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

    void VulkanImage::generateMipmaps(const VulkanCommandPool &vulkanCommandPool) {
        vk::FormatProperties formatProperties = mVulkanDevice.getPhysicalDevice().getFormatProperties(mImageFormat);
        if (!(formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear)) {
            throw std::runtime_error("texture image format does not support linear tiling!");
        }

        vulkanCommandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandGenerateMipmaps(commandBuffer);
        });
    }

    void VulkanImage::recordCommandGenerateMipmaps(const vk::CommandBuffer &commandBuffer) {

        vk::ImageSubresourceRange subresourceRange{};
        subresourceRange
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setBaseArrayLayer(0)
                .setLayerCount(1)
                .setLevelCount(1);

        vk::ImageMemoryBarrier imageMemoryBarrier;
        imageMemoryBarrier
                .setImage(mImage)
                .setSubresourceRange(subresourceRange)
                .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
                .setDstQueueFamilyIndex(vk::QueueFamilyIgnored);

        int32_t mipWidth = static_cast<int32_t>(mWidth);
        int32_t mipHeight = static_cast<int32_t>(mHeight);

        for (int i = 1; i < mMipLevels; i++) {
            subresourceRange.setBaseMipLevel(i - 1);

            imageMemoryBarrier
                    .setOldLayout(vk::ImageLayout::eTransferDstOptimal)
                    .setNewLayout(vk::ImageLayout::eTransferSrcOptimal)
                    .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
                    .setDstAccessMask(vk::AccessFlagBits::eTransferRead);

            commandBuffer.pipelineBarrier(
                    vk::PipelineStageFlagBits::eTransfer,
                    vk::PipelineStageFlagBits::eTransfer,
                    vk::DependencyFlags{},
                    //memoryBarriers
                    {},
                    //bufferMemoryBarriers
                    {},
                    //imageMemoryBarriers
                    {imageMemoryBarrier}
            );

            std::array<vk::Offset3D, 2> srcOffsets{};
            srcOffsets[0] = vk::Offset3D{0, 0, 0};
            srcOffsets[1] = vk::Offset3D{mipWidth, mipHeight, 1};

            vk::ImageSubresourceLayers srcImageSubresourceLayers{};
            srcImageSubresourceLayers
                    .setAspectMask(vk::ImageAspectFlagBits::eColor)
                    .setMipLevel(i - 1)
                    .setBaseArrayLayer(0)
                    .setLayerCount(1);

            std::array<vk::Offset3D, 2> dstOffsets{};
            dstOffsets[0] = vk::Offset3D{0, 0, 0};
            dstOffsets[1] = vk::Offset3D{mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1};

            vk::ImageSubresourceLayers dstImageSubresourceLayers{};
            dstImageSubresourceLayers
                    .setAspectMask(vk::ImageAspectFlagBits::eColor)
                    .setMipLevel(i)
                    .setBaseArrayLayer(0)
                    .setLayerCount(1);

            vk::ImageBlit imageBlit{};
            imageBlit
                    .setSrcOffsets(srcOffsets)
                    .setSrcSubresource(srcImageSubresourceLayers)
                    .setDstOffsets(dstOffsets)
                    .setDstSubresource(dstImageSubresourceLayers);

            std::array<vk::ImageBlit, 1> regions = {imageBlit};

            commandBuffer.blitImage(
                    mImage,
                    vk::ImageLayout::eTransferSrcOptimal,
                    mImage,
                    vk::ImageLayout::eTransferDstOptimal,
                    regions,
                    vk::Filter::eLinear
            );

            imageMemoryBarrier
                    .setOldLayout(vk::ImageLayout::eTransferSrcOptimal)
                    .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                    .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
                    .setDstAccessMask(vk::AccessFlagBits::eShaderRead);

            commandBuffer.pipelineBarrier(
                    vk::PipelineStageFlagBits::eTransfer,
                    vk::PipelineStageFlagBits::eFragmentShader,
                    vk::DependencyFlags{},
                    //memoryBarriers
                    {},
                    //bufferMemoryBarriers
                    {},
                    //imageMemoryBarriers
                    {imageMemoryBarrier}
            );

            if (mipWidth > 1) {
                mipWidth /= 2;
            }

            if (mipHeight > 1) {
                mipHeight /= 2;
            }
        }

        subresourceRange.setBaseMipLevel(mMipLevels - 1);
        imageMemoryBarrier
                .setOldLayout(vk::ImageLayout::eTransferDstOptimal)
                .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
                .setDstAccessMask(vk::AccessFlagBits::eShaderRead);

        commandBuffer.pipelineBarrier(
                vk::PipelineStageFlagBits::eTransfer,
                vk::PipelineStageFlagBits::eFragmentShader,
                vk::DependencyFlags{},
                //memoryBarriers
                {},
                //bufferMemoryBarriers
                {},
                //imageMemoryBarriers
                {imageMemoryBarrier}
        );

    }
} // engine