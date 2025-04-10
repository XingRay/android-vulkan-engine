//
// Created by leixing on 2025/1/4.
//

#pragma once

#include "engine/Log.h"
#include "test/common/TestBase.h"

#include "engine/VulkanEngine.h"
#include "engine/vulkan_wrapper/platform/android/AndroidVulkanSurface.h"

#include "game_activity/native_app_glue/android_native_app_glue.h"
#include "test/common/glm.h"

#include "ndk_camera/NdkCamera.h"

#include <vector>
#include <functional>
#include <memory>

#include "engine/vulkan_wrapper/VulkanInstance.h"
#include "engine/vulkan_wrapper/VulkanSurface.h"
#include "engine/vulkan_wrapper/VulkanPhysicalDevice.h"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanShader.h"
#include "engine/vulkan_wrapper/VulkanSwapchain.h"
#include "engine/vulkan_wrapper/VulkanRenderPass.h"
#include "engine/vulkan_wrapper/VulkanGraphicsPipeline.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"
//#include "engine/vulkan_wrapper/buffer/VulkanVertexBuffer.h"
//#include "engine/vulkan_wrapper/buffer/VulkanIndexBuffer.h"
//#include "engine/vulkan_wrapper/buffer/VulkanUniformBuffer.h"
#include "engine/vulkan_wrapper/VulkanSyncObject.h"
#include "engine/vulkan_wrapper/VulkanFrameBuffer.h"
#include "engine/vulkan_wrapper/image/VulkanSampler.h"

#include "engine/common/StringListSelector.h"
#include "engine/VulkanPhysicalDeviceProvider.h"

namespace test10 {

    struct Vertex {
        glm::vec3 position;
        glm::vec2 uv;

        bool operator==(const Vertex &other) const {
            return position == other.position && uv == other.uv;
        }
    };

    struct MvpMatrix {
        alignas(16)glm::mat4 model; // 模型矩阵
        alignas(16)glm::mat4 view;  // 视图矩阵
        alignas(16)glm::mat4 proj;  // 投影矩阵
    };

    class Test10ComputeShader : public test::TestBase {
    private:

        bool mFrameBufferResized = false;
        uint32_t mCurrentFrameIndex = 0;
        const std::array<float, 4> mClearColor = {0.05f, 0.05f, 0.05f, 1.0f};//{0.2f, 0.4f, 0.6f, 1.0f};
        const std::array<float, 4> mDepthStencil = {1.0f, 0, 0, 0};

        const int mFrameCount = 2;
        const android_app &mApp;
        std::unique_ptr<ndkcamera::NdkCamera> mNdkCamera;

        std::unique_ptr<engine::VulkanInstance> mInstance;
        std::unique_ptr<engine::VulkanSurface> mSurface;
        std::unique_ptr<engine::VulkanPhysicalDevice> mPhysicalDevice;
        std::unique_ptr<engine::VulkanDevice> mDevice;

        std::unique_ptr<engine::VulkanSwapchain> mSwapchain;
        std::unique_ptr<engine::VulkanRenderPass> mRenderPass;

//        std::unique_ptr<engine::VulkanPipeline> mPipeline;
        std::unique_ptr<engine::VulkanCommandPool> mCommandPool;
        std::unique_ptr<engine::VulkanFrameBuffer> mFrameBuffer;

//        std::vector<std::unique_ptr<engine::VulkanVertexBuffer>> mVulkanVertexBuffers;
        std::vector<vk::Buffer> mVertexBuffers;
        std::vector<vk::DeviceSize> mVertexBufferOffsets;

//        std::unique_ptr<engine::VulkanIndexBuffer> mIndexBuffer;

        std::unique_ptr<engine::VulkanSyncObject> mSyncObject;

        // shader
        std::vector<char> mVertexShaderCode;
        std::vector<char> mFragmentShaderCode;
//        std::vector<engine::VulkanVertex> mVertices;
//        std::vector<engine::VulkanDescriptorSet> mDescriptorSets;
//        std::vector<engine::VulkanPushConstantConfigure> mPushConstants;

    public:
        // 构造函数初始化基类 TestBase，并传递 name
        explicit Test10ComputeShader(const android_app &app, const std::string &name);

        // 初始化操作
        void init() override;

        // 检查是否准备好
        bool isReady() override;

        // 绘制三角形帧
        void drawFrame() override;

        // 清理操作
        void cleanup() override;
    };

} // test
