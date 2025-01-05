//
// Created by leixing on 2024/12/29.
//

#ifndef VULKANDEMO_SHADERDATA_H
#define VULKANDEMO_SHADERDATA_H

#include "glm.h"
#include "vulkan/vulkan.hpp"

namespace app {
    struct Vertex {
        glm::vec3 pos;
    };

    struct TransformUniformBufferObject {
        glm::mat4 model; // 模型矩阵
        glm::mat4 view;  // 视图矩阵
        glm::mat4 proj;  // 投影矩阵
    };

    struct ColorUniformBufferObject {
        alignas(16) glm::vec3 color; // 颜色数据
    };

}

#endif //VULKANDEMO_SHADERDATA_H
