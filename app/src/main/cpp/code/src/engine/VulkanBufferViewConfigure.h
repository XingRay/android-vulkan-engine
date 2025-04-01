//
// Created by leixing on 2025/3/31.
//

#pragma once

#include <memory>
#include <cstdint>
#include <vector>

#include "engine/vulkan_wrapper/VulkanBufferView.h"
#include "engine/VulkanBufferViewCreateInfo.h"

namespace engine {

    class VulkanBufferViewConfigure {
    private:
        // set bufferView
        std::shared_ptr<VulkanBufferView> mBufferView;

        // create bufferView
        std::unique_ptr<VulkanBufferViewCreateInfo> mBufferViewCreateInfo;

    public:

        VulkanBufferViewConfigure(const std::shared_ptr<VulkanBufferView>& bufferView);

        VulkanBufferViewConfigure(uint32_t capacity, const void* data, uint32_t size);

        VulkanBufferViewConfigure(uint32_t capacity);

        template<class T>
        VulkanBufferViewConfigure(uint32_t capacity, std::vector<T> data);

        template<class T>
        VulkanBufferViewConfigure(std::vector<T> data);

        ~VulkanBufferViewConfigure();

    };

} // engine
