//
// Created by leixing on 2025/1/4.
//

#include "Test03ColoredTriangle.h"
#include "FileUtil.h"
#include "vklite/VulkanUtil.h"
#include "vklite/engine/configure/VkLiteEngineBuilder.h"

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
//                            .index(std::move(indices));
//                })
                .build();
    }

    void Test03ColoredTriangle::init() {

    }

    // 检查是否准备好
    bool Test03ColoredTriangle::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test03ColoredTriangle::drawFrame() {
        mVkLiteEngine->drawFrame();
    }

    // 清理操作
    void Test03ColoredTriangle::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
        mVkLiteEngine.reset();
    }

} // test