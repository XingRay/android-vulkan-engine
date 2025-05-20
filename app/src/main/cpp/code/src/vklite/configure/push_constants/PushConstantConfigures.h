//
// Created by leixing on 2025/3/11.
//

#pragma once

#include <memory>
#include <vector>

#include "PushConstantConfigure.h"

namespace vklite {

    class PushConstantConfigures {
    private:
        std::vector<PushConstantConfigure> mPushConstantConfigures;

    public:
        PushConstantConfigures();

        ~PushConstantConfigures();

        PushConstantConfigures &addPushConstantConfigures(PushConstantConfigure pushConstantConfigure);

        std::vector<vk::PushConstantRange> createPushConstantRanges() const;

        uint32_t calcTotalPushConstantSize() const;
    };

} // engine
