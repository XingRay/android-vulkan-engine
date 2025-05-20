//
// Created by leixing on 2025/1/9.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "vklite/instance/VulkanInstance.h"
#include "vklite/VulkanSurface.h"
#include "vklite/VulkanPhysicalDevice.h"
#include "VulkanPhysicalDeviceCandidate.h"

#include "VulkanPhysicalDeviceScoreConfig.h"
#include "VulkanPhysicalDeviceScoreCalculator.h"

namespace vklite {

    class VulkanPhysicalDeviceProvider {

    public:
        virtual ~VulkanPhysicalDeviceProvider() = default;

        [[nodiscard]]
        virtual std::unique_ptr<VulkanPhysicalDeviceCandidate> provide(
                const VulkanInstance &instance,
                const VulkanSurface &surface,
                const std::vector<std::string> &deviceExtensions) = 0;
    };


    class SimpleVulkanPhysicalDeviceProvider : public VulkanPhysicalDeviceProvider {
    private:
        std::unique_ptr<VulkanPhysicalDevice> mVulkanPhysicalDevice;
        vk::QueueFlags mQueueFlags = vk::QueueFlagBits::eGraphics;

    public:

        explicit SimpleVulkanPhysicalDeviceProvider(std::unique_ptr<VulkanPhysicalDevice> &&physicalDevice);

        ~SimpleVulkanPhysicalDeviceProvider() override;

        [[nodiscard]]
        std::unique_ptr<VulkanPhysicalDeviceCandidate> provide(const VulkanInstance &instance,
                                                               const VulkanSurface &surface,
                                                               const std::vector<std::string> &deviceExtensions) override;
    };


    class FixedVulkanPhysicalDeviceProvider : public VulkanPhysicalDeviceProvider {
    private:
        std::unique_ptr<VulkanPhysicalDeviceCandidate> mCandidate;

    public:

        explicit FixedVulkanPhysicalDeviceProvider(std::unique_ptr<VulkanPhysicalDeviceCandidate> &&candidate);

        ~FixedVulkanPhysicalDeviceProvider() override;

        [[nodiscard]]
        std::unique_ptr<VulkanPhysicalDeviceCandidate> provide(const VulkanInstance &instance,
                                                               const VulkanSurface &surface,
                                                               const std::vector<std::string> &deviceExtensions) override;
    };

    class DefaultVulkanPhysicalDeviceProvider : public VulkanPhysicalDeviceProvider {
    private:
        VulkanPhysicalDeviceScoreCalculator mScoreCalculator;
        vk::QueueFlags mRequiredQueueFlags;

    public:
        DefaultVulkanPhysicalDeviceProvider(vk::QueueFlags requiredQueueFlags = vk::QueueFlagBits::eGraphics,
                                            VulkanPhysicalDeviceScoreConfig scoreConfig = VulkanPhysicalDeviceScoreConfig());

        ~DefaultVulkanPhysicalDeviceProvider() override;

        [[nodiscard]]
        std::unique_ptr<VulkanPhysicalDeviceCandidate> provide(const VulkanInstance &instance,
                                                               const VulkanSurface &surface,
                                                               const std::vector<std::string> &deviceExtensions) override;

    private:
        [[nodiscard]]
        std::optional<std::unique_ptr<VulkanPhysicalDeviceCandidate>> calcPhysicalDeviceCandidate(
                std::unique_ptr<VulkanPhysicalDevice> &&vulkanPhysicalDevice,
                const VulkanSurface &vulkanSurface,
                const std::vector<std::string> &requiredDeviceExtensions,
                vk::QueueFlags requiredQueueFlags) const;
    };

} // engine
