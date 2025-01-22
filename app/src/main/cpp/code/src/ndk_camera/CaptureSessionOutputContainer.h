//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <memory>

#include <camera/NdkCameraDevice.h>

#include "ndk_camera/CaptureSessionOutput.h"

namespace ndkcamera {

    class CaptureSessionOutputContainer {
    private:
        ACaptureSessionOutputContainer *mOutputContainer;

    public:
        explicit CaptureSessionOutputContainer();

        ~CaptureSessionOutputContainer();

        const ACaptureSessionOutputContainer *getOutputContainer();

        void add(const std::unique_ptr<CaptureSessionOutput>& sessionOutput);

    };

} // ndkcamera
