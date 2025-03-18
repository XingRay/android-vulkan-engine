//
// Created by leixing on 2025/1/7.
//

#include "VulkanSampler.h"
#include "engine/VulkanUtil.h"

namespace engine {

    VulkanSampler::VulkanSampler(const VulkanDevice &vulkanDevice, uint32_t width, uint32_t height, uint32_t channels)
            : mVulkanDevice(vulkanDevice),
              mVulkanImage(vulkanDevice, width, height),
              mVulkanStagingBuffer(vulkanDevice, width * height * channels) {

//        mImageSize = width * height * channels;

        vk::SamplerCreateInfo samplerCreateInfo;
        samplerCreateInfo
                .setMagFilter(vk::Filter::eLinear)
                .setMinFilter(vk::Filter::eLinear)
                .setAddressModeU(vk::SamplerAddressMode::eRepeat)
                .setAddressModeV(vk::SamplerAddressMode::eRepeat)
                .setAddressModeW(vk::SamplerAddressMode::eRepeat)
                .setAnisotropyEnable(vk::True)
                .setMaxAnisotropy(mVulkanDevice.getMaxSamplerAnisotropy())
                .setBorderColor(vk::BorderColor::eIntOpaqueBlack)
                        // 是否使用不归一化的坐标(x:[0~width], y:[0~height])
                        // 设置为 false 就是使用归一化坐标 (x:[0~1.0], y:[0~1.0])
                .setUnnormalizedCoordinates(vk::False)
                .setCompareEnable(vk::False)
                .setCompareOp(vk::CompareOp::eAlways)
                .setMipmapMode(vk::SamplerMipmapMode::eLinear)
                .setMipLodBias(0.0f)
                .setMinLod(0.0f)
                        // 测试： 强制使用高等级 mipmap （更模糊）
                        //.setMinLod(static_cast<float >(mVulkanImage.getMipLevels() / 2))
                .setMaxLod(static_cast<float >(mVulkanImage.getMipLevels()));

        mSampler = mVulkanDevice.getDevice().createSampler(samplerCreateInfo);
    }

    VulkanSampler::~VulkanSampler() {
        vk::Device device = mVulkanDevice.getDevice();

        device.destroy(mSampler);
    }

    const vk::Image &VulkanSampler::getImage() const {
        return mVulkanImage.getImage();
    }

    const vk::DeviceMemory &VulkanSampler::getDeviceMemory() const {
        return mVulkanImage.getDeviceMemory();
    }

    const vk::ImageView &VulkanSampler::getImageView() const {
        return mVulkanImage.getImageView();
    }

    const vk::Sampler &VulkanSampler::getSampler() const {
        return mSampler;
    }

    void VulkanSampler::updateBuffer(const VulkanCommandPool &vulkanCommandPool, const void *data, uint32_t size) {
        mVulkanStagingBuffer.updateBuffer(data, size);
        mVulkanImage.copyFromBuffer(vulkanCommandPool, mVulkanStagingBuffer.getBuffer());
    }

    void VulkanSampler::generateMipmaps(const VulkanCommandPool &vulkanCommandPool) {
        vk::FormatProperties formatProperties = mVulkanDevice.getPhysicalDevice().getFormatProperties(mVulkanImage.getImageFormat());
        if (!(formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear)) {
            throw std::runtime_error("texture image format does not support linear tiling!");
        }

        vulkanCommandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandGenerateMipmaps(commandBuffer);
        });
    }

    void VulkanSampler::recordCommandGenerateMipmaps(const vk::CommandBuffer &commandBuffer) {

        vk::ImageSubresourceRange subresourceRange{};
        subresourceRange
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setBaseArrayLayer(0)
                .setLayerCount(1)
                .setLevelCount(1);

        vk::ImageMemoryBarrier imageMemoryBarrier;
        imageMemoryBarrier
                .setImage(mVulkanImage.getImage())
                .setSubresourceRange(subresourceRange)
                .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
                .setDstQueueFamilyIndex(vk::QueueFamilyIgnored);

        int32_t mipWidth = static_cast<int32_t>(mVulkanImage.getWidth());
        int32_t mipHeight = static_cast<int32_t>(mVulkanImage.getHeight());

        for (int i = 1; i < mVulkanImage.getMipLevels(); i++) {
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
                    mVulkanImage.getImage(),
                    vk::ImageLayout::eTransferSrcOptimal,
                    mVulkanImage.getImage(),
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

        subresourceRange.setBaseMipLevel(mVulkanImage.getMipLevels() - 1);
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

    std::vector<vk::DescriptorImageInfo> VulkanSampler::createDescriptorImageInfos() {
        vk::DescriptorImageInfo samplerDescriptorImageInfo;
        samplerDescriptorImageInfo
                .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setImageView(mVulkanImage.getImageView())
                .setSampler(mSampler);

        std::vector<vk::DescriptorImageInfo> samplerDescriptorImageInfos = {samplerDescriptorImageInfo};
        return samplerDescriptorImageInfos;

    }
} // engine