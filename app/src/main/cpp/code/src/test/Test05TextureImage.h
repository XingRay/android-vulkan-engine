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

namespace test05 {

    struct Vertex {
        glm::vec3 position;
        glm::vec2 uv;
    };

    struct MvpMatrix {
        glm::mat4 model; // 模型矩阵
        glm::mat4 view;  // 视图矩阵
        glm::mat4 proj;  // 投影矩阵
    };

    class Test05TextureImage : public test::TestBase {
    private:

        const int mFrameCount = 2;

        const android_app &mApp;

        std::unique_ptr<vklite::VkLiteEngine> mVkLiteEngine;

        MvpMatrix mMvpMatrix{};
    public:
        // 构造函数初始化基类 TestBase，并传递 name
        explicit Test05TextureImage(const android_app &app, const std::string &name);

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
