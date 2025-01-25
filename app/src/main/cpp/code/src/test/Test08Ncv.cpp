//
// Created by leixing on 2025/1/4.
//

#include "Test08Ncv.h"
#include "FileUtil.h"

#include <chrono>
#include <thread>

constexpr const char *PACKAGE_NAME = "com.ktzevani.nativecameravulkan";
constexpr const char *PROJECT_NAME = "native-camera-vulkan";

namespace test08 {
    Test08Ncv::Test08Ncv(const android_app &app, const std::string &name)
            : TestBase(name), m_app(app), m_context(name, app) {

        uint32_t width = ANativeWindow_getWidth(app.window);
        uint32_t height = ANativeWindow_getHeight(app.window);

        m_img_reader = std::make_shared<devices::image_reader>(width, height, AIMAGE_FORMAT_PRIVATE, AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE, 4);
        m_camera = std::make_shared<devices::camera>(m_img_reader->get_window());
    }

    void Test08Ncv::init() {

        m_camera->start_capturing();

        while (!m_img_reader->get_latest_buffer()){
            usleep(10000u);
        }

        m_context.initialize_graphics(m_img_reader->get_latest_buffer());
    }

    // 检查是否准备好
    bool Test08Ncv::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test08Ncv::drawFrame() {
        glm::vec4 a_rgba{0,0,0,0};
        m_context.render_frame(a_rgba, m_img_reader->get_latest_buffer());
    }

    // 清理操作
    void Test08Ncv::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
        m_camera->stop_capturing();
        m_camera.reset();
        m_img_reader.reset();
    }

} // test
