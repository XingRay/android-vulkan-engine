//
// Created by leixing on 2025/1/4.
//

#include "Test09NdkCameraFaceDetection.h"
#include "FileUtil.h"

#include <chrono>
#include <thread>
#include <filesystem>

#include <opencv2/opencv.hpp>


#include "ncnn/net.h"

#include "MNN/Interpreter.hpp"

namespace std {

    template<>
    struct hash<test09::Vertex> {
        size_t operator()(test09::Vertex const &vertex) const {
            size_t seed = 0;

            // 哈希 position
            hash<glm::vec3> vec3Hash;
            seed ^= vec3Hash(vertex.position) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            // 哈希 uv
            hash<glm::vec2> vec2Hash;
            seed ^= vec2Hash(vertex.uv) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
}


namespace test09 {


    struct Anchor {
        float x_center; // 归一化坐标 [0,1]
        float y_center; // 归一化坐标 [0,1]
        float w; // 归一化宽度 [0,1]
        float h; // 归一化高度 [0,1]
    };

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

// ---------------------- 工具函数 ----------------------

// 计算 scale，与 Python 中的 calculate_scale 函数相同
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

// 绘制检测结果：在 image 上绘制 bbox 和关键点，返回绘制后的图像
    cv::Mat draw_detection(const cv::Mat &image, const cv::Rect &bbox, const std::vector<cv::Point> &keypoints) {
        cv::Mat out_img = image.clone();
        cv::rectangle(out_img, bbox, cv::Scalar(0, 255, 0), 2);
        for (const auto &pt: keypoints) {
            cv::circle(out_img, pt, 2, cv::Scalar(0, 0, 255), -1);
        }
        return out_img;
    }

// letterbox_padding: 将图像 resize 保持比例，然后填充黑边，返回填充后的图像和参数
    cv::Mat letterbox_padding(const cv::Mat &image, const cv::Size &target_size, PaddingParams &params) {
        int h = image.rows, w = image.cols;
        int target_h = target_size.height, target_w = target_size.width;
        float scale = std::min((float) target_w / w, (float) target_h / h);
        int new_w = std::round(w * scale);
        int new_h = std::round(h * scale);
        cv::Mat resized;
        cv::resize(image, resized, cv::Size(new_w, new_h));

        int pad_w = (target_w - new_w) / 2;
        int pad_h = (target_h - new_h) / 2;
        int pad_right = target_w - new_w - pad_w;
        int pad_bottom = target_h - new_h - pad_h;

        params.scale = scale;
        params.pad_top = pad_h;
        params.pad_bottom = pad_bottom;
        params.pad_left = pad_w;
        params.pad_right = pad_right;
        params.original_w = w;
        params.original_h = h;
        params.padded_w = target_w;
        params.padded_h = target_h;

        cv::Mat padded;
        cv::copyMakeBorder(resized, padded, pad_h, pad_bottom, pad_w, pad_right, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
        return padded;
    }

// 将检测框坐标从 padded 图像转换回原始图像坐标
    void transform_coords_back(float x, float y, const PaddingParams &params, float &x_orig, float &y_orig) {
        float x_unpad = x - params.pad_left;
        float y_unpad = y - params.pad_top;
        x_orig = x_unpad / params.scale;
        y_orig = y_unpad / params.scale;
        x_orig = std::min(std::max(x_orig, 0.0f), (float) params.original_w);
        y_orig = std::min(std::max(y_orig, 0.0f), (float) params.original_h);
    }

// 将一组关键点坐标从 padded 图像转换回原始图像坐标
//    std::vector<cv::Point> transform_keypoints(const std::vector<cv::Point2f> &keypoints, const PaddingParams &params) {
//        std::vector<cv::Point> pts;
//        for (const auto &kp: keypoints) {
//            float x, y;
//            transform_coords_back(kp.x, kp.y, params, x, y);
//            pts.push_back(cv::Point(std::round(x), std::round(y)));
//        }
//        return pts;
//    }

    void print_ncnn_mat(const ncnn::Mat &mat, const char *name) {
        std::string output = std::string(name) + " = [";

        for (int c = 0; c < mat.c; c++) {
            const float *ptr = mat.channel(c);
            for (int y = 0; y < mat.h; y++) {
                for (int x = 0; x < mat.w; x++) {
                    output += std::to_string(ptr[y * mat.w + x]) + " ";
                }
                output += "\n";
            }
            output += ";\n"; // 频道分隔符
        }
        output += "]\n";

        LOG_D("%s", output.c_str());
    }

    void print_ncnn_mat_shape(const ncnn::Mat &mat, const char *name) {
        LOG_D("%s shape: c=%d, d=%d, h=%d, w=%d, dims=%d", name, mat.c, mat.d, mat.h, mat.w, mat.dims);
    }

    void save_mat_to_txt(const ncnn::Mat &mat, const char *path) {
        // 确保父目录存在
        std::filesystem::path file_path(path);
        std::filesystem::create_directories(file_path.parent_path());

        // 以写模式打开文件，若文件存在则覆盖
        FILE *fp = fopen(path, "w");
        if (!fp) {
            std::cerr << "无法打开文件: " << path << std::endl;
            return;
        }

        const int dims = mat.dims;
        if (dims == 0) {
            // 空 Mat
            fclose(fp);
            return;
        }

        if (dims == 1) {
            // 一维数据，逐行写入
            for (int i = 0; i < mat.w; ++i) {
                fprintf(fp, "%.9g\n", mat[i]);
            }
        } else if (dims == 2) {
            // 二维矩阵，按行保存
            for (int h = 0; h < mat.h; ++h) {
                const float *row = mat.row(h);
                for (int w = 0; w < mat.w; ++w) {
                    fprintf(fp, "%.9g ", row[w]);
                }
                fprintf(fp, "\n");
            }
        } else {
            // 三维及以上，按通道、行、列保存
            for (int c = 0; c < mat.c; ++c) {
                const float *channel = mat.channel(c);
                for (int h = 0; h < mat.h; ++h) {
                    const float *row = channel + h * mat.w;
                    for (int w = 0; w < mat.w; ++w) {
                        fprintf(fp, "%.9g ", row[w]);
                    }
                    fprintf(fp, "\n");
                }
                fprintf(fp, "\n"); // 通道间空行分隔
            }
        }

        fclose(fp);
    }

    std::optional<std::pair<std::vector<float>, std::vector<float> > > ncnn_inference(const cv::Mat &padded, const cv::Mat &padded_float);

    std::optional<std::pair<std::vector<float>, std::vector<float> > > mnn_inference(const cv::Mat &padded, const cv::Mat &padded_float);

    void faceDetectionPostProcess(const std::vector<float> &reg_vec,
                                  const std::vector<float> &score_vec,
                                  const cv::Mat &originalImg,
                                  const PaddingParams &padding_params,
                                  const std::string &output_img_path);


    Test09NdkCameraFaceDetection::Test09NdkCameraFaceDetection(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        mNdkCamera = std::make_unique<ndkcamera::NdkCamera>();
        mNdkCamera->startPreview();

        AHardwareBuffer *hardwareBuffer = nullptr;
        while (hardwareBuffer == nullptr) {
            LOG_D("waiting for getLatestHardwareBuffer...");
            hardwareBuffer = mNdkCamera->getLatestHardwareBuffer();
        }

        std::vector<std::string> instanceExtensions = {
                VK_KHR_SURFACE_EXTENSION_NAME,
                VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,

                // old version
                VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
                // new version
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,

                VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME,
                VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES_EXTENSION_NAME,
                VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
        };

        std::vector<std::string> layers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::vector<std::string> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                VK_KHR_MAINTENANCE1_EXTENSION_NAME,
                VK_KHR_BIND_MEMORY_2_EXTENSION_NAME,
                VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
                VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME,
                VK_EXT_QUEUE_FAMILY_FOREIGN_EXTENSION_NAME,
                VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME,
                VK_KHR_EXTERNAL_SEMAPHORE_EXTENSION_NAME,
                VK_KHR_EXTERNAL_SEMAPHORE_FD_EXTENSION_NAME,
                VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
                VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME
        };

        std::vector<char> vertexShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/07_ndk_camera.vert.spv");
        std::vector<char> fragmentShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/07_ndk_camera.frag.spv");

//        std::unique_ptr<vklite::VulkanGraphicsEngine> engine = vklite::VkLiteEngineBuilder{}
//                .layers({}, layers)
//                .extensions({}, instanceExtensions)
//                .asGraphics()
//                .deviceExtensions(std::move(deviceExtensions))
//                .surface(vklite::AndroidVulkanSurface::surfaceBuilder(mApp.window))
//                .enableMsaa(1)
//                .physicalDeviceAsDefault()
//                .shader([&](vklite::VulkanShaderConfigure &shaderConfigure) {
//                    shaderConfigure
//                            .vertexShaderCode(std::move(vertexShaderCode))
//                            .fragmentShaderCode(std::move(std::move(fragmentShaderCode)))
//                            .vertex([](vklite::VulkanVertexConfigure &vertexConfigure) {
//                                vertexConfigure
//                                        .binding(0)
//                                        .size(sizeof(Vertex))
//                                        .addAttribute(ShaderFormat::Vec3)
//                                        .addAttribute(ShaderFormat::Vec2);
//                            })
//                            .uniformSet([=](vklite::VulkanDescriptorSetConfigure &configure) {
//                                configure
//                                        .set(0)
//                                        .addAndroidHardwareBufferSampler(0, vk::ShaderStageFlagBits::eFragment, hardwareBuffer);
//                            });
//                })
//                .build();
//
//        mVkLiteEngine = std::move(engine);

        mMnnDetector = std::make_unique<detector::MnnDetector>();
    }

