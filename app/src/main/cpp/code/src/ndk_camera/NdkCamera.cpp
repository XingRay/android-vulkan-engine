//
// Created by leixing on 2025/1/19.
//

#include "ndk_camera/NdkCamera.h"
#include "ndk_camera/Log.h"

#include <camera/NdkCameraMetadata.h>

namespace ndkcamera {

    // sync camera / async camera
    // sync: getLatestHardwareBuffer / cleanLatestHardwareBuffer
    // async: setPreviewCallback
    NdkCamera::NdkCamera() {
        // todo: image size selector
        mImageReader = std::make_unique<ImageReader>(1080, 1920, AIMAGE_FORMAT_PRIVATE, AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE, 4);

        mCameraManager = std::make_unique<CameraManager>();
        std::vector<CameraInfo> cameraInfoList = mCameraManager->queryCameraInfoList();

        LOG_D("cameraInfoList: ");
        for (const CameraInfo &cameraInfo: cameraInfoList) {
            LOG_D("    id:%s, lensFacing:%d, supportedHardwareLevel:%d", cameraInfo.id.c_str(), cameraInfo.lensFacing, cameraInfo.supportedHardwareLevel);
        }

        mCameraDevice = std::move(mCameraManager->openCamera(cameraInfoList[0].id));

        mCaptureSessionOutputContainer = std::make_unique<CaptureSessionOutputContainer>();
        mCaptureSessionOutput = mImageReader->createCaptureSessionOutput();
        mCaptureSessionOutputContainer->add(mCaptureSessionOutput);

        mCaptureRequest = mCameraDevice->createCaptureRequest();
        mCameraOutputTarget = mImageReader->createOutputTarget();
        mCaptureRequest->addTarget(mCameraOutputTarget);

        mCaptureSession = std::move(mCameraDevice->createCaptureSession(mCaptureSessionOutputContainer, mCaptureRequest));

        mImageReader->setImageListener([&](const AImageReader *imageReader) {
            LOG_D("imageReader:%p", imageReader);
//            imageReader.
//            previewCallback();
        });
    }

    void NdkCamera::setPreviewCallback(std::function<void(NdkCamera *camera, AHardwareBuffer *hardwareBuffer)> previewCallback) {
        mPreviewCallback = previewCallback;
    }

    NdkCamera::~NdkCamera() = default;

    void NdkCamera::startPreview() {
        // 启动相机预览
        mCaptureSession->setRepeatingRequest(mCaptureRequest);
    }

    void NdkCamera::stopPreview() {
        // 停止相机预览
        mCaptureSession->stopRepeating();
    }

    AHardwareBuffer *NdkCamera::getLatestHardwareBuffer() {
        return mImageReader->getLatestHardwareBuffer();
    }

    void NdkCamera::cleanLatestHardwareBuffer() {
        mImageReader->cleanLatestHardwareBuffer();
    }

    void NdkCamera::onImageAvailable(void *context, AImageReader *reader) {
        NdkCamera *ndkCamera = static_cast<NdkCamera *>(context);
        AImage *image = nullptr;
        AImageReader_acquireLatestImage(reader, &image);

        if (image) {
            // 获取 HardwareBuffer
            AHardwareBuffer *hardwareBuffer = nullptr;
            AImage_getHardwareBuffer(image, &hardwareBuffer);

            // 将 HardwareBuffer 传递给 Vulkan 进行渲染
            ndkCamera->mPreviewCallback(ndkCamera, hardwareBuffer);
            // 释放图像
            AImage_delete(image);
        }
    }

} // android