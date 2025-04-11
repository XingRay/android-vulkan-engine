//
// Created by leixing on 2025/1/21.
//

#include "ndk_camera/Image.h"

namespace ndkcamera {

    Image::Image(AImage *image) : mImage(image) {

    }

    Image::~Image() {
        AImage_delete(mImage);
    }
} // ndkcamera