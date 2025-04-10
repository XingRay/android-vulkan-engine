//
// Created by leixing on 2025/4/8.
//

#include "image/StbImage.h"

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
            : mWidth(width), mHeight(height), mPixels(pixels) {
        if (channels == 3) {
            mFormat = vk::Format::eR8G8B8Srgb;
        } else if (channels == 4) {
            mFormat = vk::Format::eR8G8B8A8Srgb;
        } else {
            throw std::runtime_error("unknown format");
        }
    }

    StbImage::~StbImage() {
        stbi_image_free(mPixels);
    }

    uint32_t StbImage::getWidth() const {
        return mWidth;
    }

    uint32_t StbImage::getHeight() const {
        return mHeight;
    }

    vk::Format StbImage::getFormat() const {
        return mFormat;
    }

    const void *StbImage::getPixels() const {
        return mPixels;
    }

} // test