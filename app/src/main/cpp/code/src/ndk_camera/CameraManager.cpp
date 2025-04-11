//
// Created by leixing on 2025/1/20.
//

#include "ndk_camera/CameraManager.h"
#include "ndk_camera/Log.h"

namespace ndkcamera {
    CameraManager::CameraManager() : mCameraManager(ACameraManager_create()) {

    }

    CameraManager::~CameraManager() {
        ACameraManager_delete(mCameraManager);
    }

    std::vector<CameraInfo> CameraManager::queryCameraInfoList() {
        std::vector<CameraInfo> cameraInfoList;
        // 获取相机设备列表
        ACameraIdList *cameraIdList = nullptr;
        camera_status_t status = ACameraManager_getCameraIdList(mCameraManager, &cameraIdList);
        if (status != ACAMERA_OK || cameraIdList == nullptr || cameraIdList->numCameras == 0) {
            LOG_E("Failed to get camera id list or no cameras found");
            return std::move(cameraInfoList);
        }

        for (int i = 0; i < cameraIdList->numCameras; i++) {
            CameraInfo cameraInfo;

            const char *cameraId = cameraIdList->cameraIds[i];
            if (cameraId == nullptr) {
                LOG_E("Invalid camera ID");
                continue;
            }
            cameraInfo.id = std::string(cameraId);

            std::unique_ptr<CameraMetadata> metaData = queryCameraCharacteristics(std::string(cameraId));
            if (metaData == nullptr) {
                LOG_E("Failed to get camera characteristics");
                continue;
            }

            std::optional<SupportedHardwareLevel> supportedHardwareLevel = metaData->querySupportedHardwareLevel();
            if (supportedHardwareLevel == std::nullopt) {
                LOG_E("Failed to get supportedHardwareLevel");
                continue;
            }
            cameraInfo.supportedHardwareLevel = supportedHardwareLevel.value();

            std::optional<CameraLensFacing> lensFacing = metaData->queryCameraLensFacing();
            if (lensFacing == std::nullopt) {
                LOG_E("Failed to get lens facing");
                continue;
            }
            cameraInfo.lensFacing = lensFacing.value();

            std::string cameraLensFacing;
            switch (lensFacing.value()) {
                case CameraLensFacing::FRONT:
                    cameraLensFacing = "Front";
                    break;
                case CameraLensFacing::BACK:
                    cameraLensFacing = "Back";
                    break;
                case CameraLensFacing::EXTERNAL:
                    cameraLensFacing = "External";
                    break;
                default:
                    break;
            }
            LOG_D("Camera %d cameraLensFacing: %s", i, cameraLensFacing.c_str());

            cameraInfoList.push_back(cameraInfo);
        }

        return std::move(cameraInfoList);
    }

    std::unique_ptr<CameraMetadata> CameraManager::queryCameraCharacteristics(const std::string &cameraId) {
        ACameraMetadata *metadata;
        camera_status_t status = ACameraManager_getCameraCharacteristics(mCameraManager, cameraId.c_str(), &metadata);
        if (status != ACAMERA_OK || metadata == nullptr) {
            LOG_E("Failed to get camera characteristics");
            return nullptr;
        }
        return std::make_unique<CameraMetadata>(metadata);
    }

    std::unique_ptr<CameraDevice> CameraManager::openCamera(const std::string &cameraId) {
        std::unique_ptr<CameraDevice> cameraDevice = std::make_unique<CameraDevice>();

        ACameraDevice *device;
        camera_status_t status = ACameraManager_openCamera(mCameraManager, cameraId.c_str(), cameraDevice->createStateCallbacks(), &device);
        if (status != ACAMERA_OK || device == nullptr) {
            LOG_E("Failed to open camera: %d", status);
            return nullptr; // 返回空指针表示失败
        }
        LOG_D("Device opened successfully: %p", device);
        cameraDevice->setCameraDevice(device);

        return cameraDevice;
    }

} // ndkcamera