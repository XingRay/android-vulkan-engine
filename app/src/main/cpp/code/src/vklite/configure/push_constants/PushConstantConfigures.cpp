//
// Created by leixing on 2025/3/11.
//

#include "PushConstantConfigures.h"

namespace vklite {

    PushConstantConfigures::PushConstantConfigures() {

    }

    PushConstantConfigures::~PushConstantConfigures() {

    }

    PushConstantConfigures &PushConstantConfigures::addPushConstantConfigures(PushConstantConfigure pushConstantConfigure) {
        mPushConstantConfigures.push_back(pushConstantConfigure);
        return *this;
    }

    std::vector<vk::PushConstantRange> PushConstantConfigures::createPushConstantRanges() const {
        // push constant
        std::vector<vk::PushConstantRange> pushConstantRanges;
        for (const PushConstantConfigure &pushConstantConfigure: mPushConstantConfigures) {
            vk::PushConstantRange pushConstantRange = pushConstantConfigure.createPushConstantRange();
            pushConstantRanges.push_back(pushConstantRange);
        }

        return pushConstantRanges;
    }

    uint32_t PushConstantConfigures::calcTotalPushConstantSize() const {
        uint32_t totalPushConstantSize = 0;
        for (const PushConstantConfigure &pushConstantConfigure: mPushConstantConfigures) {
            totalPushConstantSize += pushConstantConfigure.getSize();
        }

        return totalPushConstantSize;
    }
} // engine