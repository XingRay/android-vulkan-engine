//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <memory>

#include "camera/NdkCameraDevice.h"

#include "ndk_camera/CameraCaptureSession.h"
#include "ndk_camera/CaptureRequest.h"

namespace ndkcamera {

    class CameraDevice {
    private:
        ACameraDevice *mCameraDevice;

        std::unique_ptr<CaptureSessionOutputContainer> mCaptureSessionOutputContainer;

    public:
        CameraDevice();

        ~CameraDevice();

        void setCameraDevice(ACameraDevice *cameraDevice);

        std::unique_ptr<CameraCaptureSession> createCaptureSession(const std::unique_ptr<CaptureSessionOutputContainer> &captureSessionOutputContainer);

        std::unique_ptr<CameraCaptureSession> createCaptureSession(const std::unique_ptr<CaptureSessionOutputContainer> &captureSessionOutputContainer,
                                                                   const std::unique_ptr<CaptureRequest> &captureRequest);

        ACameraDevice_StateCallbacks *createStateCallbacks();

        std::unique_ptr<CaptureRequest> createCaptureRequest();

    private:

        void onDisconnected();

        void onError(int error);

        static void onDisconnected(void *context, ACameraDevice *device);

        static void onError(void *context, ACameraDevice *device, int error);
    };

} // ndkcamera
