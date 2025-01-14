//
// Created by leixing on 2025/1/10.
//

#pragma once

#include <vector>
#include <functional>

#include "vulkan/vulkan.hpp"

#include "engine/vulkan_wrapper/VulkanInstance.h"
#include "engine/VulkanPhysicalDeviceProvider.h"

#include "engine/VulkanGraphicsEngine.h"
#include "engine/VulkanVertexShaderBuilder.h"
#include "engine/VulkanFragmentShaderBuilder.h"

#include "engine/common/Selector.h"

namespace engine {

    class VulkanVertexShaderBuilder;
    class VulkanFragmentShaderBuilder;

    class Vertex;

    class VulkanGraphicsEngineBuilder {
        friend class VulkanVertexShaderBuilder;
        friend class VulkanFragmentShaderBuilder;

    private:
        std::unique_ptr<VulkanInstance> mInstance;
        std::unique_ptr<VulkanSurface> mSurface;
        std::vector<std::string> mDeviceExtensions;

        uint32_t mFrameCount = 2;

        std::unique_ptr<VulkanPhysicalDeviceProvider> mVulkanPhysicalDeviceProvider;
        std::unique_ptr<common::ValueSelector<uint32_t>> mMsaaSelector;

        std::unique_ptr<VulkanVertexShaderBuilder> mVertexShaderBuilder;
        std::unique_ptr<VulkanFragmentShaderBuilder> mFragmentShaderBuilder;

        std::vector<char> mVertexShaderCode;
        std::vector<Vertex> mVertices;

        std::vector<char> mFragmentShaderCode;

        std::vector<uint32_t> mUniformSizes;

    public:
        VulkanGraphicsEngineBuilder(std::unique_ptr<VulkanInstance> instance);

        ~VulkanGraphicsEngineBuilder();

        VulkanGraphicsEngineBuilder &surface(std::function<std::unique_ptr<VulkanSurface>(const VulkanInstance &)> surfaceBuilder);

        VulkanGraphicsEngineBuilder &deviceExtensions(std::vector<std::string> &&deviceExtensions);

        VulkanGraphicsEngineBuilder &frameCount(uint32_t frameCount);

        VulkanGraphicsEngineBuilder &physicalDeviceAsDefault();

        VulkanGraphicsEngineBuilder &physicalDevice(std::unique_ptr<VulkanPhysicalDevice> vulkanPhysicalDevice);

        VulkanGraphicsEngineBuilder &physicalDeviceProvider(std::unique_ptr<VulkanPhysicalDeviceProvider> provider);

        VulkanGraphicsEngineBuilder &enableMsaa();

        VulkanGraphicsEngineBuilder &enableMsaa(uint32_t msaaSamples);

        VulkanGraphicsEngineBuilder &enableMsaaMax(uint32_t msaaSamplesMax);

        VulkanGraphicsEngineBuilder &enableMsaa(std::function<uint32_t(const std::vector<uint32_t> &)> selector);

        VulkanVertexShaderBuilder &vertexShaderBuilder();

        VulkanGraphicsEngineBuilder &vertexShader(std::function<void(VulkanVertexShaderBuilder &)> configure);

        VulkanGraphicsEngineBuilder &fragmentShader(std::function<void(VulkanFragmentShaderBuilder &)> configure);

        VulkanGraphicsEngineBuilder &setVertexShaderCode(std::vector<char> &&code);

        VulkanGraphicsEngineBuilder &setVertices(std::vector<Vertex> &&vertices);

        VulkanFragmentShaderBuilder &fragmentShaderBuilder();

        VulkanGraphicsEngineBuilder &setFragmentShaderCode(std::vector<char> &&code);

        std::unique_ptr<VulkanGraphicsEngine> build();
    };

} // engine
