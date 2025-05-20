//
// Created by leixing on 2025/1/4.
//

#include "Test04MvpMatrix.h"
#include "FileUtil.h"
#include "vklite/engine/configure/VkLiteEngineBuilder.h"

namespace test04 {

    Test04MvpMatrix::Test04MvpMatrix(const android_app &app, const std::string &name)
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

        std::vector<char> vertexShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/04_mvp_matrix.vert.spv");
        std::vector<char> fragmentShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/04_mvp_matrix.frag.spv");

        std::vector<Vertex> vertices = {
                {{1.0f,  -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                {{0.0f,  1.0f,  0.0f}, {0.0f, 0.0f, 1.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2};

        mVkLiteEngine = vklite::VkLiteEngineBuilder{}
                .layers({}, std::move(layers))
                .extensions({}, std::move(instanceExtensions))
                .deviceExtensions(std::move(deviceExtensions))
                .surfaceBuilder(std::make_unique<vklite::AndroidVulkanSurfaceBuilder>(mApp.window))
                .enableMsaa()
                .physicalDeviceAsDefault()
//                .graphicsPipeline([&](vklite::GraphicsPipelineConfigure &graphicsPipelineConfigure) {
//                    graphicsPipelineConfigure
//                            .vertexShaderCode(std::move(vertexShaderCode))
//                            .fragmentShaderCode(std::move(std::move(fragmentShaderCode)))
//                            .addVertex([&](vklite::VulkanVertexConfigure &vertexConfigure) {
//                                vertexConfigure
//                                        .binding(0)
//                                        .stride(sizeof(Vertex))
//                                        .addAttribute(ShaderFormat::Vec3)
//                                        .addAttribute(ShaderFormat::Vec3)
//                                        .setVertexBuffer(vertices);
//                            })
//                            .index(std::move(indices))
//                            .addPushConstant(sizeof(glm::mat4), 0, vk::ShaderStageFlagBits::eVertex);;
//                })
                .build();

        mMvpMatrix = MvpMatrix{};
        float scale = 1.0f;

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.model = model;
        mMvpMatrix.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
                                      glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3(1.0f, 1.0f, 0.0f));
        mMvpMatrix.proj = glm::perspective(glm::radians(45.0f), (float) ANativeWindow_getWidth(mApp.window) / (float) ANativeWindow_getHeight(mApp.window), 0.1f, 10.0f);
    }

    void Test04MvpMatrix::init() {

    }

    // 检查是否准备好
    bool Test04MvpMatrix::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test04MvpMatrix::drawFrame() {
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        float scale = 1.0f;

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        model = glm::rotate(model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.model = model;

//        mMvpMatrix.model = glm::mat4(1.0f); // 单位矩阵
//        mMvpMatrix.view = glm::mat4(1.0f);  // 单位矩阵
//        mMvpMatrix.proj = glm::mat4(1.0f);  // 单位矩阵
        glm::mat4 mvp = mMvpMatrix.proj * mMvpMatrix.view * mMvpMatrix.model;
        mVkLiteEngine->updatePushConstant(0, &(mvp));

        mVkLiteEngine->drawFrame();
    }

    // 清理操作
    void Test04MvpMatrix::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
//        mVkLiteEngine.reset();
    }

} // test