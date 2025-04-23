//
// Created by leixing on 2025/3/11.
//
#include "VertexAttributeConfigure.h"

namespace engine {

    VertexAttributeConfigure::VertexAttributeConfigure(uint32_t binding, uint32_t location, vk::Format format, uint32_t offset)
            : mBinding(binding), mLocation(location), mFormat(format), mOffset(offset) {

    }

    VertexAttributeConfigure::~VertexAttributeConfigure() = default;

    uint32_t VertexAttributeConfigure::getBinding() const {
        return mBinding;
    }

    uint32_t VertexAttributeConfigure::getLocation() const {
        return mLocation;
    }

    vk::Format VertexAttributeConfigure::getFormat() const {
        return mFormat;
    }

    uint32_t VertexAttributeConfigure::getOffset() const {
        return mOffset;
    }
}