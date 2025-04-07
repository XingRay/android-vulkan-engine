//
// Created by leixing on 2025/3/30.
//

#pragma once

#include <cstdint>

#include "vulkan/vulkan.hpp"

namespace engine {

    class VulkanBufferInterface {
    private:

    public:
        VulkanBufferInterface();

        virtual ~VulkanBufferInterface() = 0;

        [[nodiscard]]
        virtual vk::DeviceSize getSize() const = 0;

        [[nodiscard]]
        virtual const vk::Buffer &getBuffer() const = 0;
    };

} // engine