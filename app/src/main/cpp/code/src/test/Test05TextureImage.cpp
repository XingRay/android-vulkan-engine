//
// Created by leixing on 2025/1/4.
//

#include "Test05TextureImage.h"
#include "FileUtil.h"

#include "stb_image.h"
#include "vklite/engine/configure/VkLiteEngineBuilder.h"
#include "image/StbImage.h"

namespace test05 {

    Test05TextureImage::Test05TextureImage(const android_app &app, const std::string &name)
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

        std::vector<char> vertexShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/05_texture_image.vert.spv");
        std::vector<char> fragmentShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/05_texture_image.frag.spv");

        // x轴朝右, y轴朝下, z轴朝前, 右手系 (x,y)->z
        std::vector<Vertex> vertices = {
                {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}}, // 左上角
                {{1.0f,  -1.0f, 0.0f}, {1.0f, 0.0f}}, // 右上角
                {{-1.0f, 1.0f,  0.0f}, {0.0f, 1.0f}}, // 左下角
                {{1.0f,  1.0f,  0.0f}, {1.0f, 1.0f}}, // 右下角
        };

        std::vector<uint32_t> indices = {0, 2, 1, 1, 2, 3};

        std::unique_ptr<vklite::ImageInterface> image = image::StbImage::loadImage("/storage/emulated/0/01.png");

        std::unique_ptr<vklite::VkLiteEngine> engine = vklite::VkLiteEngineBuilder{}
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
//                                descriptorSetConfigure.addSampler([&](vklite::VulkanSamplerConfigure &samplerConfigure) {
//                                    samplerConfigure
//                                            .binding(0)
//                                            .descriptorRange(1)
//                                            .descriptorOffset(0)
//                                            .shaderStageFlags(vk::ShaderStageFlagBits::eFragment)
//                                            .setImage(std::move(image));
//                                });
//                            });
//                })
                .build();

        mVkLiteEngine = std::move(engine);
    }

    void Test05TextureImage::init() {
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

    // 检查是否准备好
    bool Test05TextureImage::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test05TextureImage::drawFrame() {
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
    void Test05TextureImage::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
//        mVkLiteEngine.reset();
    }

} // test
