//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>

namespace engine {

    enum VulkanBufferType {
        NORMAL,
        TEXTURE_SAMPLER,
    };

    class VulkanBuffer {
    private:
        VulkanBufferType mType;

        uint32_t mBinding = 0;
        uint32_t mIndex = 0;

    public:
        explicit VulkanBuffer(VulkanBufferType type);

        virtual  ~VulkanBuffer();

        [[nodiscard]]
        VulkanBufferType getType() const;

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getIndex() const;

        virtual void updateBuffer(void *data, uint32_t size) = 0;
    };

} // engine
