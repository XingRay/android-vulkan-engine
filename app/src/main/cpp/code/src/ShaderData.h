//
// Created by leixing on 2024/12/29.
//

#ifndef VULKANDEMO_SHADERDATA_H
#define VULKANDEMO_SHADERDATA_H

#include "glm.h"
#include "vulkan/vulkan.hpp"

namespace app {
    struct Vertex {
        glm::vec3 pos; // 对应顶点位置属性 inPosition

        // 返回顶点输入绑定和属性描述，供 Vulkan Pipeline 使用
        static vk::VertexInputBindingDescription getBindingDescription() {
            vk::VertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0; // 顶点绑定点
            bindingDescription.stride = sizeof(Vertex); // 每个顶点的步长
            bindingDescription.inputRate = vk::VertexInputRate::eVertex; // 按顶点输入
            return bindingDescription;
        }

        static vk::VertexInputAttributeDescription getAttributeDescription() {
            // 位置属性
            vk::VertexInputAttributeDescription attributeDescription{};
            attributeDescription.binding = 0; // 绑定点 0
            attributeDescription.location = 0; // 对应顶点着色器的 layout(location = 0)
            attributeDescription.format = vk::Format::eR32G32B32Sfloat; // 位置为 vec3
            attributeDescription.offset = offsetof(Vertex, pos); // 偏移量为顶点结构体中位置的偏移

            return attributeDescription;
        }
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
