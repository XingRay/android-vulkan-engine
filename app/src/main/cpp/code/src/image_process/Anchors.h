//
// Created by leixing on 2025/2/26.
//

#pragma once

#include <vector>

namespace image {

    struct Anchor {
        float x_center; // 归一化坐标 [0,1]
        float y_center; // 归一化坐标 [0,1]
        float w; // 归一化宽度 [0,1]
        float h; // 归一化高度 [0,1]
    };

    std::vector<Anchor> generate_face_detection_anchors(int input_size/*= 128*/);

} // image
