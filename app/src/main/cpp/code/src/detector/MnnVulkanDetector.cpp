//
// Created by leixing on 2025/2/20.
//

#include "MnnVulkanDetector.h"
#include "Log.h"
#include <MNN/ImageProcess.hpp>

namespace detector {
    MnnVulkanDetector::MnnVulkanDetector() {
        LOG_D("MNN::getVersion(): %s", MNN::getVersion());

        LOG_D("create mnn net");
        std::string mnn_model_path = R"(/storage/emulated/0/test/face_detection/face_detector.mnn)";
        mnn_net = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(mnn_model_path.c_str()));

        LOG_D("create mnn session");
        MNN::ScheduleConfig config;
        config.numThread = 2;
        config.backupType = MNN_FORWARD_VULKAN;
        MNN::BackendConfig backendConfig;
        backendConfig.memory = MNN::BackendConfig::Memory_Normal; // 内存
        backendConfig.power = MNN::BackendConfig::Power_Normal; // 功耗
        backendConfig.precision = MNN::BackendConfig::PrecisionMode::Precision_Normal; // 精度
        config.backendConfig = &backendConfig;
        session = mnn_net->createSession(config);

        // input和output是参于session的计算的输入和输出
        std::string input_name = "input";
        input_tensor = mnn_net->getSessionInput(session, input_name.c_str());

        std::string output_name_classificators = "classificators";
        output_classificators = mnn_net->getSessionOutput(session, output_name_classificators.c_str());

        std::string output_name_regressors = "regressors";
        output_regressors = mnn_net->getSessionOutput(session, output_name_regressors.c_str());

        // 复制数据到 MNN 的输入 Tensor
        LOG_D("Copying data to MNN input tensor...");
        auto input_dims = input_tensor->shape();
        input_width = input_dims[2]; // 宽度
        input_height = input_dims[1]; // 高度
        input_channels = input_dims[3]; // 通道
        LOG_D("input_channels:%d, input_height:%d, input_width:%d", input_channels, input_height, input_width);

        input_host = new MNN::Tensor(input_tensor, MNN::Tensor::TENSORFLOW);
        input_data = input_host->host<float>();

        output_classificators_host = new MNN::Tensor(output_classificators, MNN::Tensor::TENSORFLOW);
        output_regressors_host = new MNN::Tensor(output_regressors, MNN::Tensor::TENSORFLOW);

        class_data = output_classificators_host->host<float>();
        reg_data = output_regressors_host->host<float>();
    }

    MnnVulkanDetector::~MnnVulkanDetector() {
        delete input_host;
    }

    std::optional<std::pair<std::vector<float>, std::vector<float> > > MnnVulkanDetector::detect(AHardwareBuffer *hardwareBuffer){
        return std::nullopt;

        // **将 AHardwareBuffer 绑定为 Vulkan 纹理**
        LOG_D("Binding AHardwareBuffer to Vulkan...");
        MNN::CV::ImageProcess::Config imgProcessConfig;
        imgProcessConfig.sourceFormat = MNN::CV::YUV_NV21;
        imgProcessConfig.destFormat = MNN::CV::RGB;  // 可能需要转换颜色格式
        imgProcessConfig.filterType = MNN::CV::BILINEAR;
        std::shared_ptr<MNN::CV::ImageProcess> imageProcess(MNN::CV::ImageProcess::create(imgProcessConfig));

        // **获取 AHardwareBuffer 信息**
        AHardwareBuffer_Desc desc = {};
        AHardwareBuffer_describe(hardwareBuffer, &desc);
        LOG_D("AHardwareBuffer Info: width=%d, height=%d, layers=%d", desc.width, desc.height, desc.layers);

        volatile uint64_t inputValue = (uint64_t)hardwareBuffer;
        input_tensor->setDevicePtr((void*)inputValue, MNN_MEMORY_AHARDWAREBUFFER);


    }

//    std::optional<std::pair<std::vector<float>, std::vector<float> > > MnnVulkanDetector::detect(float* data) {
//        memcpy(input_data, data, sizeof(float) * input_width * input_height * input_channels);
//        // int index = 0;
//        // for (int c = 0; c < input_channels; c++) {
//        //     for (int h = 0; h < input_height; h++) {
//        //         for (int w = 0; w < input_width; w++) {
//        //             input_data[index++] = padded_float.at<cv::Vec3f>(c, h)[w];
//        //         }
//        //     }
//        // }
//
//        input_tensor->copyFromHostTensor(input_host);
//
//
//        // 执行前向推理
//        LOG_D("Running inference...");
//        mnn_net->runSession(session);
//
//        // 提取 MNN 推理的输出张量
//        LOG_D("Extracting MNN outputs...");
//
//        output_classificators->copyToHostTensor(output_classificators_host);
//        output_regressors->copyToHostTensor(output_regressors_host);
//
//
//
//        int mnn_num_scores = output_classificators_host->elementSize();
//        int mnn_num_regressors = output_regressors_host->elementSize();
//
//        std::vector<float> mnn_score_vec(class_data, class_data + mnn_num_scores);
//        std::vector<float> mnn_reg_vec(reg_data, reg_data + mnn_num_regressors);
//
//        LOG_D("mnn Inference completed!");
//
//        return std::pair<std::vector<float>, std::vector<float> >(mnn_score_vec, mnn_reg_vec);
//    }
} // detector