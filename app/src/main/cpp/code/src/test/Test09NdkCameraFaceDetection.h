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
#include "detector/MnnDetector.h"

namespace test09 {

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

    class Test09NdkCameraFaceDetection : public test::TestBase {
    private:

        const int mFrameCount = 2;

        const android_app &mApp;

//        std::unique_ptr<vklite::VulkanGraphicsEngine> mVkLiteEngine;

        std::unique_ptr<ndkcamera::NdkCamera> mNdkCamera;

        std::unique_ptr<detector::MnnDetector> mMnnDetector;
    public:
        // 构造函数初始化基类 TestBase，并传递 name
        explicit Test09NdkCameraFaceDetection(const android_app &app, const std::string &name);

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
