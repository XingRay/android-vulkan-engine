//
// Created by leixing on 2025/1/4.
//

#include "Test06Load3dModel.h"
#include "FileUtil.h"

#include "stb_image.h"

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
//    const char *TEXTURE_PATH = "/storage/emulated/0/01.png";

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

        int width, height, channels;
        stbi_uc *pixels = stbi_load(TEXTURE_PATH, &width, &height, &channels, STBI_rgb_alpha);
        channels = 4;

        std::unique_ptr<engine::VulkanGraphicsEngine> engine = engine::VulkanEngineBuilder{}
                .layers({}, layers)
                .extensions({}, instanceExtensions)
                .asGraphics()
                .deviceExtensions(std::move(deviceExtensions))
                .surface(engine::AndroidVulkanSurface::surfaceBuilder(mApp.window))
                .enableMsaa()
                .physicalDeviceAsDefault()
                .shader([&](engine::VulkanShaderConfigure &shaderConfigure) {
                    shaderConfigure
                            .vertexShaderCode(std::move(vertexShaderCode))
                            .fragmentShaderCode(std::move(std::move(fragmentShaderCode)))
                            .vertex([](engine::VulkanVertexConfigure &vertexConfigure) {
                                vertexConfigure
                                        .binding(0)
                                        .size(sizeof(Vertex))
                                        .addAttribute(ShaderFormat::Vec3)
                                        .addAttribute(ShaderFormat::Vec2);
                            })
                            .uniformSet([=](engine::VulkanDescriptorSetConfigure &configure) {
                                engine::ImageSize imageSize(width, height, channels);
                                configure
                                        .set(0)
                                        .addUniform(0, vk::ShaderStageFlagBits::eVertex, sizeof(MvpMatrix))
                                        .addSampler(1, vk::ShaderStageFlagBits::eFragment, imageSize);
                            });
                })
                .build();

        mVulkanEngine = std::move(engine);
    }

    void Test06Load3dModel::init() {
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

        LOG_D("mVulkanEngine->createStagingTransferVertexBuffer");
        mVulkanEngine->createStagingTransferVertexBuffer(vertices.size() * sizeof(Vertex));

        LOG_D("mVulkanEngine->updateVertexBuffer");
        mVulkanEngine->updateVertexBuffer(vertices);

        LOG_D("mVulkanEngine->createStagingTransferIndexBuffer");
        mVulkanEngine->createStagingTransferIndexBuffer(indices.size() * sizeof(uint32_t));
        LOG_D("mVulkanEngine->updateIndexBuffer");
        mVulkanEngine->updateIndexBuffer(indices);

        int width, height, channels;
        stbi_uc *pixels = stbi_load(TEXTURE_PATH, &width, &height, &channels, STBI_rgb_alpha);
        if (!pixels) {
            throw std::runtime_error("Failed to load texture image!");
        }
        LOG_D("image: [ %d, x %d], channels: %d", width, height, channels);

        for (int i = 0; i < mFrameCount; i++) {
            LOG_D("mVulkanEngine->updateTextureSampler");
            mVulkanEngine->updateUniformBuffer(i, 0, 1, pixels, width * height * channels);
        }

        stbi_image_free(pixels);
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

        mVulkanEngine->updateUniformBuffer(mVulkanEngine->getCurrentFrameIndex(), 0, 0, &(mMvpMatrix), sizeof(MvpMatrix));
        mVulkanEngine->drawFrame();
    }

    // 清理操作
    void Test06Load3dModel::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
        mVulkanEngine.reset();
    }

} // test
