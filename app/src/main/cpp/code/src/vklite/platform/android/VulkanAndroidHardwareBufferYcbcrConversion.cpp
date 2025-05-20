//
// Created by leixing on 2025/3/15.
//

#include "VulkanAndroidHardwareBufferYcbcrConversion.h"
#include "vklite/VkCheck.h"

namespace vklite {

    VulkanAndroidHardwareBufferYcbcrConversion::VulkanAndroidHardwareBufferYcbcrConversion(const VulkanDevice &vulkanDevice,
                                                                                           const vk::AndroidHardwareBufferFormatPropertiesANDROID &formatInfo)
            : mVulkanDevice(vulkanDevice) {

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

        CALL_VK(vkCreateSamplerYcbcrConversion(vulkanDevice.getDevice(), reinterpret_cast<VkSamplerYcbcrConversionCreateInfo *>(&conversionCreateInfo), nullptr,
                                               reinterpret_cast<VkSamplerYcbcrConversion *>(&mSamplerYcbcrConversion)));

    }

    VulkanAndroidHardwareBufferYcbcrConversion::~VulkanAndroidHardwareBufferYcbcrConversion() {
        const vk::Device &device = mVulkanDevice.getDevice();

        // device.destroySamplerYcbcrConversion(mConversion); // link error
        vkDestroySamplerYcbcrConversion(device, mSamplerYcbcrConversion, nullptr);
    }

    const vk::SamplerYcbcrConversion &VulkanAndroidHardwareBufferYcbcrConversion::getSamplerYcbcrConversion() const {
        return mSamplerYcbcrConversion;
    }
} // engine