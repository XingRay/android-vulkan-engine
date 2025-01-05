//
// Created by leixing on 2025/1/4.
//

#include "Test03ColoredTriangle.h"
#include "FileUtil.h"

namespace test03 {

    Test03ColoredTriangle::Test03ColoredTriangle(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        LOG_D("Test01SimpleTriangle::Test01SimpleTriangle");

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

    void Test03ColoredTriangle::init() {
        LOG_D("Test01SimpleTriangle::init");
        const std::vector<const char *> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        std::vector<Vertex> mVertices;
        std::vector<uint32_t> mIndices;

        mVertices = {
                {{1.0f,  -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                {{0.0f,  1.0f,  0.0f}, {0.0f, 0.0f, 1.0f}},
        };

        mIndices = {0, 1, 2};

        std::vector<char> vertexShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/03_colored_triangle.vert.spv");
        std::unique_ptr<engine::VulkanVertexShader> vertexShader = std::make_unique<engine::VulkanVertexShader>(vertexShaderCode);
        size_t positionOffset = offsetof(Vertex, position);
        size_t colorOffset = offsetof(Vertex, color);
        LOG_D("positionOffset:%ld, colorOffset:%ld", positionOffset, colorOffset);
        vertexShader->addVertexBinding(sizeof(Vertex))
                .addVertexAttribute(0, vk::Format::eR32G32B32Sfloat, positionOffset, 0)
                .addVertexAttribute(1, vk::Format::eR32G32B32Sfloat, colorOffset, 0);

        std::vector<char> fragmentShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/03_colored_triangle.frag.spv");
        std::unique_ptr<engine::VulkanFragmentShader> fragmentShader = std::make_unique<engine::VulkanFragmentShader>(fragmentShaderCode);

        std::unique_ptr<engine::VulkanSurface> surface = std::make_unique<engine::AndroidVulkanSurface>(mVulkanEngine->getVKInstance(), mApp.window);
        mVulkanEngine->initVulkan(surface, deviceExtensions, vertexShader, fragmentShader);

//        mVulkanEngine->createDirectlyTransferVertexBuffer(mVertices.size() * sizeof(app::Vertex));
        mVulkanEngine->createStagingTransferVertexBuffer(mVertices.size() * sizeof(Vertex));
//        mVulkanEngine->updateVertexBuffer(mVertices.data(), mVertices.size() * sizeof(Vertex));
        mVulkanEngine->updateVertexBuffer(mVertices);

//        mVulkanEngine->createDirectlyTransferIndexBuffer(mIndices.size() * sizeof(uint32_t));
        mVulkanEngine->createStagingTransferIndexBuffer(mIndices.size() * sizeof(uint32_t));
        mVulkanEngine->updateIndexBuffer(mIndices);
    }

    // 检查是否准备好
    bool Test03ColoredTriangle::isReady() {
        LOG_I("%s is ready!", getName().c_str());
        return true;
    }

    // 绘制三角形帧
    void Test03ColoredTriangle::drawFrame() {
        LOG_I("Drawing a simple triangle for %s", getName().c_str());
        mVulkanEngine->drawFrame();
    }

    // 清理操作
    void Test03ColoredTriangle::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
        mVulkanEngine.reset();
    }

} // test