    void Test09NdkCameraFaceDetection::init() {

        // x轴朝右, y轴朝下, z轴朝前, 右手系 (x,y)->z
        std::vector<Vertex> vertices = {
                {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}, // 左上角
                {{1.0f,  -1.0f, 0.0f}, {0.0f, 0.0f}}, // 右上角
                {{-1.0f, 1.0f,  0.0f}, {1.0f, 1.0f}}, // 左下角
                {{1.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // 右下角
        };

        std::vector<uint32_t> indices = {0, 2, 1, 1, 2, 3};

//        LOG_D("mVkLiteEngine->createStagingTransferVertexBuffer");
//        mVkLiteEngine->createStagingTransferVertexBuffer(vertices.size() * sizeof(Vertex));
//
//        LOG_D("mVkLiteEngine->updateVertexBuffer");
//        mVkLiteEngine->updateVertexBuffer(vertices);
//
//        LOG_D("mVkLiteEngine->createStagingTransferIndexBuffer");
//        mVkLiteEngine->createStagingTransferIndexBuffer(indices.size() * sizeof(uint32_t));
//        LOG_D("mVkLiteEngine->updateIndexBuffer");
//        mVkLiteEngine->updateIndexBuffer(indices);


        std::string original_img_path = R"(/storage/emulated/0/test/image/face_image_1080_1920.png)";

        // 加载图像
        cv::Mat originalImg = cv::imread(original_img_path, cv::IMREAD_UNCHANGED);
        if (originalImg.empty()) {
            LOG_E("图片未找到: %s", original_img_path.c_str());
            return;
        }

        // 转换通道：如果图像有 4 通道，转换为 RGB；否则从 BGR 转换为 RGB
        if (originalImg.channels() == 4) {
            LOG_D("COLOR_BGRA2RGB");
            cv::cvtColor(originalImg, originalImg, cv::COLOR_BGRA2RGB);
        } else {
            LOG_D("COLOR_BGR2RGB");
            cv::cvtColor(originalImg, originalImg, cv::COLOR_BGR2RGB);
        }

        // 1. letterbox处理后得到 padded 图像，尺寸为 128x128，格式为 RGB
        PaddingParams padding_params{};
        cv::Mat padded = letterbox_padding(originalImg, cv::Size(128, 128), padding_params);

        cv::Mat padded_float;
        padded.convertTo(padded_float, CV_32FC3, 1.0 / 255.0);

        auto ncnn_result = ncnn_inference(padded, padded_float);
        if (ncnn_result != std::nullopt) {
            faceDetectionPostProcess(ncnn_result->first, ncnn_result->second, originalImg, padding_params, R"(/storage/emulated/0/test/output/face_detector_ncnn.png)");
        }


        auto mnn_result = mnn_inference(padded, padded_float);
        if (mnn_result != std::nullopt) {
            faceDetectionPostProcess(mnn_result->first, mnn_result->second, originalImg, padding_params, R"(/storage/emulated/0/test/output/face_detector_mnn.png)");
        }


        auto mnn_detector_result = mMnnDetector->detect((float *) padded_float.data);
        if (mnn_detector_result != std::nullopt) {
            faceDetectionPostProcess(mnn_detector_result->first, mnn_detector_result->second, originalImg, padding_params, R"(/storage/emulated/0/test/output/face_detector_mnn_detector.png)");
        }
    }

    // 检查是否准备好
    bool Test09NdkCameraFaceDetection::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test09NdkCameraFaceDetection::drawFrame() {
        // 静态变量用于帧率统计
        static auto startTime = std::chrono::steady_clock::now(); // 统计开始时间
        static int frameCount = 0;                               // 帧计数器


//        LOG_D("Test07NdkCamera::drawFrame()");
        AHardwareBuffer *buffer = mNdkCamera->getLatestHardwareBuffer();
//        LOG_D("AHardwareBuffer:%p", buffer);
        if (buffer != nullptr) {
//            mVkLiteEngine->updateUniformBuffer(mVkLiteEngine->getCurrentFrameIndex(), 0, 0, buffer, 0);
//            mVkLiteEngine->drawFrame();

            // 增加帧计数器
            frameCount++;

            // 计算时间差
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

            // 每 5 秒输出一次帧率
            if (elapsedTime >= 5) {
                float fps = static_cast<float>(frameCount) / elapsedTime; // 计算帧率
                LOG_D("FPS: %.2f", fps);                                 // 输出帧率

                // 重置统计
                startTime = currentTime;
                frameCount = 0;
            }
        }

        mNdkCamera->cleanLatestHardwareBuffer();
    }

    // 清理操作
    void Test09NdkCameraFaceDetection::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
//        mVkLiteEngine.reset();
    }


    std::optional<std::pair<std::vector<float>, std::vector<float> > > ncnn_inference(const cv::Mat &padded, const cv::Mat &padded_float) {

        LOG_D("NCNN_VERSION: %s", NCNN_VERSION_STRING);

        bool useGpu = false;
        bool useDebugParam = false;

        // 文件路径配置
        std::string param_path;
        if (useDebugParam) {
            param_path = R"(/storage/emulated/0/test/face_detection/face_detector.ncnn_debug.param)";
        } else {
            param_path = R"(/storage/emulated/0/test/face_detection/face_detector.ncnn.param)";
        }
        std::string bin_path = R"(/storage/emulated/0/test/face_detection/face_detector.ncnn.bin)";

        ncnn::Mat mat_in;
        if (useDebugParam) {
            mat_in = ncnn::Mat::from_pixels(padded.data, ncnn::Mat::PIXEL_RGB, padded.cols, padded.rows);
            const float norm_vals[3] = {1 / 255.f, 1 / 255.f, 1 / 255.f};
            mat_in.substract_mean_normalize(0, norm_vals);
            mat_in.dims = 4;
        } else {
            mat_in = ncnn::Mat(3, 128, 128, 1, padded_float.data);
        }
        print_ncnn_mat_shape(mat_in, "mat_in");

        ncnn::Net net;
        if (useGpu) {
            int gpu_count = ncnn::get_gpu_count();
            LOG_D("gpu_count:%d", gpu_count);
            if (gpu_count <= 0) {
                LOG_E("gpu_count<=0");
                return std::nullopt;
            }

            LOG_D("use_vulkan_compute");
            net.opt.use_vulkan_compute = true;

            // set specified vulkan device before loading param and model
            // net.set_vulkan_device(0); // use device-0

            net.opt.use_fp16_packed = false;
            net.opt.use_fp16_storage = false;
            net.opt.use_fp16_arithmetic = false;
            net.opt.use_int8_storage = false;
            net.opt.use_int8_arithmetic = false;
        }

        LOG_I("load_param: %s", param_path.c_str());
        if (net.load_param(param_path.c_str()) != 0) {
            LOG_E("加载 param 文件失败");
            return std::nullopt;
        }
        LOG_I("load_model: %s", bin_path.c_str());
        if (net.load_model(bin_path.c_str()) != 0) {
            LOG_E("加载 bin 文件失败");
            return std::nullopt;
        }

        ncnn::Extractor ex = net.create_extractor();
        // 设置输入节点名称为 "in0"
        LOG_D("ex.input");
        ex.input("in0", mat_in);

        // 执行推理，提取输出 "out0" 和 "out1"
        LOG_D("ex.extract");
        ncnn::Mat regressors, scores;
        ex.extract("out0", regressors);
        ex.extract("out1", scores);
        print_ncnn_mat_shape(regressors, "regressors");
        print_ncnn_mat_shape(scores, "scores");

        int num_regressors = regressors.w * regressors.h * regressors.c; // 896*16
        int num_scores = scores.w * scores.h * scores.c; // 896

        std::vector<float> reg_vec((float *) regressors.data, (float *) regressors.data + num_regressors);
        std::vector<float> score_vec((float *) scores.data, (float *) scores.data + num_scores);

        return std::pair<std::vector<float>, std::vector<float> >(score_vec, reg_vec);
    }

    std::optional<std::pair<std::vector<float>, std::vector<float> > > mnn_inference(const cv::Mat &padded, const cv::Mat &padded_float) {

        LOG_D("MNN::getVersion(): %s", MNN::getVersion());

        LOG_D("create mnn net");
        std::string mnn_model_path = R"(/storage/emulated/0/test/face_detection/face_detector.mnn)";
        std::shared_ptr<MNN::Interpreter> mnn_net = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(mnn_model_path.c_str()));

        LOG_D("create mnn session");
        MNN::ScheduleConfig config;
        config.numThread = 2;
        config.backupType = MNN_FORWARD_VULKAN;
        MNN::BackendConfig backendConfig;
        backendConfig.memory = MNN::BackendConfig::Memory_Normal; // 内存
        backendConfig.power = MNN::BackendConfig::Power_Normal; // 功耗
        backendConfig.precision = MNN::BackendConfig::PrecisionMode::Precision_Normal; // 精度
        config.backendConfig = &backendConfig;
        MNN::Session *session = mnn_net->createSession(config);

        // input和output是参于session的计算的输入和输出
        std::string input_name = "input";
        MNN::Tensor *input_tensor = mnn_net->getSessionInput(session, input_name.c_str());

        std::string output_name_classificators = "classificators";
        MNN::Tensor *output_classificators = mnn_net->getSessionOutput(session, output_name_classificators.c_str());

        std::string output_name_regressors = "regressors";
        MNN::Tensor *output_regressors = mnn_net->getSessionOutput(session, output_name_regressors.c_str());

        // 复制数据到 MNN 的输入 Tensor
        LOG_D("Copying data to MNN input tensor...");
        auto input_dims = input_tensor->shape();
        int input_width = input_dims[2]; // 宽度
        int input_height = input_dims[1]; // 高度
        int input_channels = input_dims[3]; // 通道
        LOG_D("input_channels:%d, input_height:%d, input_width:%d", input_channels, input_height, input_width);

        MNN::Tensor *input_host = new MNN::Tensor(input_tensor, MNN::Tensor::TENSORFLOW);
        float *input_data = input_host->host<float>();
        memcpy(input_data, padded_float.data, sizeof(float) * input_width * input_height * input_channels);
        // int index = 0;
        // for (int c = 0; c < input_channels; c++) {
        //     for (int h = 0; h < input_height; h++) {
        //         for (int w = 0; w < input_width; w++) {
        //             input_data[index++] = padded_float.at<cv::Vec3f>(c, h)[w];
        //         }
        //     }
        // }

        input_tensor->copyFromHostTensor(input_host);
        delete input_host;

        // 执行前向推理
        LOG_D("Running inference...");
        mnn_net->runSession(session);

        // 提取 MNN 推理的输出张量
        LOG_D("Extracting MNN outputs...");
        MNN::Tensor *output_classificators_host = new MNN::Tensor(output_classificators, MNN::Tensor::TENSORFLOW);
        MNN::Tensor *output_regressors_host = new MNN::Tensor(output_regressors, MNN::Tensor::TENSORFLOW);
        output_classificators->copyToHostTensor(output_classificators_host);
        output_regressors->copyToHostTensor(output_regressors_host);

        float *class_data = output_classificators_host->host<float>();
        float *reg_data = output_regressors_host->host<float>();

        int mnn_num_scores = output_classificators_host->elementSize();
        int mnn_num_regressors = output_regressors_host->elementSize();

        std::vector<float> mnn_score_vec(class_data, class_data + mnn_num_scores);
        std::vector<float> mnn_reg_vec(reg_data, reg_data + mnn_num_regressors);

        LOG_D("mnn Inference completed!");

        return std::pair<std::vector<float>, std::vector<float> >(mnn_score_vec, mnn_reg_vec);
    }

