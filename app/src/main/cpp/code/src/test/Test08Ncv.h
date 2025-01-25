//
// Created by leixing on 2025/1/4.
//

#pragma once

#include "engine/Log.h"
#include "test/common/TestBase.h"

#include "engine/VulkanEngine.h"
#include "engine/vulkan_wrapper/android/AndroidVulkanSurface.h"

#include "game_activity/native_app_glue/android_native_app_glue.h"
#include "test/common/glm.h"

#include "ndk_camera/NdkCamera.h"

#include <metadata/version.hpp>
#include <graphics/complex_context.hpp>
#include "devices/image_reader.hpp"
#include "devices/camera.hpp"

namespace test08 {

    class Test08Ncv : public test::TestBase {
    private:
        const android_app &m_app;
        graphics::complex_context m_context;
        std::shared_ptr<::devices::image_reader> m_img_reader;
        std::shared_ptr<::devices::camera> m_camera;

    public:
        // 构造函数初始化基类 TestBase，并传递 name
        explicit Test08Ncv(const android_app &app, const std::string &name);

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
