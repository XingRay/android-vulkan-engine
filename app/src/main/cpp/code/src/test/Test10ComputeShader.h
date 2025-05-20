//
// Created by leixing on 2025/1/4.
//

#pragma once

#include "vklite/Log.h"
#include "test/common/TestBase.h"

#include "vklite/engine/VkLiteEngine.h"
#include "vklite/platform/android/AndroidVulkanSurface.h"

#include "game_activity/native_app_glue/android_native_app_glue.h"
#include "test/common/glm.h"

#include "ndk_camera/NdkCamera.h"

#include <vector>
#include <functional>
#include <memory>

#include "vklite/instance/VulkanInstance.h"
#include "vklite/VulkanSurface.h"
#include "vklite/VulkanPhysicalDevice.h"
#include "vklite/device/VulkanDevice.h"
#include "vklite/shader/VulkanShader.h"
#include "vklite/swapchain/VulkanSwapchain.h"
#include "vklite/VulkanRenderPass.h"
#include "vklite/pipeline/GraphicsPipeline.h"
#include "vklite/command/VulkanCommandPool.h"
//#include "vklite/vulkan_wrapper/buffer/VulkanVertexBuffer.h"
//#include "vklite/vulkan_wrapper/buffer/VulkanIndexBuffer.h"
//#include "vklite/vulkan_wrapper/buffer/VulkanUniformBuffer.h"
#include "vklite/VulkanSyncObject.h"
#include "vklite/VulkanFrameBuffer.h"
#include "vklite/sampler/DefaultSampler.h"

#include "vklite/common/StringListSelector.h"
#include "vklite/configure/physical_device/VulkanPhysicalDeviceProvider.h"

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

        std::unique_ptr<vklite::VulkanInstance> mInstance;
        std::unique_ptr<vklite::VulkanSurface> mSurface;
        std::unique_ptr<vklite::VulkanPhysicalDevice> mPhysicalDevice;
        std::unique_ptr<vklite::VulkanDevice> mDevice;

        std::unique_ptr<vklite::VulkanSwapchain> mSwapchain;
        std::unique_ptr<vklite::VulkanRenderPass> mRenderPass;

//        std::unique_ptr<vklite::VulkanPipeline> mPipeline;
        std::unique_ptr<vklite::VulkanCommandPool> mCommandPool;
        std::unique_ptr<vklite::VulkanFrameBuffer> mFrameBuffer;

//        std::vector<std::unique_ptr<vklite::VulkanVertexBuffer>> mVulkanVertexBuffers;
        std::vector<vk::Buffer> mVertexBuffers;
        std::vector<vk::DeviceSize> mVertexBufferOffsets;

//        std::unique_ptr<vklite::VulkanIndexBuffer> mIndexBuffer;

        std::unique_ptr<vklite::VulkanSyncObject> mSyncObject;

        // shader
        std::vector<char> mVertexShaderCode;
        std::vector<char> mFragmentShaderCode;
//        std::vector<vklite::VulkanVertex> mVertices;
//        std::vector<vklite::VulkanDescriptorSet> mDescriptorSets;
//        std::vector<vklite::VulkanPushConstantConfigure> mPushConstants;

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
