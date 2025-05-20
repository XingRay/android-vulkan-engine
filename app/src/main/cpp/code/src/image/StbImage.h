//
// Created by leixing on 2025/4/8.
//

#pragma once

#include <memory>
#include <string>

#include "vklite/ImageInterface.h"
#include "stb_image.h"

namespace image {

    class StbImage : public vklite::ImageInterface {
    private:
        uint32_t mWidth;
        uint32_t mHeight;
        vk::Format mFormat;
        stbi_uc *mPixels;

    public:
        static std::unique_ptr<StbImage> loadImage(const char *filePath, int stbiFomrat);

        static std::unique_ptr<StbImage> loadImage(const std::string &filePath, int stbiFomrat);

        static std::unique_ptr<StbImage> loadImage(const char *filePath);

        static std::unique_ptr<StbImage> loadImage(const std::string &filePath);

        static std::unique_ptr<StbImage> loadImageAsRgba(const char *filePath);

        static std::unique_ptr<StbImage> loadImageAsRgba(const std::string &filePath);

        static std::unique_ptr<StbImage> loadImageAsRgb(const char *filePath);

        static std::unique_ptr<StbImage> loadImageAsRgb(const std::string &filePath);

        StbImage(uint32_t width, uint32_t height, uint32_t channels, stbi_uc *pixels);

        ~StbImage() override;

        uint32_t getWidth() const override;

        uint32_t getHeight() const override;

        vk::Format getFormat() const override;

        const void *getPixels() const override;

        uint32_t getPixelBytes() const override;
    };

} // test
