//
// Created by leixing on 2025/1/4.
//

#include "Test05TextureImage.h"
#include "FileUtil.h"

#include "stb_image.h"

namespace test05 {

    Test05TextureImage::Test05TextureImage(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app), mMvpMatrix(glm::mat4(1.0f)) {

        std::vector<const char *> instanceExtensions = {
                VK_KHR_SURFACE_EXTENSION_NAME,
                "VK_KHR_android_surface",

                // old version
                VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
                // new version
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        };

        std::vector<const char *> layers = {
                "VK_LAYER_KHRONOS_validation"
        };

        mVulkanEngine = std::make_unique<engine::VulkanEngine>(instanceExtensions, layers, mFrameCount);
    }

    void Test05TextureImage::init() {
        const std::vector<const char *> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

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

        std::vector<char> vertexShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/05_texture_image.vert.spv");
        std::unique_ptr<engine::VulkanVertexShader> vertexShader = std::make_unique<engine::VulkanVertexShader>(vertexShaderCode);
        (*vertexShader)
                .addVertexBinding(sizeof(Vertex))
                .addVertexAttribute(0, ShaderFormat::Vec3, offsetof(Vertex, position), 0)
                .setPushConstant(sizeof(glm::mat4));

        std::vector<char> fragmentShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/05_texture_image.frag.spv");
        std::unique_ptr<engine::VulkanFragmentShader> fragmentShader = std::make_unique<engine::VulkanFragmentShader>(fragmentShaderCode);
        (*fragmentShader)
                .addSampler(width, height, channels, mFrameCount, 0);

        std::unique_ptr<engine::VulkanSurface> surface = std::make_unique<engine::AndroidVulkanSurface>(mVulkanEngine->getVKInstance(), mApp.window);
        mVulkanEngine->initVulkan(surface, deviceExtensions, vertexShader, fragmentShader);

//        mVulkanEngine->createDirectlyTransferVertexBuffer(mVertices.size() * sizeof(app::Vertex));
        mVulkanEngine->createStagingTransferVertexBuffer(vertices.size() * sizeof(Vertex));
//        mVulkanEngine->updateVertexBuffer(mVertices.data(), mVertices.size() * sizeof(Vertex));
        mVulkanEngine->updateVertexBuffer(vertices);

//        mVulkanEngine->createDirectlyTransferIndexBuffer(mIndices.size() * sizeof(uint32_t));
        mVulkanEngine->createStagingTransferIndexBuffer(indices.size() * sizeof(uint32_t));
        mVulkanEngine->updateIndexBuffer(indices);

//        mvpMatrix.model = glm::mat4(1.0f); // 单位矩阵
//        mvpMatrix.view = glm::mat4(1.0f);  // 单位矩阵
//        mvpMatrix.proj = glm::mat4(1.0f);  // 单位矩阵
        glm::mat4 mvp = mMvpMatrix.proj * mMvpMatrix.view * mMvpMatrix.model;
        mVulkanEngine->updateVertexPushConstant(&(mvp));

        for (int i = 0; i < mFrameCount; i++) {
            mVulkanEngine->updateTextureSampler(i, 0, 0, pixels, width * height * channels);
        }

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
        mVulkanEngine->updateVertexPushConstant(&(mvp));

        mVulkanEngine->drawFrame();
    }

    // 清理操作
    void Test05TextureImage::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
        mVulkanEngine.reset();
    }

} // test