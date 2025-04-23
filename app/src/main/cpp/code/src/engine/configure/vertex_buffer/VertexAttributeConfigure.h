//
// Created by leixing on 2025/1/14.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include <stdint.h>

namespace engine {

    class VertexAttributeConfigure {
    private:
        uint32_t mBinding;
        uint32_t mLocation;
        vk::Format mFormat;
        uint32_t mOffset;

    public:
        VertexAttributeConfigure(uint32_t binding, uint32_t location, vk::Format format, uint32_t offset);

        ~VertexAttributeConfigure();

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getLocation() const;

        [[nodiscard]]
        vk::Format getFormat() const;

        [[nodiscard]]
        uint32_t getOffset() const;
    };


} // engine
