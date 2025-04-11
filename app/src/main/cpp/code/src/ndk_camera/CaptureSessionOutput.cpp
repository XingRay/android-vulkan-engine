//
// Created by leixing on 2025/1/20.
//

#include "ndk_camera/CaptureSessionOutput.h"
#include "camera/NdkCameraDevice.h"

namespace ndkcamera {
    CaptureSessionOutput::CaptureSessionOutput(ACaptureSessionOutput *sessionOutput)
            : mSessionOutput(sessionOutput) {

    }

    CaptureSessionOutput::~CaptureSessionOutput() {
        ACaptureSessionOutput_free(mSessionOutput);
    }

    const ACaptureSessionOutput *CaptureSessionOutput::getCaptureSessionOutput() {
        return mSessionOutput;
    }

} // ndkcamera