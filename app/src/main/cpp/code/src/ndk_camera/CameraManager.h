//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <vector>
#include <string>
#include <functional>

#include <camera/NdkCameraManager.h>

#include "ndk_camera/CameraInfo.h"
#include "ndk_camera/CameraMetadata.h"
#include "ndk_camera/CameraDevice.h"

namespace ndkcamera {

    class CameraDevice;

    class CameraManager {
    private:
        ACameraManager *mCameraManager;

    public:
        CameraManager();

        ~CameraManager();

        std::vector<CameraInfo> queryCameraInfoList();

        std::unique_ptr<CameraMetadata> queryCameraCharacteristics(const std::string &cameraId);

        std::unique_ptr<CameraDevice> openCamera(const std::string& cameraId);
    };

} // ndkcamera
