//
// Created by leixing on 2024/12/23.
//

#include <map>

#include "VulkanDevice.h"
#include "vklite/Log.h"
#include "vklite/VulkanUtil.h"

#include "vklite/common/StringUtil.h"

namespace vklite {

    VulkanDevice::VulkanDevice(const VulkanPhysicalDevice &physicalDevice,
                               const VulkanPhysicalDeviceSurfaceSupport &surfaceSupport,
                               const std::vector<std::string> &deviceExtensions,
                               const std::vector<std::string> &layers,
                               uint32_t sampleCount)
            : mPhysicalDevice(physicalDevice.getPhysicalDevice()) {

        mMsaaSamples = VulkanUtil::uint32ToSampleCountFlagBits(sampleCount);

        mGraphicQueueFamilyIndex = surfaceSupport.graphicQueueFamilyIndex;
        mPresentQueueFamilyIndex = surfaceSupport.presentQueueFamilyIndex;
        LOG_D("graphicFamilyIndex:%d, presentFamilyIndex:%d", mGraphicQueueFamilyIndex, mPresentQueueFamilyIndex);

        mQueueFamilyIndices.push_back(mGraphicQueueFamilyIndex);
        if (mGraphicQueueFamilyIndex != mPresentQueueFamilyIndex) {
            mQueueFamilyIndices.push_back(mPresentQueueFamilyIndex);
        }

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        std::array<float, 1> queuePriorities = {1.0f};
        for (uint32_t queueFamilyIndex: mQueueFamilyIndices) {
            vk::DeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo
                    .setQueueFamilyIndex(queueFamilyIndex)
                    .setQueueCount(1)
                    .setQueuePriorities(queuePriorities);
            queueCreateInfos.push_back(queueCreateInfo);
        }

        vk::PhysicalDeviceFeatures supportedFeatures = mPhysicalDevice.getFeatures();
        vk::PhysicalDeviceFeatures deviceFeatures{};

        if (supportedFeatures.samplerAnisotropy) {
            deviceFeatures.setSamplerAnisotropy(vk::True);
        }
        if (supportedFeatures.sampleRateShading) {
            deviceFeatures.setSampleRateShading(vk::True);
        }

        std::vector<const char *> enabledDeviceExtensionNames = common::StringUtil::toStringPtrArray(deviceExtensions);
        LOG_D("enabled device extension names:[%ld]", enabledDeviceExtensionNames.size());
        for (const char *name: enabledDeviceExtensionNames) {
            LOG_D("    %s", name);
        }

        std::vector<const char *> layerNames = common::StringUtil::toStringPtrArray(layers);

        vk::PhysicalDeviceFeatures2 physicalDeviceFeatures2;
        vk::PhysicalDeviceSamplerYcbcrConversionFeatures ycbcr_features;
        physicalDeviceFeatures2.pNext = &ycbcr_features;

        ycbcr_features.samplerYcbcrConversion = true;
        physicalDeviceFeatures2.features.samplerAnisotropy = true;

        vk::DeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo
                .setFlags(vk::DeviceCreateFlags{})
                .setQueueCreateInfos(queueCreateInfos)
                .setPEnabledFeatures(&deviceFeatures)
                .setPEnabledExtensionNames(enabledDeviceExtensionNames)
                .setPEnabledLayerNames(layerNames)
                .setPNext(&physicalDeviceFeatures2);

        mDevice = mPhysicalDevice.createDevice(deviceCreateInfo);

        mGraphicsQueue = mDevice.getQueue(mGraphicQueueFamilyIndex, 0);
        mPresentQueue = mDevice.getQueue(mPresentQueueFamilyIndex, 0);

        mCapabilities = surfaceSupport.capabilities;
        mFormats = surfaceSupport.formats;
        mPresentModes = surfaceSupport.presentModes;
    }

    VulkanDevice::~VulkanDevice() {
        LOG_D("VulkanDevice::~VulkanDevice");
        if (mDevice != nullptr) {
            mDevice.destroy();
        } else {
            LOG_W("mDevice is null");
        }
    }

    vk::PhysicalDevice VulkanDevice::getPhysicalDevice() const {
        return mPhysicalDevice;
    }

    const vk::Device &VulkanDevice::getDevice() const {
        return mDevice;
    }

    vk::SampleCountFlagBits VulkanDevice::getMsaaSamples() const {
        return mMsaaSamples;
    }

    uint32_t VulkanDevice::getGraphicQueueFamilyIndex() const {
        return mGraphicQueueFamilyIndex;
    }

    uint32_t VulkanDevice::getPresentQueueFamilyIndex() const {
        return mPresentQueueFamilyIndex;
    }

    const std::vector<uint32_t> &VulkanDevice::getQueueFamilyIndices() const {
        return mQueueFamilyIndices;
    }

    const vk::Queue &VulkanDevice::getGraphicsQueue() const {
        return mGraphicsQueue;
    }

    const vk::Queue &VulkanDevice::getPresentQueue() const {
        return mPresentQueue;
    }

    vk::SurfaceCapabilitiesKHR VulkanDevice::getCapabilities() const {
        return mCapabilities;
    }

    std::vector<vk::SurfaceFormatKHR> VulkanDevice::getFormats() const {
        return mFormats;
    }

    std::vector<vk::PresentModeKHR> VulkanDevice::getPresentModes() const {
        return mPresentModes;
    }

    uint32_t VulkanDevice::getMaxPushConstantsSize() const {
        vk::PhysicalDeviceProperties deviceProperties = mPhysicalDevice.getProperties();
        return deviceProperties.limits.maxPushConstantsSize;
    }

    float VulkanDevice::getMaxSamplerAnisotropy() const {
        vk::PhysicalDeviceProperties properties = mPhysicalDevice.getProperties();
        return properties.limits.maxSamplerAnisotropy;
    }
} // engine