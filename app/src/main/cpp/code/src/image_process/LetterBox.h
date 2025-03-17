//
// Created by leixing on 2025/2/26.
//

#pragma once

#include <memory>
#include "opencv2/opencv.hpp"

namespace image {

    struct PaddingParams {
        float scale;
        int pad_top;
        int pad_bottom;
        int pad_left;
        int pad_right;
        int original_w;
        int original_h;
        int padded_w;
        int padded_h;
    };

    PaddingParams calcLetterbox(int h, int w, int target_h, int target_w);

    // 将一组关键点坐标从 padded 图像转换回原始图像坐标
    std::vector<cv::Point> transform_keypoints(const std::vector<cv::Point2f> &keypoints, const PaddingParams &params);

} // image
