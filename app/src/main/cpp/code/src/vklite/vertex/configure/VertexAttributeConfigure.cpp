//
// Created by leixing on 2025/3/11.
//
#include "VertexAttributeConfigure.h"

namespace vklite {

    VertexAttributeConfigure::VertexAttributeConfigure(vk::Format format, uint32_t offset)
            : mFormat(format), mOffset(offset) {}

    VertexAttributeConfigure::~VertexAttributeConfigure() = default;

    vk::Format VertexAttributeConfigure::getFormat() const {
        return mFormat;
    }

    uint32_t VertexAttributeConfigure::getOffset() const {
        return mOffset;
    }
}