    void faceDetectionPostProcess(const std::vector<float> &score_vec,
                                  const std::vector<float> &reg_vec,
                                  const cv::Mat &originalImg,
                                  const PaddingParams &padding_params,
                                  const std::string &output_img_path) {
        std::vector<float> sigmoidScore;
        sigmoidScore.reserve(score_vec.size());
        // 对 score_vec 执行 clip(-100,100) 并计算 sigmoid
        for (auto &s: score_vec) {
            sigmoidScore.push_back(1.0f / (1.0f + std::exp(-std::max(-100.0f, std::min(100.0f, s)))));
        }
        // 找到最大分数索引
        int64_t max_index = std::distance(sigmoidScore.begin(), std::max_element(sigmoidScore.begin(), sigmoidScore.end()));
        float max_score = sigmoidScore[max_index];
        LOG_I("最大分数: %.4f, 索引: %ld", max_score, max_index);

        // 从 reg_vec 中取出 best regressor（16 个数）
        assert(max_index * 16 + 16 <= (int) reg_vec.size());
        std::vector<float> best_regressor(reg_vec.begin() + max_index * 16, reg_vec.begin() + max_index * 16 + 16);
        float dx = best_regressor[0], dy = best_regressor[1], w_box = best_regressor[2], h_box = best_regressor[3];
        std::vector<float> keypoints(best_regressor.begin() + 4, best_regressor.end());

        // 生成锚点
        std::vector<Anchor> anchors = generate_face_detection_anchors(128);
        for (int i = 0; i < 5 && i < (int) anchors.size(); i++) {
            const Anchor &a = anchors[i];
        }

        // 对应的 anchor
        Anchor anchor = anchors[max_index];

        // 计算边界框（以 128 为基准尺寸）
        float box_center_x = dx + anchor.x_center * 128.0f;
        float box_center_y = dy + anchor.y_center * 128.0f;
        float box_w = w_box;
        float box_h = h_box;
        float box_x = box_center_x - box_w / 2.0f;
        float box_y = box_center_y - box_h / 2.0f;
        cv::Rect bbox(cv::Point(std::round(box_x), std::round(box_y)),
                      cv::Size(std::round(box_w), std::round(box_h)));

        // 解析关键点
        std::vector<cv::Point2f> kps;
        for (size_t i = 0; i + 1 < keypoints.size(); i += 2) {
            float kp_dx = keypoints[i];
            float kp_dy = keypoints[i + 1];
            float kp_x = kp_dx * anchor.w + anchor.x_center * 128.0f;
            float kp_y = kp_dy * anchor.h + anchor.y_center * 128.0f;
            kps.push_back(cv::Point2f(kp_x, kp_y));
        }

        // // 绘制检测结果在 padded 图像上
        // cv::Mat result_img = padded.clone();
        // cv::Mat result_draw = draw_detection(result_img, bbox,
        //                                      // 转换关键点为 cv::Point (四舍五入)
        //                                      std::vector<cv::Point>(kps.begin(), kps.end()));
        // // 保存结果图像（转换为 BGR 保存）
        // cv::Mat result_bgr;
        // cv::cvtColor(result_draw, result_bgr, cv::COLOR_RGB2BGR);
        // cv::imwrite(output_img_path, result_bgr);
        // LOG_I("检测结果保存至: %s", output_img_path.c_str());

        // 将检测框和关键点转换回原始图像坐标
        auto transform_bbox = [&](const cv::Rect &bbox, const PaddingParams &params) -> cv::Rect {
            float x1_orig, y1_orig, x2_orig, y2_orig;
            transform_coords_back(bbox.x, bbox.y, params, x1_orig, y1_orig);
            transform_coords_back(bbox.x + bbox.width, bbox.y + bbox.height, params, x2_orig, y2_orig);
            int new_x = std::round(x1_orig);
            int new_y = std::round(y1_orig);
            int new_w = std::round(x2_orig - x1_orig);
            int new_h = std::round(y2_orig - y1_orig);
            return cv::Rect(new_x, new_y, new_w, new_h);
        };

//        auto transform_keypoints_func = [&](const std::vector<cv::Point2f> &kps, const PaddingParams &params) -> std::vector<cv::Point> {
//            return transform_keypoints(kps, params);
//        };

//        cv::Rect original_bbox = transform_bbox(bbox, padding_params);
//        std::vector<cv::Point> original_keypoints = transform_keypoints_func(kps, padding_params);

//        cv::Mat original_with_det = originalImg.clone();
//        cv::Mat original_draw = draw_detection(original_with_det, original_bbox, original_keypoints);
//        cv::Mat original_bgr;
//        cv::cvtColor(original_draw, original_bgr, cv::COLOR_RGB2BGR);
//        cv::imwrite(output_img_path, original_bgr);
//        LOG_I("原始图像检测结果保存至: %s", output_img_path.c_str());
    }

} // test