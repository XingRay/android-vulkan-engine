//
// Created by leixing on 2025/3/10.
//

#pragma once

#include <cstdint>
#include <vector>

namespace engine {

    class VulkanPushConstants {
    private:
        std::vector<uint8_t> mPushConstants;

    public:
        explicit VulkanPushConstants(uint32_t size);

        ~VulkanPushConstants();

        void update(const void *data, uint32_t size);
    };

} // engine
