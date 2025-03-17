//
// Created by leixing on 2025/1/4.
//

#include "Test07NdkCamera.h"
#include "FileUtil.h"

#include <chrono>
#include <thread>

namespace std {

    template<>
    struct hash<test07::Vertex> {
        size_t operator()(test07::Vertex const &vertex) const {
            size_t seed = 0;

            // 哈希 position
            hash<glm::vec3> vec3Hash;
            seed ^= vec3Hash(vertex.position) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            // 哈希 uv
            hash<glm::vec2> vec2Hash;
            seed ^= vec2Hash(vertex.uv) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
}


namespace test07 {
    Test07NdkCamera::Test07NdkCamera(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        mNdkCamera = std::make_unique<ndkcamera::NdkCamera>();
        mNdkCamera->startPreview();

        AHardwareBuffer *hardwareBuffer = nullptr;
        while (hardwareBuffer == nullptr) {
            LOG_D("waiting for getLatestHardwareBuffer...");
            hardwareBuffer = mNdkCamera->getLatestHardwareBuffer();
        }

        std::vector<std::string> instanceExtensions = {
                VK_KHR_SURFACE_EXTENSION_NAME,
                VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,

                // old version
                VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
                // new version
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,

                VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME,
                VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME,
                VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
        };

        std::vector<std::string> layers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::vector<std::string> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                VK_KHR_MAINTENANCE1_EXTENSION_NAME,
                VK_KHR_BIND_MEMORY_2_EXTENSION_NAME,
                VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
                VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME,
                VK_EXT_QUEUE_FAMILY_FOREIGN_EXTENSION_NAME,
                VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME,
                VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME,
                VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME,
                VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
                VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME
        };

        std::vector<char> vertexShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/07_ndk_camera.vert.spv");
        std::vector<char> fragmentShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/07_ndk_camera.frag.spv");

//        std::unique_ptr<engine::VulkanGraphicsEngine> engine = engine::VulkanEngineBuilder{}
//                .layers({}, layers)
//                .extensions({}, instanceExtensions)
//                .asGraphics()
//                .deviceExtensions(std::move(deviceExtensions))
//                .surface(engine::AndroidVulkanSurface::surfaceBuilder(mApp.window))
//                .enableMsaa(1)
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
//                                        .addAttribute(ShaderFormat::Vec2);
//                            })
//                            .uniformSet([=](engine::VulkanDescriptorSetConfigure &configure) {
//                                configure
//                                        .set(0)
//                                        .addAndroidHardwareBufferSampler(0, vk::ShaderStageFlagBits::eFragment, hardwareBuffer);
//                            });
//                })
//                .build();
//
//        mVulkanEngine = std::move(engine);
    }

    void Test07NdkCamera::init() {

        // x轴朝右, y轴朝下, z轴朝前, 右手系 (x,y)->z
        std::vector<Vertex> vertices = {
                {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}, // 左上角
                {{1.0f,  -1.0f, 0.0f}, {0.0f, 0.0f}}, // 右上角
                {{-1.0f, 1.0f,  0.0f}, {1.0f, 1.0f}}, // 左下角
                {{1.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // 右下角
        };

        std::vector<uint32_t> indices = {0, 2, 1, 1, 2, 3};

//        LOG_D("mVulkanEngine->createStagingTransferVertexBuffer");
//        mVulkanEngine->createStagingTransferVertexBuffer(vertices.size() * sizeof(Vertex));
//
//        LOG_D("mVulkanEngine->updateVertexBuffer");
//        mVulkanEngine->updateVertexBuffer(vertices);
//
//        LOG_D("mVulkanEngine->createStagingTransferIndexBuffer");
//        mVulkanEngine->createStagingTransferIndexBuffer(indices.size() * sizeof(uint32_t));
//        LOG_D("mVulkanEngine->updateIndexBuffer");
//        mVulkanEngine->updateIndexBuffer(indices);
    }

    // 检查是否准备好
    bool Test07NdkCamera::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test07NdkCamera::drawFrame() {
        // 静态变量用于帧率统计
        static auto startTime = std::chrono::steady_clock::now(); // 统计开始时间
        static int frameCount = 0;                               // 帧计数器


//        LOG_D("Test07NdkCamera::drawFrame()");
        AHardwareBuffer *buffer = mNdkCamera->getLatestHardwareBuffer();
//        LOG_D("AHardwareBuffer:%p", buffer);
        if (buffer != nullptr) {
//            mVulkanEngine->updateUniformBuffer(mVulkanEngine->getCurrentFrameIndex(), 0, 0, buffer, 0);
//            mVulkanEngine->drawFrame();

            // 增加帧计数器
            frameCount++;

            // 计算时间差
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

            // 每 5 秒输出一次帧率
            if (elapsedTime >= 5) {
                float fps = static_cast<float>(frameCount) / elapsedTime; // 计算帧率
                LOG_D("FPS: %.2f", fps);                                 // 输出帧率

                // 重置统计
                startTime = currentTime;
                frameCount = 0;
            }
        }

        mNdkCamera->cleanLatestHardwareBuffer();
    }

    // 清理操作
    void Test07NdkCamera::cleanup() {
//        LOG_I("Cleaning up %s", getName().c_str());
//        mVulkanEngine.reset();
    }

} // test
