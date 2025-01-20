//
// Created by leixing on 2025/1/19.
//

#pragma once

#include <functional>

#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraDevice.h>
#include <camera/NdkCameraCaptureSession.h>
#include <media/NdkImageReader.h>
#include <android/hardware_buffer.h>

#include "ndk_camera/CameraManager.h"

namespace ndkcamera {

    class NdkCamera {
    private:
        std::unique_ptr<CameraManager> mCameraManager;


        ACameraDevice *mCameraDevice;
        ACaptureSessionOutputContainer *mOutputContainer;
        AImageReader *mImageReader;
        ACaptureSessionOutput *mSessionOutput;
        ACameraOutputTarget *mOutputTarget;
        ACaptureRequest *mCaptureRequest;
        ACameraCaptureSession *mCaptureSession;

        std::function<void(NdkCamera *camera, AHardwareBuffer *hardwareBuffer)> mPreviewCallback;
    public:
        explicit NdkCamera(std::function<void(NdkCamera *camera, AHardwareBuffer *hardwareBuffer)> previewCallback);

        ~NdkCamera();

        void startPreview();

        void stopPreview();

    private:
        static void onImageAvailable(void *context, AImageReader *reader);
    };

} // android
