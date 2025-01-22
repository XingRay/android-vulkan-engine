//
// Created by leixing on 2025/1/20.
//

#include "CaptureRequest.h"
#include "engine/Log.h"

namespace ndkcamera {
    CaptureRequest::CaptureRequest(ACaptureRequest *captureRequest) : mCaptureRequest(captureRequest) {

    }

    CaptureRequest::~CaptureRequest() {
        LOG_D("ACaptureRequest_free(captureRequest:%p)", mCaptureRequest);
        ACaptureRequest_free(mCaptureRequest);
    }

    ACaptureRequest *CaptureRequest::getCaptureRequest() {
        return mCaptureRequest;
    }

    void CaptureRequest::addTarget(const std::unique_ptr<CameraOutputTarget> &cameraOutputTarget) {
        LOG_D("ACaptureRequest_addTarget()");
        ACaptureRequest_addTarget(mCaptureRequest, cameraOutputTarget->getCameraOutputTarget());
        LOG_D("ACaptureRequest_addTarget(mCaptureRequest:%p, cameraOutputTarget->getCameraOutputTarget():%p)", mCaptureRequest, cameraOutputTarget->getCameraOutputTarget());
    }

} // ndkcamera