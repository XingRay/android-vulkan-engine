//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <memory>

#include "camera/NdkCameraCaptureSession.h"

#include "ndk_camera/CaptureSessionOutputContainer.h"
#include "ndk_camera/CaptureRequest.h"

namespace ndkcamera {

    class CameraCaptureSession {

    private:
        ACameraCaptureSession *mCaptureSession;

        int mCaptureSequenceId;

    public:
        CameraCaptureSession();

        ~CameraCaptureSession();

        void setCameraCaptureSession(ACameraCaptureSession *cameraCaptureSession);

        ACameraCaptureSession_stateCallbacks *createStateCallbacks();

        void setRepeatingRequest(const std::unique_ptr<CaptureRequest> &captureRequest);

        void stopRepeating();

    private:
        static void onClosed(void *context, ACameraCaptureSession *session);

        static void onReady(void *context, ACameraCaptureSession *session);

        static void onActive(void *context, ACameraCaptureSession *session);

        void onClosed();

        void onReady();

        void onActive();

        ACameraCaptureSession_captureCallbacks *createCaptureCallbacks();

        static void onCaptureStarted(
                void *context, ACameraCaptureSession *session,
                const ACaptureRequest *request, int64_t timestamp);

        static void onCaptureProgressed(void *context, ACameraCaptureSession *session,
                                        ACaptureRequest *request, const ACameraMetadata *result);

        static void onCaptureCompleted(void *context, ACameraCaptureSession *session,
                                       ACaptureRequest *request, const ACameraMetadata *result);

        static void onCaptureFailed(void *context, ACameraCaptureSession *session,
                                    ACaptureRequest *request, ACameraCaptureFailure *failure);

        static void onCaptureSequenceCompleted(void *context, ACameraCaptureSession *session,
                                               int sequenceId, int64_t frameNumber);

        static void onCaptureSequenceAborted(void *context, ACameraCaptureSession *session,
                                             int sequenceId);

        static void onCaptureBufferLost(void *context, ACameraCaptureSession *session,
                                        ACaptureRequest *request, ACameraWindowType *window, int64_t frameNumber);


        void onCaptureStarted(
                ACameraCaptureSession *session,
                const ACaptureRequest *request, int64_t timestamp);

        void onCaptureProgressed(ACameraCaptureSession *session,
                                 ACaptureRequest *request, const ACameraMetadata *result);

        void onCaptureCompleted(ACameraCaptureSession *session,
                                ACaptureRequest *request, const ACameraMetadata *result);

        void onCaptureFailed(ACameraCaptureSession *session,
                             ACaptureRequest *request, ACameraCaptureFailure *failure);

        void onCaptureSequenceCompleted(ACameraCaptureSession *session,
                                        int sequenceId, int64_t frameNumber);

        void onCaptureSequenceAborted(ACameraCaptureSession *session,
                                      int sequenceId);

        void onCaptureBufferLost(ACameraCaptureSession *session,
                                 ACaptureRequest *request, ACameraWindowType *window, int64_t frameNumber);
    };

} // ndkcamera
