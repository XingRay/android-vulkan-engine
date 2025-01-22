//
// Created by leixing on 2025/1/20.
//

#pragma once

#include "camera/NdkCaptureRequest.h"

namespace ndkcamera {

    class CameraOutputTarget {
    private:
        ACameraOutputTarget *mCameraOutputTarget;
    public:
        explicit CameraOutputTarget(ACameraOutputTarget *outputTarget);

        ~CameraOutputTarget();

        const ACameraOutputTarget *getCameraOutputTarget();
    };

} // ndkcamera
