//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <optional>

#include "camera/NdkCameraMetadata.h"

namespace ndkcamera {

    // acamera_metadata_enum_acamera_info_supported_hardware_level
    enum class SupportedHardwareLevel {
        //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_LIMITED                    = 0,
        LIMITED,

        //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_FULL                       = 1,
        FULL,

        //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_LEGACY                     = 2,
        LEGACY,

        //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_3                          = 3,
        LEVEL_3,

        //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_EXTERNAL                   = 4,
        EXTERNAL,
    };

    enum class CameraLensFacing {
        // acamera_metadata_enum_acamera_lens_facing#ACAMERA_LENS_FACING_FRONT= 0,
        FRONT,

        // acamera_metadata_enum_acamera_lens_facing#ACAMERA_LENS_FACING_BACK= 1,
        BACK,

        // acamera_metadata_enum_acamera_lens_facing#ACAMERA_LENS_FACING_EXTERNAL= 2,
        EXTERNAL,
    };

    class CameraMetadata {
    private:
        ACameraMetadata *mMetadata;

    public:
        explicit CameraMetadata(ACameraMetadata *metadata);

        ~CameraMetadata();

        std::optional<SupportedHardwareLevel> querySupportedHardwareLevel();

        std::optional<CameraLensFacing> queryCameraLensFacing();
    };

} // ndkcamera
