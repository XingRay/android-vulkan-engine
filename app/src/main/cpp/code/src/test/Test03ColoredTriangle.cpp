//
// Created by leixing on 2025/1/4.
//

#include "Test03ColoredTriangle.h"
#include "FileUtil.h"
#include "engine/VulkanUtil.h"

namespace test03 {

    Test03ColoredTriangle::Test03ColoredTriangle(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        LOG_D("Test03ColoredTriangle::Test03ColoredTriangle");

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

        std::vector<char> vertexShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/03_colored_triangle.vert.spv");
        std::vector<char> fragmentShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/03_colored_triangle.frag.spv");

//        std::unique_ptr<engine::VulkanGraphicsEngine> engine = engine::VulkanEngineBuilder{}
//                .layers({}, layers)
//                .extensions({}, instanceExtensions)
//                .asGraphics()
//                .deviceExtensions(std::move(deviceExtensions))
//                .surface(engine::AndroidVulkanSurface::surfaceBuilder(mApp.window))
//                .enableMsaa()
//                .physicalDeviceAsDefault()
//                .shader([&](engine::VulkanShaderConfigure &shaderConfigure) {
//                    shaderConfigure
//                            .vertexShaderCode(std::move(vertexShaderCode))
//                            .fragmentShaderCode(std::move(std::move(fragmentShaderCode)))
//                            .vertex([](engine::VulkanVertexConfigure &vertexConfigure) {
//                                vertexConfigure
//                                        .binding(0)
//                                        .size(sizeof(Vertex))
//                                        .addAttribute(ShaderFormat::Vec3)
//                                                //.addAttribute(1, 0, ShaderFormat::Vec3, sizeof(ShaderFormat::Vec3)) //error
//                                                //.addAttribute(1, 0, ShaderFormat::Vec3, engine::VulkanUtil::getFormatSize(vk::Format::eR32G32B32Sfloat)); //ok
//                                        .addAttribute(ShaderFormat::Vec3); //simple, binding set as vertexConfigure#binding, location and offset auto calc
//                            });
//                })
//                .build();
//
//        mVulkanEngine = std::move(engine);
    }

    void Test03ColoredTriangle::init() {
        std::vector<Vertex> vertices = {
                {{1.0f,  -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                {{0.0f,  1.0f,  0.0f}, {0.0f, 0.0f, 1.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2};

//        mVulkanEngine->createStagingTransferVertexBuffer(vertices.size() * sizeof(Vertex));
//        mVulkanEngine->updateVertexBuffer(vertices);
//
//        mVulkanEngine->createStagingTransferIndexBuffer(indices.size() * sizeof(uint32_t));
//        mVulkanEngine->updateIndexBuffer(indices);
    }

    // 检查是否准备好
    bool Test03ColoredTriangle::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test03ColoredTriangle::drawFrame() {
//        mVulkanEngine->drawFrame();
    }

    // 清理操作
    void Test03ColoredTriangle::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
//        mVulkanEngine.reset();
    }

} // test