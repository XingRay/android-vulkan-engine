// Copyright 2016 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <android/log.h>
#include "game_activity/native_app_glue/android_native_app_glue.h"
#include "vklite/engine/VkLiteEngine.h"
#include "vklite/platform/android/AndroidVulkanSurface.h"
#include "pthread.h"

#include "vklite/Log.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>


#include "test/Test01SimpleTriangle.h"
#include "test/Test02SingleColorTriangle.h"
#include "test/Test03ColoredTriangle.h"
#include "test/Test04MvpMatrix.h"
#include "test/Test05TextureImage.h"
#include "test/Test06Load3dModel.h"
#include "test/Test07NdkCamera.h"
#include "test/Test08FaceDetection.h"

void handle_cmd(android_app *app, int32_t cmd) {
//    LOG_D("AndroidMain#handle_cmd, cmd:%d", cmd);

    switch (cmd) {
        case APP_CMD_INIT_WINDOW: {
            app->test = new test01::Test01SimpleTriangle(*app, "Test01SimpleTriangle");
//            app->test = new test02::Test02SingleColorTriangle(*app, "Test02SingleColorTriangle");
//            app->test = new test03::Test03ColoredTriangle(*app, "Test03ColoredTriangle");
//            app->test = new test04::Test04MvpMatrix(*app, "Test04MvpMatrix");
//            app->test = new test05::Test05TextureImage(*app, "Test05TextureImage");
//            app->test = new test06::Test06Load3dModel(*app, "Test06Load3dModel");
//            app->test = new test07::Test07NdkCamera(*app, "Test07NdkCamera");
//            app->test = new test08::Test08FaceDetection(*app, "Test08FaceDetection");
            app->test->init();
        }
            break;

        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            if (app->test != nullptr) {
                app->test->cleanup();
                delete app->test;
                app->test = nullptr;
            }
            break;

        default:
            __android_log_print(ANDROID_LOG_INFO, "Vulkan Tutorials", "event not handled: %d", cmd);
    }
}

void android_main(struct android_app *app) {
    LOG_D("AndroidMain#android_main, thread:%ld", pthread_self());
    // Set the callback to process system events
    app->onAppCmd = handle_cmd;

    // Used to poll the events in the main loop
    int events;
    android_poll_source *source;

    // Main loop
    do {
        if (ALooper_pollOnce((app->test != nullptr && app->test->isReady()) ? 1 : 0, nullptr, &events, (void **) &source) >= 0) {
            if (source != nullptr) {
                source->process(app, source);
            }
        }

        // render if vulkan is ready
        if ((app->test != nullptr && app->test->isReady())) {
            app->test->drawFrame();
        }
    } while (app->destroyRequested == 0);
}

//extern "C"
//JNIEXPORT void JNICALL
//Java_io_github_xingray_vulkandemo_MainActivity_changeTriangleColor(JNIEnv *env, jobject thiz, jint color) {
//
//}

void runInLooper(const android_app *app, int color) {
    LOG_D("AndroidMain#runInLooper, thread:%ld", pthread_self());
    LOG_D("AndroidMain#runInLooper, color:%d", color);

    if (app->test != nullptr) {
//        VkCommandPool &commandPool = app->vulkanEngine->render.cmdPool_;
        LOG_D("vulkan test:%p", app->test);
        // 将32位颜色值转换为浮点RGB
        float r = ((color >> 16) & 0xFF) / 255.0f;
        float g = ((color >> 8) & 0xFF) / 255.0f;
        float b = (color & 0xFF) / 255.0f;

        // 调用VulkanEngine中的更新颜色方法
//        app->test->UpdateColor(r, g, b);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_io_github_xingray_vulkandemo_MainActivity_changeTriangleColor(JNIEnv *env, jobject thiz, jlong native_handle, jint color) {
    NativeCode *code = reinterpret_cast<NativeCode *>(native_handle);
    LOG_D("changeTriangleColor, thread:%ld", pthread_self());
    android_app *app = reinterpret_cast<android_app *>(code->instance);
    ALooper *looper = app->looper;

    if (looper == nullptr) {
        LOG_E("ALooper is null. Cannot post task.");
        return;
    }

    // 创建一个管道用于通信
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        LOG_E("Failed to create pipe: %s", strerror(errno));
        return;
    }
    const int readFd = pipefd[0];
    const int writeFd = pipefd[1];

    // 注册到 ALooper 中
    ALooper_addFd(looper, readFd, 0, ALOOPER_EVENT_INPUT, [](int fd, int events, void *data) -> int {
        if (events & ALOOPER_EVENT_INPUT) {
            int color;
            ssize_t bytesRead = read(fd, &color, sizeof(color));
            if (bytesRead > 0) {
                runInLooper(reinterpret_cast<android_app *>(data), color);
            } else {
                LOG_E("Failed to read from pipe: %s", strerror(errno));
            }
        }

        // 如果只监听一次事件，则在此关闭 fd
        close(fd);
        // 返回 0 以从 ALooper 中移除 fd
        return 0;
    }, app);

    write(writeFd, &color, sizeof(color));
    close(writeFd);
}