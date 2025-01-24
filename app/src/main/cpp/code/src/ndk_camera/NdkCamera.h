//
// Created by leixing on 2025/1/19.
//

#pragma once

#include <functional>

#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraDevice.h>

#include <media/NdkImageReader.h>
#include <android/hardware_buffer.h>

#include "ndk_camera/CameraManager.h"
#include "ndk_camera/CameraDevice.h"
#include "ndk_camera/CameraCaptureSession.h"
#include "ndk_camera/ImageReader.h"
#include "ndk_camera/CaptureRequest.h"
#include "ndk_camera/CameraOutputTarget.h"

namespace ndkcamera {

    class NdkCamera {
    private:
        std::unique_ptr<CameraManager> mCameraManager;
        std::unique_ptr<CameraDevice> mCameraDevice;
        std::unique_ptr<CameraCaptureSession> mCaptureSession;
        std::unique_ptr<ImageReader> mImageReader;
        std::unique_ptr<CaptureSessionOutputContainer> mCaptureSessionOutputContainer;
        std::unique_ptr<CaptureSessionOutput> mCaptureSessionOutput;

        std::unique_ptr<CaptureRequest> mCaptureRequest;
        std::unique_ptr<CameraOutputTarget> mCameraOutputTarget;

        std::function<void(NdkCamera *camera, AHardwareBuffer *hardwareBuffer)> mPreviewCallback;
    public:
        explicit NdkCamera();

        ~NdkCamera();

        void setPreviewCallback(std::function<void(NdkCamera *camera, AHardwareBuffer *hardwareBuffer)> previewCallback);

        void startPreview();

        void stopPreview();

        AHardwareBuffer *getLatestHardwareBuffer();

        void cleanLatestHardwareBuffer();

    private:
        static void onImageAvailable(void *context, AImageReader *reader);
    };

} // android
