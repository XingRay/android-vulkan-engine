//
// Created by leixing on 2025/1/8.
//

#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "engine/common/StringListSelector.h"

#ifndef APPLICATION_NAME
#define APPLICATION_NAME "3d_application"
#endif

#ifndef ENGINE_NAME
#define ENGINE_NAME "3d_engine"
#endif

#include "vulkan/vulkan.hpp"

#include "engine/vulkan_wrapper/VulkanInstance.h"
#include "engine/configure/physical_device/VulkanPhysicalDeviceProvider.h"

#include "engine/VulkanEngine.h"
#include "engine/VulkanSurfaceBuilder.h"
#include "engine/configure/pipeline/GraphicsPipelineConfigure.h"
#include "engine/configure/pipeline/ComputePipelineConfigure.h"

#include "engine/common/Selector.h"

namespace engine {

    class VulkanEngineBuilder {

    private:
        std::unique_ptr<common::ListSelector<std::string>> mExtensionsSelector;
        std::unique_ptr<common::ListSelector<std::string>> mLayersSelector;

        std::string mApplicationName = APPLICATION_NAME;
        uint32_t mApplicationVersion = VK_MAKE_VERSION(1, 0, 0);

        std::string mEngineName = ENGINE_NAME;
        uint32_t mEngineVersion = VK_MAKE_VERSION(1, 0, 0);

        std::unique_ptr<VulkanSurfaceBuilder> mSurfaceBuilder;
        std::vector<std::string> mDeviceExtensions;

        uint32_t mFrameCount = 2;

        std::unique_ptr<VulkanPhysicalDeviceProvider> mVulkanPhysicalDeviceProvider;
        std::unique_ptr<common::ValueSelector<uint32_t>> mMsaaSelector;

        std::unique_ptr<GraphicsPipelineConfigure> mVulkanGraphicsPipelineConfigure;
        std::unique_ptr<ComputePipelineConfigure> mVulkanComputePipelineConfigure;

//        // shader
//        std::vector<char> mComputeShaderCode;
//        std::vector<char> mVertexShaderCode;
//        std::vector<char> mFragmentShaderCode;
//
//        std::vector<VulkanVertex> mVertices;
//        std::vector<VulkanDescriptorSet> mDescriptorSets;
//        std::vector<VulkanPushConstant> mPushConstants;

    public:

        VulkanEngineBuilder();

        ~VulkanEngineBuilder();

        // instance extensions
        VulkanEngineBuilder &extensions(std::vector<std::string> &&required, std::vector<std::string> &&optional = {});

        VulkanEngineBuilder &extensionsSelector(std::unique_ptr<common::ListSelector<std::string>> &selector);

        VulkanEngineBuilder &extensionsSelector(std::function<std::vector<std::string>(const std::vector<std::string> &)> selector);

        // instance layers
        VulkanEngineBuilder &layers(std::vector<std::string> &&required, std::vector<std::string> &&optional = {});

        VulkanEngineBuilder &layersSelector(std::unique_ptr<common::ListSelector<std::string>> &selector);

        VulkanEngineBuilder &layersSelector(std::function<std::vector<std::string>(const std::vector<std::string> &)> selector);

        // application
        VulkanEngineBuilder &applicationName(const std::string &applicationName);

        VulkanEngineBuilder &applicationVersion(uint32_t version);

        // version: "1.0" or "1.0.0"
        VulkanEngineBuilder &applicationVersion(const std::string &version);


        // engine
        VulkanEngineBuilder &engineName(const std::string &engineName);

        VulkanEngineBuilder &engineVersion(uint32_t version);

        // version: "1.0" or "1.0.0"
        VulkanEngineBuilder &engineVersion(const std::string &version);

        // surface
        VulkanEngineBuilder &surfaceBuilder(std::unique_ptr<VulkanSurfaceBuilder> &&surfaceBuilder);

        // device extensions
        VulkanEngineBuilder &deviceExtensions(std::vector<std::string> &&deviceExtensions);

        // frame count
        VulkanEngineBuilder &frameCount(uint32_t frameCount);

        // physical device
        VulkanEngineBuilder &physicalDeviceAsDefault();

        VulkanEngineBuilder &physicalDevice(std::unique_ptr<VulkanPhysicalDevice> &&vulkanPhysicalDevice);

        VulkanEngineBuilder &physicalDeviceProvider(std::unique_ptr<VulkanPhysicalDeviceProvider> &&provider);

        // msaa
        VulkanEngineBuilder &enableMsaa();

        VulkanEngineBuilder &enableMsaa(uint32_t msaaSamples);

        VulkanEngineBuilder &enableMsaaMax();

        VulkanEngineBuilder &enableMsaaMax(uint32_t msaaSamplesMax);

        VulkanEngineBuilder &enableMsaa(const std::function<uint32_t(const std::vector<uint32_t> &)> &selector);

        // graphics pipeline
        VulkanEngineBuilder &graphicsPipeline(const std::function<void(GraphicsPipelineConfigure &)> &configure);

        // compute pipeline
        VulkanEngineBuilder &computePipeline(const std::function<void(ComputePipelineConfigure &)> &configure);

        std::unique_ptr<VulkanEngine> build();

    private:

    };

} // engine
