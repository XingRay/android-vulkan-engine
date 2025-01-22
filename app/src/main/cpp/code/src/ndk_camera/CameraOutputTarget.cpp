//
// Created by leixing on 2025/1/20.
//

#include "CameraOutputTarget.h"

namespace ndkcamera {
    CameraOutputTarget::CameraOutputTarget(ACameraOutputTarget *outputTarget) : mCameraOutputTarget(outputTarget) {

    }

    CameraOutputTarget::~CameraOutputTarget() {
        ACameraOutputTarget_free(mCameraOutputTarget);
    }

    const ACameraOutputTarget *CameraOutputTarget::getCameraOutputTarget() {
        return mCameraOutputTarget;
    }

} // ndkcamera