//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <memory>

#include "camera/NdkCaptureRequest.h"
#include "ndk_camera/CameraOutputTarget.h"

namespace ndkcamera {

    class CaptureRequest {
    private:
        ACaptureRequest *mCaptureRequest;

    public:
        explicit CaptureRequest(ACaptureRequest *captureRequest);

        ~CaptureRequest();

        ACaptureRequest *getCaptureRequest();

        void addTarget(const std::unique_ptr<CameraOutputTarget> &cameraOutputTarget);

        void setFps(int32_t fps);

        void setFps(int32_t min, int32_t max);
    };

} // ndkcamera
