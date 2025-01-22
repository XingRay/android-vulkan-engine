//
// Created by leixing on 2025/1/20.
//

#pragma once

#include "camera/NdkCameraCaptureSession.h"

namespace ndkcamera {

    class CaptureSessionOutput {
    private:
        ACaptureSessionOutput *mSessionOutput;

    public:
        explicit CaptureSessionOutput(ACaptureSessionOutput *sessionOutput);

        ~CaptureSessionOutput();

        const ACaptureSessionOutput *getCaptureSessionOutput();
    };

} // ndkcamera
