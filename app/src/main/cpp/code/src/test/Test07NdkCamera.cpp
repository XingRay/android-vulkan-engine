//
// Created by leixing on 2025/1/4.
//

#include "Test07NdkCamera.h"
#include "FileUtil.h"

#include "stb_image.h"

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

    const char *TEXTURE_PATH = "/storage/emulated/0/test/model/viking_room/viking_room.png";

    Test07NdkCamera::Test07NdkCamera(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        mNdkCamera = std::make_unique<ndkcamera::NdkCamera>([](ndkcamera::NdkCamera *camera, AHardwareBuffer *hardwareBuffer) {
            LOG_D("hardwareBuffer:%p", hardwareBuffer);
        });
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

        int width, height, channels;
        stbi_uc *pixels = stbi_load(TEXTURE_PATH, &width, &height, &channels, STBI_rgb_alpha);
        channels = 4;

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
                                configure
                                        .set(0)
//                                        .addSampler(0, vk::ShaderStageFlagBits::eFragment, imageSize);
                                        .addAndroidHardwareBufferSampler(0, vk::ShaderStageFlagBits::eFragment, hardwareBuffer);
                            });
                })
                .build();

        mVulkanEngine = std::move(engine);


    }

    void Test07NdkCamera::init() {

        // x轴朝右, y轴朝下, z轴朝前, 右手系 (x,y)->z
        std::vector<Vertex> vertices = {
                {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}}, // 左上角
                {{1.0f,  -1.0f, 0.0f}, {1.0f, 0.0f}}, // 右上角
                {{-1.0f, 1.0f,  0.0f}, {0.0f, 1.0f}}, // 左下角
                {{1.0f,  1.0f,  0.0f}, {1.0f, 1.0f}}, // 右下角
        };

        std::vector<uint32_t> indices = {0, 2, 1, 1, 2, 3};

        LOG_D("mVulkanEngine->createStagingTransferVertexBuffer");
        mVulkanEngine->createStagingTransferVertexBuffer(vertices.size() * sizeof(Vertex));

        LOG_D("mVulkanEngine->updateVertexBuffer");
        mVulkanEngine->updateVertexBuffer(vertices);

        LOG_D("mVulkanEngine->createStagingTransferIndexBuffer");
        mVulkanEngine->createStagingTransferIndexBuffer(indices.size() * sizeof(uint32_t));
        LOG_D("mVulkanEngine->updateIndexBuffer");
        mVulkanEngine->updateIndexBuffer(indices);

        int width, height, channels;
        stbi_uc *pixels = stbi_load(TEXTURE_PATH, &width, &height, &channels, STBI_rgb_alpha);
        if (!pixels) {
            throw std::runtime_error("Failed to load texture image!");
        }
        LOG_D("image: [ %d, x %d], channels: %d", width, height, channels);

        for (int i = 0; i < mFrameCount; i++) {
            LOG_D("mVulkanEngine->updateTextureSampler");
            mVulkanEngine->updateUniformBuffer(i, 0, 0, pixels, width * height * channels);
        }

        stbi_image_free(pixels);
    }

    // 检查是否准备好
    bool Test07NdkCamera::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test07NdkCamera::drawFrame() {
//        LOG_D("Test07NdkCamera::drawFrame()");
        AHardwareBuffer *buffer = mNdkCamera->getLatestHardwareBuffer();
//        LOG_D("AHardwareBuffer:%p", buffer);
        if (buffer != nullptr) {
            mVulkanEngine->updateUniformBuffer(mVulkanEngine->getCurrentFrameIndex(), 0, 0, buffer, 0);
        }

        mVulkanEngine->drawFrame();
    }

    // 清理操作
    void Test07NdkCamera::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
        mVulkanEngine.reset();
    }

} // test
