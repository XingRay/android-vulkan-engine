//
// Created by leixing on 2024/12/23.
//

#include "VulkanDevice.h"
#include "Log.h"
#include <set>
#include <map>
#include "VulkanUtil.h"


namespace engine {

    VulkanDevice::VulkanDevice(const vk::Instance &instance,
                               const vk::SurfaceKHR &surface,
                               const std::vector<const char *> &layers,
                               const std::vector<const char *> &deviceExtensions) {
        LOG_D("pickPhysicalDevice");
        auto devices = instance.enumeratePhysicalDevices();
        for (const auto &device: devices) {
            vk::PhysicalDeviceProperties properties = device.getProperties();
            LOG_D("\tdeviceName: %s\tdeviceType: %s", properties.deviceName.data(), to_string(properties.deviceType).data());
        }

        // 计算每张卡的分数, 取最高分
        std::unique_ptr<PhysicalDeviceCandidate> candidate = nullptr;
        std::multimap<int32_t, std::unique_ptr<PhysicalDeviceCandidate>> candidates;
        for (const auto &device: devices) {
            candidates.insert(calcDeviceSuitable(device, deviceExtensions, surface));
        }
        if (candidates.rbegin()->first > 0) {
            candidate = std::move(candidates.rbegin()->second);
        } else {
            LOG_D("failed to find GPUs with vulkan support !");
            throw std::runtime_error("failed to find GPUs with vulkan support !");
        }

        mPhysicalDevice = candidate->physicalDevice;
        VulkanUtil::printPhysicalDeviceInfo(mPhysicalDevice);
        VulkanUtil::printPhysicalDeviceInfoWithSurface(mPhysicalDevice, surface);

        mMsaaSamples = getMaxUsableSampleCount(mPhysicalDevice);
        mGraphicQueueFamilyIndex = candidate->graphicQueueFamilyIndex.value();
        mPresentQueueFamilyIndex = candidate->presentQueueFamilyIndex.value();
        LOG_D("graphicFamilyIndex:%d, presentFamilyIndex:%d", mGraphicQueueFamilyIndex, mPresentQueueFamilyIndex);

        mQueueFamilyIndices.push_back(mGraphicQueueFamilyIndex);
        if (mGraphicQueueFamilyIndex != mPresentQueueFamilyIndex) {
            mQueueFamilyIndices.push_back(mPresentQueueFamilyIndex);
        }

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        std::array<float, 1> queuePriorities = {1.0f};
        for (uint32_t queueFamilyIndex: mQueueFamilyIndices) {
            vk::DeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.setQueueFamilyIndex(queueFamilyIndex)
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

        vk::DeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo
                .setQueueCreateInfos(queueCreateInfos)
                .setPEnabledFeatures(&deviceFeatures)
                .setPEnabledExtensionNames(deviceExtensions)
                .setPEnabledLayerNames(layers);

        mDevice = mPhysicalDevice.createDevice(deviceCreateInfo);

        mGraphicsQueue = mDevice.getQueue(mGraphicQueueFamilyIndex, 0);
        mPresentQueue = mDevice.getQueue(mPresentQueueFamilyIndex, 0);

        mCapabilities = mPhysicalDevice.getSurfaceCapabilitiesKHR(surface);
        mFormats = mPhysicalDevice.getSurfaceFormatsKHR(surface);
        mPresentModes = mPhysicalDevice.getSurfacePresentModesKHR(surface);
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

    vk::Device VulkanDevice::getDevice() const {
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

    vk::Queue VulkanDevice::getGraphicsQueue() const {
        return mGraphicsQueue;
    }

    vk::Queue VulkanDevice::getPresentQueue() const {
        return mPresentQueue;
    }

    [[nodiscard]]
    vk::SurfaceCapabilitiesKHR VulkanDevice::getCapabilities() const {
        return mCapabilities;
    }

    [[nodiscard]]
    std::vector<vk::SurfaceFormatKHR> VulkanDevice::getFormats() const {
        return mFormats;
    }

    [[nodiscard]]
    std::vector<vk::PresentModeKHR> VulkanDevice::getPresentModes() const {
        return mPresentModes;
    }

    std::pair<int32_t, std::unique_ptr<PhysicalDeviceCandidate>> VulkanDevice::calcDeviceSuitable(
            const vk::PhysicalDevice &device,
            const std::vector<const char *> &requiredDeviceExtensions,
            const vk::SurfaceKHR &surface) {
        int32_t score = 0;

        vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
        vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();

        QueueFamilyIndices indices = findQueueFamilies(device, surface);
        if (!indices.isComplete()) {
            LOG_D("device QueueFamilyIndices is not complete !");
            return std::make_pair(score, nullptr);
        }

        if (!isDeviceSupportedRequiredDeviceExtensions(device, requiredDeviceExtensions)) {
            LOG_D("isDeviceSupportedRequiredDeviceExtensions: false");
            return std::make_pair(score, nullptr);
        }

        // 验证扩展可用后才尝试查询交换链支持
        SwapChainSupportDetail swapChainSupportedDetail = querySwapChainSupported(device, surface);
        if (swapChainSupportedDetail.formats.empty() || swapChainSupportedDetail.presentModes.empty()) {
            LOG_D("swapChainSupportedDetail: formats or presentModes is empty");
            return std::make_pair(score, nullptr);
        }

        if (!deviceFeatures.geometryShader || !deviceFeatures.samplerAnisotropy) {
            return std::make_pair(score, nullptr);
        }

        switch (deviceProperties.deviceType) {
            case vk::PhysicalDeviceType::eIntegratedGpu:
                // 核显
                score += 10;
                break;

            case vk::PhysicalDeviceType::eDiscreteGpu:
                // 独显
                score += 20;
                break;

            case vk::PhysicalDeviceType::eVirtualGpu:
                // 虚拟GPU
                score += 15; // 虚拟GPU在性能上可能低于独显，但高于CPU
                break;

            case vk::PhysicalDeviceType::eCpu:
                // CPU
                score += 5;  // CPU的性能通常较低，分数也低
                break;

            case vk::PhysicalDeviceType::eOther:
            default:
                // 其他类型的设备
                score += 0;  // 对于未知类型或其他类型，默认不加分
                break;
        }

        std::unique_ptr<PhysicalDeviceCandidate> candidate = std::make_unique<PhysicalDeviceCandidate>(device, indices.graphicQueueFamilyIndex, indices.presentQueueFamilyIndex,
                                                                                                       swapChainSupportedDetail);
        return std::make_pair(score, std::move(candidate));
    }

    QueueFamilyIndices VulkanDevice::findQueueFamilies(const vk::PhysicalDevice &device, const vk::SurfaceKHR &surface) {
        QueueFamilyIndices indices;
        auto queueFamilyProperties = device.getQueueFamilyProperties();

        for (int i = 0; i < queueFamilyProperties.size(); i++) {
            const auto &queueFamilyProperty = queueFamilyProperties[i];
            const vk::QueueFlags &queueFlags = queueFamilyProperty.queueFlags;

            if (queueFlags & vk::QueueFlagBits::eGraphics) {
                LOG_D("graphicQueueFamily found, index:%d", i);
                indices.graphicQueueFamilyIndex = i;
            }

            if (device.getSurfaceSupportKHR(i, surface)) {
                LOG_D("presentQueueFamily found, index:%d", i);
                indices.presentQueueFamilyIndex = i;
            }

            if (indices.isComplete()) {
                break;
            }
        }

        return indices;
    }

    vk::SampleCountFlagBits VulkanDevice::getMaxUsableSampleCount(const vk::PhysicalDevice &device) {
        vk::PhysicalDeviceProperties properties = device.getProperties();
        vk::PhysicalDeviceLimits &limits = properties.limits;

        vk::SampleCountFlags counts = limits.framebufferColorSampleCounts & limits.framebufferDepthSampleCounts;
        if (counts & vk::SampleCountFlagBits::e64) {
            return vk::SampleCountFlagBits::e64;
        } else if (counts & vk::SampleCountFlagBits::e32) {
            return vk::SampleCountFlagBits::e32;
        } else if (counts & vk::SampleCountFlagBits::e16) {
            return vk::SampleCountFlagBits::e16;
        } else if (counts & vk::SampleCountFlagBits::e8) {
            return vk::SampleCountFlagBits::e8;
        } else if (counts & vk::SampleCountFlagBits::e4) {
            return vk::SampleCountFlagBits::e4;
        } else if (counts & vk::SampleCountFlagBits::e2) {
            return vk::SampleCountFlagBits::e2;
        } else {
            return vk::SampleCountFlagBits::e1;
        }
    }

    bool VulkanDevice::isDeviceSupportedRequiredDeviceExtensions(const vk::PhysicalDevice &device,
                                                                 const std::vector<const char *> &requiredDeviceExtensions) {
        LOG_D("requiredExtensions");
        for (const auto &extension: requiredDeviceExtensions) {
            LOG_D("\t:%s", extension);
        }

        std::set<std::string> requiredExtensionSet(requiredDeviceExtensions.begin(), requiredDeviceExtensions.end());

        std::vector<vk::ExtensionProperties> properties = device.enumerateDeviceExtensionProperties();
//        LOG_D("device.enumerateDeviceExtensionProperties():");
        for (const auto &property: properties) {
//            LOG_D("\tproperty.extensionName: %s", property.extensionName.data());
            requiredExtensionSet.erase(property.extensionName);
        }
        return requiredExtensionSet.empty();
    }

    SwapChainSupportDetail VulkanDevice::querySwapChainSupported(const vk::PhysicalDevice &device, const vk::SurfaceKHR &surface) {
        SwapChainSupportDetail detail;

        detail.capabilities = device.getSurfaceCapabilitiesKHR(surface);
        detail.formats = device.getSurfaceFormatsKHR(surface);
        detail.presentModes = device.getSurfacePresentModesKHR(surface);

        return detail;
    }

    vk::ShaderModule VulkanDevice::createShaderModule(const std::vector<char> &code) const {
        vk::ShaderModuleCreateInfo createInfo;
        createInfo
                .setCodeSize(code.size())
                .setPCode(reinterpret_cast<const uint32_t *>(code.data()));

        return mDevice.createShaderModule(createInfo);
    }

    bool QueueFamilyIndices::isComplete() {
        return graphicQueueFamilyIndex.has_value() && presentQueueFamilyIndex.has_value();
    }
} // engine