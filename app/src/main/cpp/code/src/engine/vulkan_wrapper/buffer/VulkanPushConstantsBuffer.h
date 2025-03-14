//
// Created by leixing on 2025/3/10.
//

#pragma once

#include <cstdint>
#include <vector>

namespace engine {

    class VulkanPushConstantsBuffer {
    private:
        std::vector<uint8_t> mPushConstants;

    public:
        explicit VulkanPushConstantsBuffer(uint32_t size);

        ~VulkanPushConstantsBuffer();

        void update(void *data, uint32_t size);
    };

} // engine
