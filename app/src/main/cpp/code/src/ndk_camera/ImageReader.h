//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <memory>
#include <functional>

#include <media/NdkImageReader.h>

#include "ndk_camera/CaptureSessionOutput.h"
#include "ndk_camera/CameraOutputTarget.h"
#include "ndk_camera/Image.h"

namespace ndkcamera {

    class ImageReader {
    private:
        int32_t mWidth;

        int32_t mHeight;

        int32_t mFormat;

        uint64_t mUsage;

        int32_t mMaxImages;

        AImageReader *mImageReader;

        ANativeWindow *mImageReaderWindow;

        std::function<void(const AImageReader *reader)> mImageListener;

        uint32_t mCurrentBufferIndex;
        std::vector<AHardwareBuffer *> mBuffers;
//        std::vector<std::unique_ptr<Image>> mImages;
        std::vector<AImage*> mImages;

    public:
        ImageReader(int32_t width, int32_t height, int32_t format, uint64_t usage, int32_t maxImages);

        ~ImageReader();

        std::unique_ptr<CaptureSessionOutput> createCaptureSessionOutput();

        std::unique_ptr<CameraOutputTarget> createOutputTarget();

        void setImageListener(std::function<void(const AImageReader *reader)> &&imageListener);

        AHardwareBuffer *getLatestHardwareBuffer();

        void cleanLatestHardwareBuffer();

    private:
        static void onImageAvailable(void *context, AImageReader *reader);

        void onImageAvailable();
    };

} // ndkcamera
