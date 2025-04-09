//
// Created by leixing on 2025/4/8.
//

#include "StbImage.h"

namespace image {

    std::unique_ptr<StbImage> StbImage::loadImage(const std::string &filePath) {
        return loadImage(filePath.c_str());
    }

    std::unique_ptr<StbImage> StbImage::loadImage(const char *filePath) {
        int width, height, channels;
        stbi_uc *pixels = stbi_load(filePath, &width, &height, &channels, STBI_rgb_alpha);
        if (!pixels) {
            throw std::runtime_error("Failed to load image!");
        }
        return std::make_unique<StbImage>(width, height, channels, pixels);
    }

    StbImage::StbImage(uint32_t width, uint32_t height, uint32_t channels, stbi_uc *pixels)
            : mWidth(width), mHeight(height), mChannels(channels), mPixels(pixels) {}

    StbImage::~StbImage() {
        stbi_image_free(mPixels);
    }

    uint32_t StbImage::getWidth() const {
        return mWidth;
    }

    uint32_t StbImage::getHeight() const {
        return mHeight;
    }

    uint32_t StbImage::getChannels() const {
        return mChannels;
    }

    const void *StbImage::getPixels() const {
        return mPixels;
    }

} // test