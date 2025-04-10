//
// Created by leixing on 2025/1/7.
//

#include "VulkanSampler.h"
#include "engine/VulkanUtil.h"

namespace engine {

    VulkanSampler::VulkanSampler(const VulkanDevice &vulkanDevice, float maxLoad/*uint32_t width, uint32_t height, uint32_t channels*/)
            : mVulkanDevice(vulkanDevice)/*,
              mVulkanImage(vulkanDevice, width, height),
              mVulkanStagingBuffer(vulkanDevice, width * height * channels)*/ {

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
                .setMaxLod(maxLoad);

        mSampler = mVulkanDevice.getDevice().createSampler(samplerCreateInfo);
    }

    VulkanSampler::~VulkanSampler() {
        vk::Device device = mVulkanDevice.getDevice();

        device.destroy(mSampler);
    }

    const vk::Sampler &VulkanSampler::getSampler() const {
        return mSampler;
    }

} // engine