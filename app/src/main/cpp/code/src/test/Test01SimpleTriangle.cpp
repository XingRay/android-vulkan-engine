//
// Created by leixing on 2025/1/4.
//

#include "Test01SimpleTriangle.h"
#include "FileUtil.h"

namespace test01 {

    Test01SimpleTriangle::Test01SimpleTriangle(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        LOG_D("Test01SimpleTriangle::Test01SimpleTriangle");

        std::vector<std::string> instanceExtensions = {
                VK_KHR_SURFACE_EXTENSION_NAME,
                "VK_KHR_android_surface",

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

        std::vector<char> vertexShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/01_triangle.vert.spv");
        std::vector<char> fragmentShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/01_triangle.frag.spv");

        std::unique_ptr<engine::VulkanGraphicsEngine> engine = engine::VulkanEngineBuilder{}
                .layers({}, layers)
                .extensions({}, instanceExtensions)
                .asGraphics()
                .deviceExtensions(std::move(deviceExtensions))
                .surface(engine::AndroidVulkanSurface::surfaceBuilder(mApp.window))
                .enableMsaa()
                .physicalDeviceAsDefault()
                .vertexShader([&](engine::VulkanVertexShaderBuilder &vertexShaderBuilder) {
                    vertexShaderBuilder.code(std::move(vertexShaderCode))
                            .vertex([](engine::VertexBuilder &vertexBuilder) {
                                vertexBuilder
                                        .binding(0)
                                        .size(sizeof(Vertex))
                                        .addAttribute(ShaderFormat::Vec3);
                            });
                })
                .fragmentShader([&](engine::VulkanFragmentShaderBuilder &fragmentShaderBuilder) {
                    fragmentShaderBuilder.code(std::move(fragmentShaderCode));
                })
                .build();

        mVulkanEngine = std::move(engine);

    }

    void Test01SimpleTriangle::init() {

        std::vector<Vertex> vertices = {
                {{1.0f,  -1.0f, 0.0f}},
                {{-1.0f, -1.0f, 0.0f}},
                {{0.0f,  1.0f,  0.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2};

        mVulkanEngine->createStagingTransferVertexBuffer(vertices.size() * sizeof(Vertex));
        mVulkanEngine->updateVertexBuffer(vertices);

        mVulkanEngine->createStagingTransferIndexBuffer(indices.size() * sizeof(uint32_t));
        mVulkanEngine->updateIndexBuffer(indices);
    }

    // 检查是否准备好
    bool Test01SimpleTriangle::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test01SimpleTriangle::drawFrame() {
        mVulkanEngine->drawFrame();
    }

    // 清理操作
    void Test01SimpleTriangle::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
        mVulkanEngine.reset();
    }

} // test