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

namespace test01 {

    struct Vertex {
        glm::vec3 pos;
    };

    class Test01SimpleTriangle : public test::TestBase {
    private:

        const android_app &mApp;

        std::unique_ptr<vklite::VkLiteEngine> mVkLiteEngine;

    public:
        // 构造函数初始化基类 TestBase，并传递 name
        explicit Test01SimpleTriangle(const android_app &vertexConfigure, const std::string &name);

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
