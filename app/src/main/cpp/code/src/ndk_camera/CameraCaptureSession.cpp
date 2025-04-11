//
// Created by leixing on 2025/1/20.
//

#include "ndk_camera/CameraCaptureSession.h"
#include "ndk_camera/Log.h"
#include <array>

namespace ndkcamera {
    CameraCaptureSession::CameraCaptureSession() : mCaptureSession(nullptr) {

    }

    CameraCaptureSession::~CameraCaptureSession() {
        ACameraCaptureSession_close(mCaptureSession);
    }

    void CameraCaptureSession::setCameraCaptureSession(ACameraCaptureSession *cameraCaptureSession) {
        mCaptureSession = cameraCaptureSession;
    }

    ACameraCaptureSession_stateCallbacks *CameraCaptureSession::createStateCallbacks() {
        ACameraCaptureSession_stateCallbacks *callbacks = new ACameraCaptureSession_stateCallbacks{};
        callbacks->context = this;
        callbacks->onClosed = onClosed;
        callbacks->onReady = onReady;
        callbacks->onActive = onActive;
        return callbacks;
    }

    void CameraCaptureSession::setRepeatingRequest(const std::unique_ptr<CaptureRequest> &captureRequest) {
        std::array<ACaptureRequest *, 1> requests{captureRequest->getCaptureRequest()};

        LOG_D("ACameraCaptureSession_setRepeatingRequest()");
//        ACameraCaptureSession_captureCallbacks *callbacks = createCaptureCallbacks();
        ACameraCaptureSession_captureCallbacks *callbacks = nullptr;
//        int *id = &mCaptureSequenceId;
        int *id = nullptr;
        ACameraCaptureSession_setRepeatingRequest(mCaptureSession, callbacks, 1, requests.data(), id);
        LOG_D("ACameraCaptureSession_setRepeatingRequest(mCaptureSession:%p, requests:[%p])", mCaptureSession, requests[0]);
    }

    void CameraCaptureSession::stopRepeating() {
        ACameraCaptureSession_stopRepeating(mCaptureSession);
    }

    void CameraCaptureSession::onClosed(void *context, ACameraCaptureSession *session) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onClosed();
    }

    void CameraCaptureSession::onReady(void *context, ACameraCaptureSession *session) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onReady();
    }

    void CameraCaptureSession::onActive(void *context, ACameraCaptureSession *session) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onActive();
    }

    void CameraCaptureSession::onClosed() {
        LOG_D("CameraCaptureSession::onClosed()");
    }

    void CameraCaptureSession::onReady() {
        LOG_D("CameraCaptureSession::onReady()");
    }

    void CameraCaptureSession::onActive() {
        LOG_D("CameraCaptureSession::onActive()");
    }

    ACameraCaptureSession_captureCallbacks *CameraCaptureSession::createCaptureCallbacks() {
        ACameraCaptureSession_captureCallbacks *callbacks = new ACameraCaptureSession_captureCallbacks{};
        callbacks->context = this;

        callbacks->onCaptureStarted = onCaptureStarted;
        callbacks->onCaptureProgressed = onCaptureProgressed;
        callbacks->onCaptureCompleted = onCaptureCompleted;
        callbacks->onCaptureFailed = onCaptureFailed;
        callbacks->onCaptureSequenceCompleted = onCaptureSequenceCompleted;
        callbacks->onCaptureSequenceAborted = onCaptureSequenceAborted;
        callbacks->onCaptureBufferLost = onCaptureBufferLost;

        return callbacks;
    }

    void CameraCaptureSession::onCaptureStarted(
            void *context, ACameraCaptureSession *session,
            const ACaptureRequest *request, int64_t timestamp) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onCaptureStarted(session, request, timestamp);
    }

    void CameraCaptureSession::onCaptureProgressed(void *context, ACameraCaptureSession *session,
                                                   ACaptureRequest *request, const ACameraMetadata *result) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onCaptureProgressed(session, request, result);
    }

    void CameraCaptureSession::onCaptureCompleted(void *context, ACameraCaptureSession *session,
                                                  ACaptureRequest *request, const ACameraMetadata *result) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onCaptureCompleted(session, request, result);
    }

    void CameraCaptureSession::onCaptureFailed(void *context, ACameraCaptureSession *session,
                                               ACaptureRequest *request, ACameraCaptureFailure *failure) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onCaptureFailed(session, request, failure);
    }

    void CameraCaptureSession::onCaptureSequenceCompleted(void *context, ACameraCaptureSession *session,
                                                          int sequenceId, int64_t frameNumber) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onCaptureSequenceCompleted(session, sequenceId, frameNumber);
    }

    void CameraCaptureSession::onCaptureSequenceAborted(void *context, ACameraCaptureSession *session,
                                                        int sequenceId) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onCaptureSequenceAborted(session, sequenceId);
    }

    void CameraCaptureSession::onCaptureBufferLost(void *context, ACameraCaptureSession *session,
                                                   ACaptureRequest *request, ACameraWindowType *window, int64_t frameNumber) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onCaptureBufferLost(session, request, window, frameNumber);
    }


    void CameraCaptureSession::onCaptureStarted(
            ACameraCaptureSession *session,
            const ACaptureRequest *request, int64_t timestamp) {
        LOG_D("CameraCaptureSession::onCaptureStarted");
    }

    void CameraCaptureSession::onCaptureProgressed(ACameraCaptureSession *session,
                                                   ACaptureRequest *request, const ACameraMetadata *result) {
        LOG_D("CameraCaptureSession::onCaptureProgressed");
    }

    void CameraCaptureSession::onCaptureCompleted(ACameraCaptureSession *session,
                                                  ACaptureRequest *request, const ACameraMetadata *result) {
        LOG_D("CameraCaptureSession::onCaptureCompleted");
    }

    void CameraCaptureSession::onCaptureFailed(ACameraCaptureSession *session,
                                               ACaptureRequest *request, ACameraCaptureFailure *failure) {
        LOG_D("CameraCaptureSession::onCaptureFailed");
    }

    void CameraCaptureSession::onCaptureSequenceCompleted(ACameraCaptureSession *session,
                                                          int sequenceId, int64_t frameNumber) {
        LOG_D("CameraCaptureSession::onCaptureSequenceCompleted");
    }

    void CameraCaptureSession::onCaptureSequenceAborted(ACameraCaptureSession *session,
                                                        int sequenceId) {
        LOG_D("CameraCaptureSession::onCaptureSequenceAborted");
    }

    void CameraCaptureSession::onCaptureBufferLost(ACameraCaptureSession *session,
                                                   ACaptureRequest *request, ACameraWindowType *window, int64_t frameNumber) {
        LOG_D("CameraCaptureSession::onCaptureBufferLost");
    }

} // ndkcamera