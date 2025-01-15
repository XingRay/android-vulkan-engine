//
// Created by leixing on 2025/1/4.
//

#include "Test02SingleColorTriangle.h"
#include "FileUtil.h"

namespace test02 {

    Test02SingleColorTriangle::Test02SingleColorTriangle(const android_app &app, const std::string &name)
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

        std::vector<char> vertexShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/02_triangle_color.vert.spv");
        std::vector<char> fragmentShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/02_triangle_color.frag.spv");

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
                                        .addAttribute(ShaderFormat::Vec3);
                            })
                            .uniformSet([](engine::VulkanUniformSetConfigure &uniformSetConfigure) {
                                uniformSetConfigure
                                        .set(0);
//                                        .addBindingNormal(0, sizeof(ColorUniformBufferObject));
//                                .addBindingSampler(1,)
                            });
                })
                .build();

        mVulkanEngine = std::move(engine);
    }

    void Test02SingleColorTriangle::init() {


        std::vector<Vertex> vertices = {
                {{1.0f,  -1.0f, 0.0f}},
                {{-1.0f, -1.0f, 0.0f}},
                {{0.0f,  1.0f,  0.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2};

//        std::vector<char> vertexShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/02_triangle_color.vert.spv");
//        std::unique_ptr<engine::VulkanVertexShader> vertexShader = std::make_unique<engine::VulkanVertexShader>(vertexShaderCode);
//        vertexShader->addVertexBinding(sizeof(Vertex))
////                .addVertexAttribute(vk::Format::eR32G32B32Sfloat);
//                .addVertexAttribute(ShaderFormat::Vec3)
//                .addUniform(sizeof(ColorUniformBufferObject), mFrameCount);

//        std::vector<char> fragmentShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/02_triangle_color.frag.spv");
//        std::unique_ptr<engine::VulkanFragmentShader> fragmentShader = std::make_unique<engine::VulkanFragmentShader>(fragmentShaderCode);

//        std::unique_ptr<engine::VulkanSurface> surface = std::make_unique<engine::AndroidVulkanSurface>(mVulkanEngine->getVKInstance(), mApp.window);
//        mVulkanEngine->initVulkan(surface, deviceExtensions, vertexShader, fragmentShader);

//        mVulkanEngine->createDirectlyTransferVertexBuffer(mVertices.size() * sizeof(app::Vertex));
        mVulkanEngine->createStagingTransferVertexBuffer(vertices.size() * sizeof(Vertex));
//        mVulkanEngine->updateVertexBuffer(mVertices.data(), mVertices.size() * sizeof(Vertex));
        mVulkanEngine->updateVertexBuffer(vertices);

//        mVulkanEngine->createDirectlyTransferIndexBuffer(mIndices.size() * sizeof(uint32_t));
        mVulkanEngine->createStagingTransferIndexBuffer(indices.size() * sizeof(uint32_t));
        mVulkanEngine->updateIndexBuffer(indices);

        ColorUniformBufferObject colorUniformBufferObject;
        colorUniformBufferObject.color = {0.2f, 0.8f, 0.4f};

        for (int i = 0; i < mFrameCount; i++) {
            mVulkanEngine->updateUniformBuffer(i, 0, 0, &colorUniformBufferObject, sizeof(ColorUniformBufferObject));
        }

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