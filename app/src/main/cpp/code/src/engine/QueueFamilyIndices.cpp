//
// Created by leixing on 2025/1/9.
//

#include "engine/QueueFamilyIndices.h"

namespace engine {

    bool QueueFamilyIndices::isComplete() {
        return graphicQueueFamilyIndex.has_value() && presentQueueFamilyIndex.has_value();
    }

} // engine