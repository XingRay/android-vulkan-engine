//
// Created by leixing on 2025/2/26.
//

#include "image_process/Anchors.h"

namespace image {

    float calculate_scale(float min_scale, float max_scale, int stride_index, int num_strides) {
        if (num_strides == 1) {
            return (min_scale + max_scale) * 0.5f;
        } else {
            return min_scale + (max_scale - min_scale) * stride_index / (num_strides - 1.0f);
        }
    }

    std::vector<Anchor> generate_face_detection_anchors(int input_size/*= 128*/) {
        int num_layers = 4;
        float min_scale = 0.1484375f; // 19/128
        float max_scale = 0.75f;
        int input_size_height = input_size;
        int input_size_width = input_size;
        float anchor_offset_x = 0.5f;
        float anchor_offset_y = 0.5f;
        std::vector<int> strides = {8, 16, 16, 16};
        std::vector<float> aspect_ratios = {1.0f};
        bool fixed_anchor_size = true;
        float interpolated_scale_aspect_ratio = 1.0f;
        bool reduce_boxes_in_lowest_layer = false;

        std::vector<Anchor> anchors;
        int layer_id = 0;
        int num_strides = strides.size();

        while (layer_id < num_layers) {
            std::vector<float> anchor_heights;
            std::vector<float> anchor_widths;
            std::vector<float> aspect_ratios_layer;
            std::vector<float> scales_layer;

            int last_same_stride_layer = layer_id;
            while (last_same_stride_layer < num_strides && strides[last_same_stride_layer] == strides[layer_id]) {
                int current_stride_index = last_same_stride_layer;
                float scale = calculate_scale(min_scale, max_scale, current_stride_index, num_strides);
                if (current_stride_index == 0 && reduce_boxes_in_lowest_layer) {
                    aspect_ratios_layer.push_back(1.0f);
                    aspect_ratios_layer.push_back(2.0f);
                    aspect_ratios_layer.push_back(0.5f);
                    scales_layer.push_back(0.1f);
                    scales_layer.push_back(scale);
                    scales_layer.push_back(scale);
                } else {
                    for (float ratio: aspect_ratios) {
                        aspect_ratios_layer.push_back(ratio);
                        scales_layer.push_back(scale);
                    }
                    if (interpolated_scale_aspect_ratio > 0) {
                        float scale_next = (current_stride_index == num_strides - 1)
                                           ? 1.0f
                                           : calculate_scale(min_scale, max_scale, current_stride_index + 1, num_strides);
                        scales_layer.push_back(std::sqrt(scale * scale_next));
                        aspect_ratios_layer.push_back(interpolated_scale_aspect_ratio);
                    }
                }
                last_same_stride_layer++;
            }

            for (size_t i = 0; i < aspect_ratios_layer.size(); i++) {
                float ratio_sqrt = std::sqrt(aspect_ratios_layer[i]);
                anchor_heights.push_back(scales_layer[i] / ratio_sqrt);
                anchor_widths.push_back(scales_layer[i] * ratio_sqrt);
            }

            int stride = strides[layer_id];
            int feature_map_height = (int) std::ceil((float) input_size_height / stride);
            int feature_map_width = (int) std::ceil((float) input_size_width / stride);

            for (int y = 0; y < feature_map_height; y++) {
                for (int x = 0; x < feature_map_width; x++) {
                    for (size_t anchor_id = 0; anchor_id < anchor_heights.size(); anchor_id++) {
                        float x_center = (x + anchor_offset_x) / feature_map_width;
                        float y_center = (y + anchor_offset_y) / feature_map_height;
                        float w, h;
                        if (fixed_anchor_size) {
                            w = 1.0f;
                            h = 1.0f;
                        } else {
                            w = anchor_widths[anchor_id];
                            h = anchor_heights[anchor_id];
                        }
                        anchors.push_back({x_center, y_center, w, h});
                    }
                }
            }
            layer_id = last_same_stride_layer;
        }
        return anchors;
    }
} // image