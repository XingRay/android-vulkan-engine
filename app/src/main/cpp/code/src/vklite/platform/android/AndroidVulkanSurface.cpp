//
// Created by leixing on 2024/12/22.
//
#ifdef ANDROID

#include "AndroidVulkanSurface.h"
#include "vklite/Log.h"

namespace vklite {
    AndroidVulkanSurface::AndroidVulkanSurface(const VulkanInstance &instance, ANativeWindow *window) : mInstance(instance) {
        LOG_D("AndroidSurface::AndroidSurface");
        vk::AndroidSurfaceCreateInfoKHR createInfo{};
        createInfo
                .setFlags(vk::AndroidSurfaceCreateFlagsKHR{})
                .setWindow(window);

        try {
            mSurface = instance.getInstance().createAndroidSurfaceKHR(createInfo);
        } catch (vk::SystemError &err) {
            LOG_E("Failed to create Android surface: %s", std::string(err.what()).data());
            throw std::runtime_error("Failed to create Android surface: " + std::string(err.what()));
        }
    }

    AndroidVulkanSurface::~AndroidVulkanSurface() {
        LOG_D("AndroidSurface::~AndroidSurface()");
        if (mSurface != nullptr) {
            mInstance.getInstance().destroy(mSurface);
        } else {
            LOG_W("surface is null");
        }
    }

    AndroidVulkanSurfaceBuilder::AndroidVulkanSurfaceBuilder(ANativeWindow *window)
            : mWindow(window) {

    }

    AndroidVulkanSurfaceBuilder::~AndroidVulkanSurfaceBuilder() = default;

    [[nodiscard]]
    std::unique_ptr<VulkanSurface> AndroidVulkanSurfaceBuilder::build(const VulkanInstance & vulkanInstance) const{
        return std::make_unique<AndroidVulkanSurface>(vulkanInstance, mWindow);
    }

} // engine

#endif