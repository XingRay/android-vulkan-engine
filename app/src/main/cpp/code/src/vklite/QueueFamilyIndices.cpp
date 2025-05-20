//
// Created by leixing on 2025/1/9.
//

#include "vklite/QueueFamilyIndices.h"

namespace vklite {

    bool QueueFamilyIndices::isComplete() {
        return graphicQueueFamilyIndex.has_value() && presentQueueFamilyIndex.has_value();
    }

} // engine