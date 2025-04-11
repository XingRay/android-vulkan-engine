//
// Created by leixing on 2025/1/20.
//

#include "ndk_camera/CaptureSessionOutputContainer.h"

#include "ndk_camera/Log.h"

namespace ndkcamera {

    CaptureSessionOutputContainer::CaptureSessionOutputContainer() : mOutputContainer(nullptr) {
        LOG_D("ACaptureSessionOutputContainer_create()");
        ACaptureSessionOutputContainer_create(&mOutputContainer);
        LOG_D("ACaptureSessionOutputContainer_create(mOutputContainer:%p)", mOutputContainer);
    }

    CaptureSessionOutputContainer::~CaptureSessionOutputContainer() {
        ACaptureSessionOutputContainer_free(mOutputContainer);
    }

    const ACaptureSessionOutputContainer *CaptureSessionOutputContainer::getOutputContainer() {
        return mOutputContainer;
    }

    void CaptureSessionOutputContainer::add(const std::unique_ptr<CaptureSessionOutput> &sessionOutput) {
        LOG_D("ACaptureSessionOutputContainer_add(mOutputContainer:%p, sessionOutput->getCaptureSessionOutput():%p)", mOutputContainer, sessionOutput->getCaptureSessionOutput());
        ACaptureSessionOutputContainer_add(mOutputContainer, sessionOutput->getCaptureSessionOutput());
    }

} // ndkcamera