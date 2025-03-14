//
// Created by leixing on 2025/1/4.
//

#include "Test05TextureImage.h"
#include "FileUtil.h"

#include "stb_image.h"

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

        int width, height, channels;
        stbi_uc *pixels = stbi_load("/storage/emulated/0/01.png", &width, &height, &channels, STBI_rgb_alpha);

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
                                configure.addSampler(0, vk::ShaderStageFlagBits::eFragment, imageSize);
                            })
                            .addPushConstant(sizeof(glm::mat4), 0, vk::ShaderStageFlagBits::eVertex);
                })
                .build();

        mVulkanEngine = std::move(engine);
    }

    void Test05TextureImage::init() {
        // x轴朝右, y轴朝下, z轴朝前, 右手系 (x,y)->z
        std::vector<Vertex> vertices = {
                {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}}, // 左上角
                {{1.0f,  -1.0f, 0.0f}, {1.0f, 0.0f}}, // 右上角
                {{-1.0f, 1.0f,  0.0f}, {0.0f, 1.0f}}, // 左下角
                {{1.0f,  1.0f,  0.0f}, {1.0f, 1.0f}}, // 右下角
        };

        std::vector<uint32_t> indices = {0, 2, 1, 1, 2, 3};

        mMvpMatrix = MvpMatrix{};
        float scale = 1.0f;

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.model = model;
        mMvpMatrix.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
                                      glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3(1.0f, 1.0f, 0.0f));
        mMvpMatrix.proj = glm::perspective(glm::radians(45.0f), (float) ANativeWindow_getWidth(mApp.window) / (float) ANativeWindow_getHeight(mApp.window), 0.1f, 10.0f);

        int width, height, channels;
        stbi_uc *pixels = stbi_load("/storage/emulated/0/01.png", &width, &height, &channels, STBI_rgb_alpha);
        if (!pixels) {
            throw std::runtime_error("Failed to load texture image!");
        }
        LOG_D("image: [ %d, x %d], channels: %d", width, height, channels);

        LOG_D("mVulkanEngine->createStagingTransferVertexBuffer");
        mVulkanEngine->createStagingTransferVertexBuffer(vertices.size() * sizeof(Vertex));

        LOG_D("mVulkanEngine->updateVertexBuffer");
        mVulkanEngine->updateVertexBuffer(vertices);

        LOG_D("mVulkanEngine->createStagingTransferIndexBuffer");
        mVulkanEngine->createStagingTransferIndexBuffer(indices.size() * sizeof(uint32_t));
        LOG_D("mVulkanEngine->updateIndexBuffer");
        mVulkanEngine->updateIndexBuffer(indices);

        for (int i = 0; i < mFrameCount; i++) {
            LOG_D("mVulkanEngine->updateTextureSampler");
            mVulkanEngine->updateUniformBuffer(i, 0, 0, pixels, width * height * channels);
        }

        stbi_image_free(pixels);
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
        mVulkanEngine->updatePushConstant(0, &(mvp));

        mVulkanEngine->drawFrame();
    }

    // 清理操作
    void Test05TextureImage::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
        mVulkanEngine.reset();
    }

} // test
