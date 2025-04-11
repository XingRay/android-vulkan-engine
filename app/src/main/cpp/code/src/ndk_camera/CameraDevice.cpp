//
// Created by leixing on 2025/1/20.
//

#include "ndk_camera/CameraDevice.h"
#include "ndk_camera/Log.h"

namespace ndkcamera {

    CameraDevice::CameraDevice() : mCameraDevice(nullptr) {
        mCaptureSessionOutputContainer = std::make_unique<CaptureSessionOutputContainer>();
    }

    CameraDevice::~CameraDevice() {
        if (mCameraDevice != nullptr) {
            ACameraDevice_close(mCameraDevice);
        }
    }

    void CameraDevice::setCameraDevice(ACameraDevice *cameraDevice) {
        mCameraDevice = cameraDevice;
    }

    std::unique_ptr<CameraCaptureSession> CameraDevice::createCaptureSession(const std::unique_ptr<CaptureSessionOutputContainer> &captureSessionOutputContainer) {
        std::unique_ptr<CameraCaptureSession> cameraCaptureSession = std::make_unique<CameraCaptureSession>();
        const ACaptureSessionOutputContainer *outputs = captureSessionOutputContainer->getOutputContainer();
        ACameraCaptureSession_stateCallbacks *callbacks = cameraCaptureSession->createStateCallbacks();

        ACameraCaptureSession *captureSession;
        LOG_D("ACameraDevice_createCaptureSession()");
//        camera_status_t status = ACameraDevice_createCaptureSession(mCameraDevice, outputs, callbacks, &captureSession);
        ACaptureRequest *captureRequest;
        ACameraDevice_createCaptureRequest(mCameraDevice, TEMPLATE_PREVIEW, &captureRequest);

        // 设置目标帧率范围为 60 FPS
        int32_t targetFpsRange[2] = {60, 60};
        ACaptureRequest_setEntry_i32(captureRequest, ACAMERA_CONTROL_AE_TARGET_FPS_RANGE, 2, targetFpsRange);
        LOG_D("ACameraDevice_createCaptureSessionWithSessionParameters");
        camera_status_t status = ACameraDevice_createCaptureSessionWithSessionParameters(mCameraDevice, outputs, captureRequest, callbacks, &captureSession);
        LOG_D("ACameraDevice_createCaptureSession(mCameraDevice:%p, outputs:%p, captureSession:%p)", mCameraDevice, outputs, captureSession);
        if (status != ACAMERA_OK || captureSession == nullptr) {
            LOG_E("Failed to create capture session: %d", status);
            return nullptr; // 返回空指针表示失败
        }
        cameraCaptureSession->setCameraCaptureSession(captureSession);

        return cameraCaptureSession;
    }

    std::unique_ptr<CameraCaptureSession> CameraDevice::createCaptureSession(const std::unique_ptr<CaptureSessionOutputContainer> &captureSessionOutputContainer,
                                                                             const std::unique_ptr<CaptureRequest> &captureRequest) {
        std::unique_ptr<CameraCaptureSession> cameraCaptureSession = std::make_unique<CameraCaptureSession>();
        const ACaptureSessionOutputContainer *outputs = captureSessionOutputContainer->getOutputContainer();
        ACameraCaptureSession_stateCallbacks *callbacks = cameraCaptureSession->createStateCallbacks();

        ACameraCaptureSession *captureSession;
        LOG_D("ACameraDevice_createCaptureSessionWithSessionParameters()");
        camera_status_t status = ACameraDevice_createCaptureSessionWithSessionParameters(mCameraDevice, outputs, captureRequest->getCaptureRequest(), callbacks, &captureSession);
        LOG_D("ACameraDevice_createCaptureSession(mCameraDevice:%p, outputs:%p, captureSession:%p)", mCameraDevice, outputs, captureSession);
        if (status != ACAMERA_OK || captureSession == nullptr) {
            LOG_E("Failed to create capture session: %d", status);
            return nullptr; // 返回空指针表示失败
        }
        cameraCaptureSession->setCameraCaptureSession(captureSession);

        return cameraCaptureSession;
    }

    ACameraDevice_StateCallbacks *CameraDevice::createStateCallbacks() {
        ACameraDevice_StateCallbacks *callbacks = new ACameraDevice_StateCallbacks{};
        callbacks->context = this;
        callbacks->onDisconnected = onDisconnected;
        callbacks->onError = onError;
        return callbacks;
    }

    std::unique_ptr<CaptureRequest> CameraDevice::createCaptureRequest() {
        ACaptureRequest *captureRequest;
        LOG_D("ACameraDevice_createCaptureRequest()");
        ACameraDevice_createCaptureRequest(mCameraDevice, TEMPLATE_PREVIEW, &captureRequest);
        LOG_D("ACameraDevice_createCaptureRequest(mCameraDevice:%p, captureRequest:%p)", mCameraDevice, captureRequest);

        return std::make_unique<CaptureRequest>(captureRequest);
    }

    void CameraDevice::onDisconnected() {
        LOG_D("CameraDevice::onDisconnected()");
    }

    void CameraDevice::onError(int error) {
        LOG_D("CameraDevice::onError(error:%d)", error);
    }

    void CameraDevice::onDisconnected(void *context, ACameraDevice *device) {
        CameraDevice *cameraDevice = static_cast<CameraDevice *>(context);
        cameraDevice->onDisconnected();
    }

    void CameraDevice::onError(void *context, ACameraDevice *device, int error) {
        CameraDevice *cameraDevice = static_cast<CameraDevice *>(context);
        cameraDevice->onError(error);
    }

} // ndkcamera