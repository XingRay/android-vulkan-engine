//
// Created by leixing on 2025/1/4.
//

#include "Test06Load3dModel.h"
#include "FileUtil.h"

#include "vklite/engine/configure/VkLiteEngineBuilder.h"
#include "image/StbImage.h"

#include <tiny_obj_loader.h>

namespace std {
    template<>
    struct hash<test06::Vertex> {
        size_t operator()(test06::Vertex const &vertex) const {
            size_t seed = 0;

            // 哈希 position
            hash<glm::vec3> vec3Hash;
            seed ^= vec3Hash(vertex.position) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            // 哈希 uv
            hash<glm::vec2> vec2Hash;
            seed ^= vec2Hash(vertex.uv) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
}


namespace test06 {

    const char *MODEL_PATH = "/storage/emulated/0/test/model/viking_room/viking_room.obj";
    const char *TEXTURE_PATH = "/storage/emulated/0/test/model/viking_room/viking_room.png";

    Test06Load3dModel::Test06Load3dModel(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app), mMvpMatrix(glm::mat4(1.0f)) {

        std::vector<std::string> instanceExtensions = {
                VK_KHR_SURFACE_EXTENSION_NAME,
                VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,

                // old version
                VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
                // new version
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        };

        std::vector<std::string> layers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::vector<std::string> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        std::vector<char> vertexShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/06_load_3d_model.vert.spv");
        std::vector<char> fragmentShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/06_load_3d_model.frag.spv");

        // load texture
        std::unique_ptr<image::StbImage> image = image::StbImage::loadImageAsRgb(TEXTURE_PATH);

        // load vertex
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warning;
        std::string error;

        bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, MODEL_PATH);
        if (!success) {
            throw std::runtime_error(warning + error);
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for (const auto &shape: shapes) {
            for (const auto &index: shape.mesh.indices) {
                Vertex vertex{};

                vertex.position = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.uv = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1],
                };

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = vertices.size();
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }


        mMvpMatrix = MvpMatrix{};
        float scale = 1.0f;

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.model = model;
        mMvpMatrix.view = glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f),
                                      glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.proj = glm::perspective(glm::radians(45.0f), (float) ANativeWindow_getWidth(mApp.window) / (float) ANativeWindow_getHeight(mApp.window), 0.1f, 10.0f);
        // GLM 最初为 OpenGL 设计，OpenGL 中剪裁坐标的Y坐标是反转的。补偿这种情况的最简单方法是翻转投影矩阵中Y轴缩放因子的符号。如果不这样做，图像将上下颠倒。
        mMvpMatrix.proj[1][1] *= -1;


        mVkLiteEngine = vklite::VkLiteEngineBuilder{}
                .layers({}, std::move(layers))
                .extensions({}, std::move(instanceExtensions))
                .deviceExtensions(std::move(deviceExtensions))
                .surfaceBuilder(std::make_unique<vklite::AndroidVulkanSurfaceBuilder>(mApp.window))
                .enableMsaa()
                .physicalDeviceAsDefault()
//                .graphicsPipeline([&](vklite::VulkanGraphicsPipelineConfigure &graphicsPipelineConfigure) {
//                    graphicsPipelineConfigure
//                            .vertexShaderCode(std::move(vertexShaderCode))
//                            .fragmentShaderCode(std::move(std::move(fragmentShaderCode)))
//                            .addVertex([&](vklite::VulkanVertexConfigure &vertexConfigure) {
//                                vertexConfigure
//                                        .binding(0)
//                                        .stride(sizeof(Vertex))
//                                        .addAttribute(ShaderFormat::Vec3)
//                                        .addAttribute(ShaderFormat::Vec2)
//                                        .setVertexBuffer(vertices);
//                            })
//                            .index(std::move(indices))
//                            .addPushConstant(sizeof(glm::mat4), 0, vk::ShaderStageFlagBits::eVertex)
//                            .addDescriptorSet([&](vklite::VulkanDescriptorSetConfigure &descriptorSetConfigure) {
//                                descriptorSetConfigure
//                                        .set(0)
//                                        .addUniform([&](vklite::VulkanUniformConfigure &uniformConfigure) {
//                                            uniformConfigure
//                                                    .binding(0)
//                                                    .descriptorRange(1)
//                                                    .descriptorOffset(0)
//                                                    .shaderStageFlags(vk::ShaderStageFlagBits::eVertex)
//                                                    .setUniformBuffer(mMvpMatrix);
//                                        })
//                                        .addSampler([&](vklite::VulkanSamplerConfigure &samplerConfigure) {
//                                            samplerConfigure
//                                                    .binding(1)
//                                                    .descriptorRange(1)
//                                                    .descriptorOffset(0)
//                                                    .shaderStageFlags(vk::ShaderStageFlagBits::eFragment)
//                                                    .setImage(std::move(image));
//                                        });
//
//                            });
//                })
                .build();
    }

    void Test06Load3dModel::init() {

    }

    // 检查是否准备好
    bool Test06Load3dModel::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test06Load3dModel::drawFrame() {
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        float scale = 1.0f;

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        model = glm::rotate(model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.model = model;

        mVkLiteEngine->updateCurrentFrameUniformBuffer(0, 0, mMvpMatrix);
        mVkLiteEngine->drawFrame();
    }

    // 清理操作
    void Test06Load3dModel::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
    }

} // test
