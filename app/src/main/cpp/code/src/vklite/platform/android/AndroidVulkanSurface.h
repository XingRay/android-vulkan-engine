//
// Created by leixing on 2024/12/22.
//
#ifdef ANDROID

#pragma once

#include "android/native_window.h"
#include <vulkan/vulkan.hpp>

#include "vklite/VulkanSurface.h"
#include "vklite/instance/VulkanInstance.h"
#include "vklite/VulkanSurfaceBuilder.h"

namespace vklite {

    class AndroidVulkanSurface : public VulkanSurface {

    private:
        VulkanInstance mInstance;
        vk::SurfaceKHR mSurface;

    public:
        AndroidVulkanSurface(const VulkanInstance &instance, ANativeWindow *window);

        ~AndroidVulkanSurface() override;

        [[nodiscard]]
        vk::SurfaceKHR getSurface() const override { return mSurface; }
    };

    class AndroidVulkanSurfaceBuilder : public VulkanSurfaceBuilder {
    private:
        ANativeWindow *mWindow;

    public:
        AndroidVulkanSurfaceBuilder(ANativeWindow *window);

        ~AndroidVulkanSurfaceBuilder() override;

        [[nodiscard]]
        std::unique_ptr<VulkanSurface> build(const VulkanInstance &) const override;
    };

} // engine

#endif