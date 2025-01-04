//
// Created by leixing on 2024/12/22.
//

#ifndef VULKANDEMO_ANDROIDVULKANSURFACE_H
#define VULKANDEMO_ANDROIDVULKANSURFACE_H

#include <vulkan/vulkan.hpp>
#include "VulkanSurface.h"

namespace engine {

    class AndroidVulkanSurface : public VulkanSurface {

    private:
        vk::Instance mInstance;
        vk::SurfaceKHR mSurface;

    public:
        AndroidVulkanSurface(const vk::Instance& instance, ANativeWindow *window);

        ~AndroidVulkanSurface() override;

        [[nodiscard]]
        vk::SurfaceKHR getSurface() const override { return mSurface; }
    };

} // engine

#endif //VULKANDEMO_ANDROIDVULKANSURFACE_H
