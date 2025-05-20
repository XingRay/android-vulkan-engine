//
// Created by leixing on 2025/1/14.
//

#pragma once

#include <stdint.h>

#include "vulkan/vulkan.hpp"


namespace vklite {

    class VertexAttributeConfigure {
    private:
        vk::Format mFormat;
        uint32_t mOffset;

    public:
        VertexAttributeConfigure(vk::Format format, uint32_t offset);

        ~VertexAttributeConfigure();

        [[nodiscard]]
        vk::Format getFormat() const;

        [[nodiscard]]
        uint32_t getOffset() const;
    };


} // engine
