//
// Created by leixing on 2025/1/19.
//

#include "NdkCamera.h"
#include "engine/Log.h"

#include <camera/NdkCameraMetadata.h>

namespace ndkcamera {

    NdkCamera::NdkCamera(std::function<void(NdkCamera *camera, AHardwareBuffer *hardwareBuffer)> previewCallback) : mPreviewCallback(previewCallback) {
        // 初始化相机管理器
        mCameraManager = std::make_unique<CameraManager>();
        std::vector<CameraInfo> cameraInfoList = mCameraManager->queryCameraInfoList();

        LOG_D("cameraInfoList: ");
        for (const CameraInfo &cameraInfo: cameraInfoList) {
            LOG_D("    id:%s, lensFacing:%d, supportedHardwareLevel:%d", cameraInfo.id.c_str(), cameraInfo.lensFacing, cameraInfo.supportedHardwareLevel);
        }

        mCameraManager->openCamera(cameraInfoList[0].id);

//        // 打开第一个相机设备
//        ACameraManager_openCamera(mCameraManager, cameraIdList->cameraIds[0], nullptr, &mCameraDevice);
//
//        // 释放相机设备列表
//        ACameraManager_deleteCameraIdList(cameraIdList);
//
//        // 创建相机捕获会话
//        ACaptureSessionOutputContainer_create(&mOutputContainer);
//
//        ACameraDevice_createCaptureSession(mCameraDevice, mOutputContainer, nullptr, &mCaptureSession);
//
//        // 创建 ImageReader
//
//        int32_t width = 1920;  // 相机帧宽度
//        int32_t height = 1080; // 相机帧高度
//        int32_t format = AIMAGE_FORMAT_YUV_420_888; // 相机帧格式
//        int32_t maxImages = 2; // 最大缓冲帧数
//        AImageReader_new(width, height, format, maxImages, &mImageReader);
//
//        // 获取 ImageReader 的窗口
//        ANativeWindow *imageReaderWindow = nullptr;
//        AImageReader_getWindow(mImageReader, &imageReaderWindow);
//
//        // 将 ImageReader 的输出添加到捕获会话
//
//        ACaptureSessionOutput_create(imageReaderWindow, &mSessionOutput);
//        ACaptureSessionOutputContainer_add(mOutputContainer, mSessionOutput);
//
//        // 创建捕获请求
//        ACameraDevice_createCaptureRequest(mCameraDevice, TEMPLATE_PREVIEW, &mCaptureRequest);
//
//        // 将 ImageReader 的输出目标添加到捕获请求
//
//        ACameraOutputTarget_create(imageReaderWindow, &mOutputTarget);
//        ACaptureRequest_addTarget(mCaptureRequest, mOutputTarget);
//
//        // 设置 ImageListener
//        AImageReader_ImageListener listener = {this, onImageAvailable};
//        AImageReader_setImageListener(mImageReader, &listener);
    }


    NdkCamera::~NdkCamera() {
        // 释放相机资源
//        ACameraOutputTarget_free(mOutputTarget);
//        ACaptureRequest_free(mCaptureRequest);
//        ACaptureSessionOutput_free(mSessionOutput);
//        AImageReader_delete(mImageReader);
//        ACameraCaptureSession_close(mCaptureSession);
//        ACaptureSessionOutputContainer_free(mOutputContainer);
//
//        ACameraDevice_close(mCameraDevice);
//        ACameraManager_delete(mCameraManager);
    }

    void NdkCamera::startPreview() {
        // 启动相机预览
        ACameraCaptureSession_setRepeatingRequest(mCaptureSession, nullptr, 1, &mCaptureRequest, nullptr);
    }

    void NdkCamera::stopPreview() {
        // 停止相机预览
        ACameraCaptureSession_stopRepeating(mCaptureSession);

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