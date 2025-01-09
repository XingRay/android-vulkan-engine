//
// Created by leixing on 2024/12/22.
//

#include "AndroidVulkanSurface.h"
#include "Log.h"

namespace engine {
    AndroidVulkanSurface::AndroidVulkanSurface(const vk::Instance& instance, ANativeWindow *window) : mInstance(instance) {
        LOG_D("AndroidSurface::AndroidSurface");
        vk::AndroidSurfaceCreateInfoKHR createInfo = vk::AndroidSurfaceCreateInfoKHR{}
                .setFlags(vk::AndroidSurfaceCreateFlagsKHR{})
                .setWindow(window);

        try {
            mSurface = instance.createAndroidSurfaceKHR(createInfo);
        } catch (vk::SystemError &err) {
            LOG_E("Failed to create Android surface: %s", std::string(err.what()).data());
            throw std::runtime_error("Failed to create Android surface: " + std::string(err.what()));
        }
    }

    AndroidVulkanSurface::~AndroidVulkanSurface() {
        LOG_D("AndroidSurface::~AndroidSurface()");
        if (mSurface != nullptr) {
            mInstance.destroy(mSurface);
        } else {
            LOG_W("surface is null");
        }
    }

} // engine