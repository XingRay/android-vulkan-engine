//
// Created by leixing on 2025/1/4.
//

#include "Test02SingleColorTriangle.h"
#include "FileUtil.h"
#include "engine/configure/engine/VulkanEngineBuilder.h"

namespace test02 {

    Test02SingleColorTriangle::Test02SingleColorTriangle(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        LOG_D("Test01SimpleTriangle::Test01SimpleTriangle");

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

        std::vector<char> vertexShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/02_triangle_color.vert.spv");
        std::vector<char> fragmentShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/02_triangle_color.frag.spv");

        std::vector<Vertex> vertices = {
                {{1.0f,  -1.0f, 0.0f}},
                {{-1.0f, -1.0f, 0.0f}},
                {{0.0f,  1.0f,  0.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2};

        ColorUniformBufferObject colorUniformBufferObject{{0.8f, 0.2f, 0.4f}};

        mVulkanEngine = engine::VulkanEngineBuilder{}
                .layers({}, std::move(layers))
                .extensions({}, std::move(instanceExtensions))
                .deviceExtensions(std::move(deviceExtensions))
                .surfaceBuilder(std::make_unique<engine::AndroidVulkanSurfaceBuilder>(mApp.window))
                .enableMsaa()
                .physicalDeviceAsDefault()
//                .graphicsPipeline([&](engine::VulkanGraphicsPipelineConfigure &graphicsPipelineConfigure) {
//                    graphicsPipelineConfigure
//                            .vertexShaderCode(std::move(vertexShaderCode))
//                            .fragmentShaderCode(std::move(std::move(fragmentShaderCode)))
//                            .addVertex([&](engine::VulkanVertexConfigure &vertexConfigure) {
//                                vertexConfigure
//                                        .binding(0)
//                                        .stride(sizeof(Vertex))
//                                        .addAttribute(ShaderFormat::Vec3)
//                                        .setVertexBuffer(vertices);
//                            })
//                            .index(std::move(indices))
//                                    // or
////                            .index([&](engine::VulkanIndexConfigure &indexConfigure) {
////                                indexConfigure
////                                        .setIndexBuffer(std::move(indices));
////                            })
////                            .addDescriptorSet([&](engine::VulkanDescriptorSetConfigure &descriptorSetConfigure) {
////                                descriptorSetConfigure
////                                        .set(0)
////                                        .addUniform([&](engine::VulkanUniformConfigure &uniformConfigure) {
////                                            uniformConfigure
////                                                    .binding(0)
////                                                    .descriptorRange(1)
////                                                    .descriptorOffset(0)
////                                                    .shaderStageFlags(vk::ShaderStageFlagBits::eVertex)
////                                                    .setUniformBuffer(colorUniformBufferObject);
////                                        });
////                            })
//                            ;
//                })
                .build();
    }

    void Test02SingleColorTriangle::init() {

    }

    // 检查是否准备好
    bool Test02SingleColorTriangle::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test02SingleColorTriangle::drawFrame() {
        mVulkanEngine->drawFrame();
    }

    // 清理操作
    void Test02SingleColorTriangle::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
        mVulkanEngine.reset();
    }

} // test