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
#include "engine/VulkanEngine.h"
#include "android/AndroidVulkanSurface.h"
#include "pthread.h"

#include "Log.h"

// Vulkan 更新函数
//void VulkanUpdateColor(float r, float g, float b) {
//	// 假设通过 Push Constants 更新颜色
//	vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(float) * 3, &color);
//}

// 更新三角形颜色
//void UpdateTriangleColor(uint32_t color) {
//	float r = ((color >> 16) & 0xFF) / 255.0f;
//	float g = ((color >> 8) & 0xFF) / 255.0f;
//	float b = (color & 0xFF) / 255.0f;
//	VulkanUpdateColor(r, g, b);
//}

// Process the next main command.
std::vector<char> loadFile(AAssetManager *assetManager, const char *filePath) {
    // 打开文件
    AAsset *file = AAssetManager_open(assetManager, filePath, AASSET_MODE_BUFFER);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + std::string(filePath));
    }

    // 获取文件大小
    size_t fileLength = AAsset_getLength(file);

    // 读取文件内容
    std::vector<char> fileContent(fileLength); // 自动管理内存
    AAsset_read(file, fileContent.data(), fileLength);

    // 关闭文件
    AAsset_close(file);

    // 返回文件内容和大小
    return fileContent;
}

void handle_cmd(android_app *app, int32_t cmd) {
    LOG_D("AndroidMain#handle_cmd, cmd:%d", cmd);

    switch (cmd) {
        case APP_CMD_INIT_WINDOW: {

            std::vector<const char *> instanceExtensions = {
                    VK_KHR_SURFACE_EXTENSION_NAME,
                    "VK_KHR_android_surface",

                    // old version
                    VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
                    // new version
                    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
            };

            std::vector<const char *> layers = {
                    "VK_LAYER_KHRONOS_validation"
            };

            const std::vector<const char *> deviceExtensions = {
                    VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };

            // The window is being shown, get it ready.
            app->vulkanEngine = new engine::VulkanEngine(instanceExtensions, layers);

            std::vector<char> vertexShader = loadFile(app->activity->assetManager, "shaders/01_triangle.vert.spv");
            std::vector<char> fragmentShader = loadFile(app->activity->assetManager, "shaders/01_triangle.frag.spv");

            std::unique_ptr<engine::VulkanSurface> surface = std::make_unique<engine::AndroidVulkanSurface>(app->vulkanEngine->getVKInstance(), app->window);
            app->vulkanEngine->initVulkan(surface, deviceExtensions, vertexShader, fragmentShader);
        }
            break;

        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            app->vulkanEngine->deleteVulkan();
            delete app->vulkanEngine;
            app->vulkanEngine = nullptr;
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
        if (ALooper_pollOnce((app->vulkanEngine != nullptr && app->vulkanEngine->isVulkanReady()) ? 1 : 0, nullptr, &events, (void **) &source) >= 0) {
            if (source != nullptr) {
                source->process(app, source);
            }
        }

        // render if vulkan is ready
        if ((app->vulkanEngine != nullptr && app->vulkanEngine->isVulkanReady())) {
            app->vulkanEngine->drawFrame();
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

    if (app->vulkanEngine != nullptr) {
//        VkCommandPool &commandPool = app->vulkanEngine->render.cmdPool_;
        LOG_D("vulkanEngine:%p", app->vulkanEngine);
        // 将32位颜色值转换为浮点RGB
        float r = ((color >> 16) & 0xFF) / 255.0f;
        float g = ((color >> 8) & 0xFF) / 255.0f;
        float b = (color & 0xFF) / 255.0f;

        // 调用VulkanEngine中的更新颜色方法
        app->vulkanEngine->UpdateColor(r, g, b);
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