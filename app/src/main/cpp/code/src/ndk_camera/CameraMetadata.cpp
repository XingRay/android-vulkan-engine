//
// Created by leixing on 2025/1/20.
//

#include "ndk_camera/CameraMetadata.h"
#include "ndk_camera/Log.h"

namespace ndkcamera {

    CameraMetadata::CameraMetadata(ACameraMetadata *metadata) : mMetadata(metadata) {

    }

    CameraMetadata::~CameraMetadata() {
        ACameraMetadata_free(mMetadata);
    }


    std::optional<SupportedHardwareLevel> CameraMetadata::querySupportedHardwareLevel() {
        // 检查相机硬件支持级别
        ACameraMetadata_const_entry hardwareLevelEntry = {};
        camera_status_t status = ACameraMetadata_getConstEntry(mMetadata, ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL, &hardwareLevelEntry);
        if (status != ACAMERA_OK) {
            LOG_E("Failed to get hardware level");
            return std::nullopt;
        }

        LOG_D("Camera hardware level: %d", hardwareLevelEntry.data.u8[0]);
        uint8_t supportedHardwareLevel = hardwareLevelEntry.data.u8[0];
        // 将 uint8_t 转换为 SupportedHardwareLevel 枚举
        switch (supportedHardwareLevel) {
            case 0:
                return SupportedHardwareLevel::LIMITED;
            case 1:
                return SupportedHardwareLevel::FULL;
            case 2:
                return SupportedHardwareLevel::LEGACY;
            case 3:
                return SupportedHardwareLevel::LEVEL_3;
            case 4:
                return SupportedHardwareLevel::EXTERNAL;
            default:
                LOG_E("Unknown hardware level: %d", supportedHardwareLevel);
                return std::nullopt;
        }
    }

    std::optional<CameraLensFacing> CameraMetadata::queryCameraLensFacing() {
        ACameraMetadata_const_entry lensFacingEntry = {};
        camera_status_t status = ACameraMetadata_getConstEntry(mMetadata, ACAMERA_LENS_FACING, &lensFacingEntry);
        if (status != ACAMERA_OK) {
            LOG_E("Failed to get lens facing");
            return std::nullopt;
        }

        switch (lensFacingEntry.data.u8[0]) {
            case ACAMERA_LENS_FACING_FRONT:
                return CameraLensFacing::FRONT;

            case ACAMERA_LENS_FACING_BACK:
                return CameraLensFacing::BACK;

            case ACAMERA_LENS_FACING_EXTERNAL:
                return CameraLensFacing::EXTERNAL;

            default:
                LOG_E("Unknown lensFacing: %d", lensFacingEntry.data.u8[0]);
                return std::nullopt;
        }
    }

} // ndkcamera