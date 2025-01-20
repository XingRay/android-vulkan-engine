//
// Created by leixing on 2025/1/20.
//

#include "CameraDevice.h"
#include "engine/Log.h"

namespace ndkcamera {

    CameraDevice::CameraDevice() : mCameraDevice(nullptr) {

    }

    CameraDevice::~CameraDevice() {
        if (mCameraDevice != nullptr) {
            ACameraDevice_close(mCameraDevice);
        }
    }


    ACameraDevice_StateCallbacks *CameraDevice::getStateChangeCallbacks() {
        ACameraDevice_StateCallbacks *callbacks = new ACameraDevice_StateCallbacks{};
        callbacks->context = this;
        callbacks->onDisconnected = onDisconnected;
        callbacks->onError = onError;
        return callbacks;
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