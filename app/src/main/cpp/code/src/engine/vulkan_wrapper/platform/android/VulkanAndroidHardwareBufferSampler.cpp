//
// Created by leixing on 2025/1/26.
//

#include "VulkanAndroidHardwareBufferSampler.h"
#include "engine/VkCheck.h"
#include "engine/Log.h"

namespace engine {

    VulkanAndroidHardwareBufferSampler::VulkanAndroidHardwareBufferSampler(const VulkanDevice &vulkanDevice,
                                                                           const AndroidHardwareBuffer& androidHardwareBuffer,
                                                                           const VulkanAndroidSamplerYcbcrConversion &vulkanAndroidSamplerYcbcrConversion)
            : mVulkanDevice(vulkanDevice),
            mImage(vulkanDevice, androidHardwareBuffer, vulkanAndroidSamplerYcbcrConversion) {
        vk::Device device = vulkanDevice.getDevice();

        vk::SamplerCreateInfo samplerCreateInfo;
        vk::SamplerYcbcrConversionInfo conversionInfo;
        conversionInfo
                .setConversion(vulkanAndroidSamplerYcbcrConversion.getSamplerYcbcrConversion());

        samplerCreateInfo
                .setPNext(&conversionInfo)
                .setMagFilter(vk::Filter::eNearest)
                .setMinFilter(vk::Filter::eNearest)
                .setMipmapMode(vk::SamplerMipmapMode::eNearest)
                .setAddressModeU(vk::SamplerAddressMode::eClampToEdge)
                .setAddressModeV(vk::SamplerAddressMode::eClampToEdge)
                .setAddressModeW(vk::SamplerAddressMode::eClampToEdge)
                .setMipLodBias(0.0f)
                .setAnisotropyEnable(false)
                .setMaxAnisotropy(1.0f)
                .setCompareEnable(false)
                .setCompareOp(vk::CompareOp::eNever)
                .setMinLod(0.0f)
                .setMaxLod(0.0f)
                .setBorderColor(vk::BorderColor::eFloatOpaqueWhite)
                .setUnnormalizedCoordinates(false);

        mSampler = device.createSampler(samplerCreateInfo);
    }

    VulkanAndroidHardwareBufferSampler::~VulkanAndroidHardwareBufferSampler() {
        vk::Device device = mVulkanDevice.getDevice();

        device.destroySampler(mSampler);
    }

    vk::Sampler VulkanAndroidHardwareBufferSampler::getSampler() const {
        return mSampler;
    }

    std::vector<vk::DescriptorImageInfo> VulkanAndroidHardwareBufferSampler::createDescriptorImageInfos() {
        vk::DescriptorImageInfo samplerDescriptorImageInfo;
        samplerDescriptorImageInfo
                .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setImageView(mImage.getImageView())
                .setSampler(mSampler);

        std::vector<vk::DescriptorImageInfo> samplerDescriptorImageInfos = {samplerDescriptorImageInfo};
        return samplerDescriptorImageInfos;
    }
} // engine