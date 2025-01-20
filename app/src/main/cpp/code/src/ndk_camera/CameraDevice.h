//
// Created by leixing on 2025/1/20.
//

#pragma once

#include "camera/NdkCameraDevice.h"

#include "ndk_camera/CameraManager.h"

namespace ndkcamera {

    class CameraManager;

    class CameraDevice {
        friend CameraManager;

    private:
        ACameraDevice *mCameraDevice;

    public:
        CameraDevice();

        ~CameraDevice();

    private:
        ACameraDevice_StateCallbacks* getStateChangeCallbacks();

        void onDisconnected();

        void onError(int error);

        static void onDisconnected(void* context, ACameraDevice* device);

        static void onError(void* context, ACameraDevice* device, int error);
    };

} // ndkcamera
