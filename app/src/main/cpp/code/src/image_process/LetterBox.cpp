//
// Created by leixing on 2025/2/26.
//

#include "LetterBox.h"
#include <algorithm>

namespace image {

    PaddingParams calcLetterbox(int h, int w, int target_h, int target_w) {
        float scale = std::min((float) target_w / w, (float) target_h / h);
        int new_w = std::round(w * scale);
        int new_h = std::round(h * scale);

        int pad_w = (target_w - new_w) / 2;
        int pad_h = (target_h - new_h) / 2;
        int pad_right = target_w - new_w - pad_w;
        int pad_bottom = target_h - new_h - pad_h;

        PaddingParams params{};
        params.scale = scale;
        params.pad_top = pad_h;
        params.pad_bottom = pad_bottom;
        params.pad_left = pad_w;
        params.pad_right = pad_right;
        params.original_w = w;
        params.original_h = h;
        params.padded_w = target_w;
        params.padded_h = target_h;

        return params;
    }

} // image