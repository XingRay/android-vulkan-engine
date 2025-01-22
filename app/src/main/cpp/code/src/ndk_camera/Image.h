//
// Created by leixing on 2025/1/21.
//

#pragma once

#include <media/NdkImage.h>

namespace ndkcamera {

    class Image {
    private:
        AImage* mImage;

    public:
        explicit Image(AImage* image);

        ~Image();
    };

} // ndkcamera
