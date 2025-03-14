//
// Created by leixing on 2025/1/26.
//

#include "VulkanAndroidHardwareSampler.h"
#include "engine/VkCheck.h"
#include "engine/Log.h"

namespace engine {

//    VulkanAndroidHardwareSampler::VulkanAndroidHardwareSampler(const VulkanDevice &vulkanDevice, AHardwareBuffer *hardwareBuffer) : VulkanAndroidHardwareSampler(vulkanDevice, [&]() {
//        vk::Device device = vulkanDevice.getDevice();
//        // 获取 HardwareBuffer 属性
//        vk::AndroidHardwareBufferPropertiesANDROID propertiesInfo;
//        vk::AndroidHardwareBufferFormatPropertiesANDROID formatInfo;
//        propertiesInfo.pNext = &formatInfo;
//
//        CALL_VK(vkGetAndroidHardwareBufferPropertiesANDROID(device, hardwareBuffer, reinterpret_cast<VkAndroidHardwareBufferPropertiesANDROID *>(&propertiesInfo)));
//        return formatInfo;
//    }()) {
//        //...
//    }

    VulkanAndroidHardwareSampler::VulkanAndroidHardwareSampler(const VulkanDevice &vulkanDevice,
                                                               const vk::AndroidHardwareBufferFormatPropertiesANDROID &formatInfo)
            : mVulkanDevice(vulkanDevice) {
        vk::Device device = vulkanDevice.getDevice();

        vk::ExternalFormatANDROID externalFormat;
        if (formatInfo.format == vk::Format::eUndefined) {
            externalFormat.externalFormat = formatInfo.externalFormat;
        }

        vk::SamplerYcbcrModelConversion ycbcrModel;
        if (formatInfo.format == vk::Format::eUndefined) {
            ycbcrModel = formatInfo.suggestedYcbcrModel;
        } else {
            ycbcrModel = vk::SamplerYcbcrModelConversion::eYcbcr601;
        }

        vk::SamplerYcbcrConversionCreateInfo conversionCreateInfo;
        conversionCreateInfo
                .setPNext(&externalFormat)
                .setYcbcrModel(ycbcrModel)
                .setFormat(formatInfo.format)
                .setYcbcrRange(formatInfo.suggestedYcbcrRange)
                .setComponents(formatInfo.samplerYcbcrConversionComponents)
                .setXChromaOffset(formatInfo.suggestedXChromaOffset)
                .setYChromaOffset(formatInfo.suggestedYChromaOffset)
                .setChromaFilter(vk::Filter::eNearest)
                .setForceExplicitReconstruction(false);

        // mConversion = device.createSamplerYcbcrConversion(conv_info); // link error
        CALL_VK(vkCreateSamplerYcbcrConversion(device, reinterpret_cast<VkSamplerYcbcrConversionCreateInfo *>(&conversionCreateInfo), nullptr,
                                               reinterpret_cast<VkSamplerYcbcrConversion *>(&mConversion)));

        vk::SamplerCreateInfo samplerCreateInfo;
        vk::SamplerYcbcrConversionInfo conversionInfo;
        conversionInfo.conversion = mConversion;

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

    VulkanAndroidHardwareSampler::~VulkanAndroidHardwareSampler() {
        vk::Device device = mVulkanDevice.getDevice();

        device.destroySampler(mSampler);
        // device.destroySamplerYcbcrConversion(mConversion); // link error
        vkDestroySamplerYcbcrConversion(device, mConversion, nullptr);
    }

    vk::SamplerYcbcrConversion VulkanAndroidHardwareSampler::getConversion() const {
        return mConversion;
    }

    vk::Sampler VulkanAndroidHardwareSampler::getSampler() const {
        return mSampler;
    }

    std::vector<vk::DescriptorImageInfo> VulkanAndroidHardwareSampler::createDescriptorImageInfos(){
        vk::DescriptorImageInfo samplerDescriptorImageInfo;
                        samplerDescriptorImageInfo
                                .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                                .setImageView(pVulkanHardwareBuffer->getImageView())
                                .setSampler(pVulkanHardwareBuffer->getSampler());

                        std::array<vk::DescriptorImageInfo, 1> samplerDescriptorImageInfos = {samplerDescriptorImageInfo};

    }

} // engine