//
// Created by leixing on 2025/1/20.
//

#include "ndk_camera/ImageReader.h"

#include "ndk_camera/Log.h"
#include "camera/NdkCameraDevice.h"

namespace ndkcamera {

    ImageReader::ImageReader(int32_t width, int32_t height, int32_t format, uint64_t usage, int32_t maxImages)
            : mWidth(width), mHeight(height), mFormat(format), mUsage(usage), mMaxImages(maxImages), mImageReader(nullptr), mImageReaderWindow(nullptr) {

        if (maxImages < 2) {
            throw std::runtime_error("Max images must >= 2");
        }

        media_status_t status = AImageReader_newWithUsage(width, height, format, usage, maxImages + 2, &mImageReader);
        if (status != AMEDIA_OK || mImageReader == nullptr) {
            LOG_E("Failed to create AImageReader");
            return;
        }

        // mImageReaderWindow 由 mImageReader 自动管理, 不需要手动释放
        status = AImageReader_getWindow(mImageReader, &mImageReaderWindow);
        if (status != AMEDIA_OK || mImageReaderWindow == nullptr) {
            LOG_E("Failed to get ANativeWindow from AImageReader");
            AImageReader_delete(mImageReader);
            return;
        }

        LOG_D("ANativeWindow obtained: %p", mImageReaderWindow);

        mImages.resize(maxImages);
        mBuffers.resize(maxImages);
        mCurrentBufferIndex = 0;

//        AImageReader_ImageListener listener = {this, onImageAvailable};
//        AImageReader_setImageListener(mImageReader, &listener);
    }

    ImageReader::~ImageReader() {
        for (AImage *image: mImages) {
            if (image != nullptr) {
                AImage_delete(image);
            }
        }
        AImageReader_delete(mImageReader);
    }

    std::unique_ptr<CaptureSessionOutput> ImageReader::createCaptureSessionOutput() {
        ACaptureSessionOutput *sessionOutput;
        LOG_D("ACaptureSessionOutput_create()");
        camera_status_t status = ACaptureSessionOutput_create(mImageReaderWindow, &sessionOutput);
        LOG_D("ACaptureSessionOutput_create(mImageReaderWindow:%p, sessionOutput:%p)", mImageReaderWindow, sessionOutput);

        if (status != ACAMERA_OK || sessionOutput == nullptr) {
            LOG_E("Failed to create CaptureSessionOutput from ImageReaderWindow");
            return nullptr;
        }

        return std::make_unique<CaptureSessionOutput>(sessionOutput);
    }

    std::unique_ptr<CameraOutputTarget> ImageReader::createOutputTarget() {
        ACameraOutputTarget *outputTarget;
        LOG_D("ACameraOutputTarget_create()");
        camera_status_t status = ACameraOutputTarget_create(mImageReaderWindow, &outputTarget);
        LOG_D("ACameraOutputTarget_create(mImageReaderWindow:%p, outputTarget:%p)", mImageReaderWindow, outputTarget);
        if (status != ACAMERA_OK || outputTarget == nullptr) {
            LOG_E("Failed to create CameraOutputTarget from ImageReaderWindow");
            return nullptr;
        }
        return std::make_unique<CameraOutputTarget>(outputTarget);
    }

    void ImageReader::setImageListener(std::function<void(const AImageReader *reader)> &&imageListener) {
        mImageListener = std::move(imageListener);
    }

    void ImageReader::onImageAvailable(void *context, AImageReader *reader) {
        ImageReader *imageReader = static_cast<ImageReader *>(context);
        LOG_D("static ImageReader::onImageAvailable, reader:%p", reader);
        imageReader->onImageAvailable();
    }

    void ImageReader::onImageAvailable() {
        LOG_D("ImageReader::onImageAvailable, mImageReader:%p", mImageReader);
        mImageListener(mImageReader);
    }

    AHardwareBuffer *ImageReader::getLatestHardwareBuffer() {
        AImage *image = nullptr;
        media_status_t status = AImageReader_acquireLatestImage(mImageReader, &image);
        if (status != AMEDIA_OK || image == nullptr) {
            return nullptr; // 获取图像失败，返回空指针
        }

        AHardwareBuffer *buffer = nullptr;
        status = AImage_getHardwareBuffer(image, &buffer);
        if (status != AMEDIA_OK || buffer == nullptr) {
            AImage_delete(image);                       // 获取硬件缓冲区失败，释放图像
            return nullptr;
        }

        // 更新当前缓冲区
        mCurrentBufferIndex = (mCurrentBufferIndex + 1) % mBuffers.size();
        if (mImages[mCurrentBufferIndex]) {
            AImage_delete(mImages[mCurrentBufferIndex]); // 释放旧的图像
        }
        mImages[mCurrentBufferIndex] = image;           // 保存新的图像
        mBuffers[mCurrentBufferIndex] = buffer;         // 保存新的硬件缓冲区

        return buffer;
    }

    void ImageReader::cleanLatestHardwareBuffer() {
        AImage *image = mImages[mCurrentBufferIndex];
        if (image != nullptr) {
            AImage_delete(image);
            mImages[mCurrentBufferIndex] = nullptr;
            mBuffers[mCurrentBufferIndex] = nullptr;
        }
    }

} // ndkcamera