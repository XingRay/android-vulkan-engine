//
// Created by leixing on 2025/1/10.
//

#include "engine/VulkanGraphicsEngineBuilder.h"

#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/common/Uint32Selector.h"

namespace engine {

    VulkanGraphicsEngineBuilder::VulkanGraphicsEngineBuilder(std::unique_ptr<VulkanInstance> instance) {
        mInstance = std::move(instance);
    }

    VulkanGraphicsEngineBuilder::~VulkanGraphicsEngineBuilder() = default;

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::surface(std::function<std::unique_ptr<VulkanSurface>(const VulkanInstance &)> surfaceBuilder) {
        mSurface = std::move(surfaceBuilder(*mInstance));
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::deviceExtensions(std::vector<std::string> &&deviceExtensions) {
        mDeviceExtensions = std::move(deviceExtensions);
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::frameCount(uint32_t frameCount) {
        mFrameCount = frameCount;
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::physicalDeviceAsDefault() {
        mVulkanPhysicalDeviceProvider = std::make_unique<DefaultVulkanPhysicalDeviceProvider>(*mInstance, *mSurface, mDeviceExtensions);
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::physicalDevice(std::unique_ptr<VulkanPhysicalDevice> vulkanPhysicalDevice) {
        std::unique_ptr<VulkanPhysicalDeviceCandidate> candidate = std::make_unique<VulkanPhysicalDeviceCandidate>(std::move(vulkanPhysicalDevice));
        std::optional<VulkanPhysicalDeviceSurfaceSupport> surfaceSupport = candidate->getPhysicalDevice()->querySurfaceSupport(*mSurface);
        if (!surfaceSupport.has_value()) {
            throw std::runtime_error("vulkanPhysicalDevice can not use for surface");
        }
        candidate->setSurfaceSupport(std::move(surfaceSupport.value()));
        mVulkanPhysicalDeviceProvider = std::make_unique<SimpleVulkanPhysicalDeviceProvider>(std::move(candidate));
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::physicalDeviceProvider(std::unique_ptr<VulkanPhysicalDeviceProvider> provider) {
        mVulkanPhysicalDeviceProvider = std::move(provider);
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::enableMsaa() {
        mMsaaSelector = std::make_unique<common::MaxUint32Selector>();
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::enableMsaa(uint32_t msaaSamples) {
        mMsaaSelector = std::make_unique<common::FixUint32Selector>(msaaSamples);
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::enableMsaaMax(uint32_t msaaSamplesMax) {
        mMsaaSelector = std::make_unique<common::MaxUint32Selector>(msaaSamplesMax);
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::enableMsaa(std::function<uint32_t(const std::vector<uint32_t> &)> selector) {
        mMsaaSelector = std::make_unique<common::LambdaUint32Selector>(selector);
        return *this;
    }

    VulkanVertexShaderBuilder &VulkanGraphicsEngineBuilder::vertexShaderBuilder() {
        mVertexShaderBuilder = std::make_unique<VulkanVertexShaderBuilder>(*this);
        return *mVertexShaderBuilder;
    }

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::vertexShader(std::function<void(VulkanVertexShaderBuilder &)> configure) {
        VulkanVertexShaderBuilder builder(*this);
        configure(builder);
        builder.build();
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::fragmentShader(std::function<void(VulkanFragmentShaderBuilder &)> configure) {
        VulkanFragmentShaderBuilder builder(*this);
        configure(builder);
        builder.build();
        return *this;
    }

    VulkanFragmentShaderBuilder &VulkanGraphicsEngineBuilder::fragmentShaderBuilder() {
        mFragmentShaderBuilder = std::make_unique<VulkanFragmentShaderBuilder>(*this);
        return *mFragmentShaderBuilder;
    }


    std::unique_ptr<VulkanGraphicsEngine> VulkanGraphicsEngineBuilder::build() {
        std::unique_ptr<VulkanPhysicalDeviceCandidate> candidate = std::move(mVulkanPhysicalDeviceProvider->provide());
        std::unique_ptr<VulkanPhysicalDevice> &vulkanPhysicalDevice = candidate->getPhysicalDevice();

        uint32_t sampleCount = 0;
        if (mMsaaSelector != nullptr) {
            sampleCount = mMsaaSelector->select(vulkanPhysicalDevice->querySupportedSampleCounts());
        }

        std::unique_ptr<VulkanDevice> vulkanDevice = std::make_unique<VulkanDevice>(*vulkanPhysicalDevice,
                                                                                    candidate->getSurfaceSupport().value(),
                                                                                    mDeviceExtensions,
                                                                                    mInstance->getEnabledLayers(),
                                                                                    sampleCount);

        std::unique_ptr<VulkanShader> vulkanShader = std::make_unique<VulkanShader>();
        (*vulkanShader).setVertexShaderCode(std::move(mVertexShaderCode));
        (*vulkanShader).setFragmentShaderCode(std::move(mFragmentShaderCode));
        for (const VulkanVertex &vertex: mVertices) {
            (*vulkanShader).addVertexBinding(vertex.binding, vertex.size);
            for (const VulkanVertexAttribute &attribute: vertex.attributes) {
                (*vulkanShader).addVertexAttribute(attribute.binding, attribute.location, attribute.format, attribute.offset);
            }
        }


        return std::make_unique<VulkanGraphicsEngine>(std::move(mInstance),
                                                      std::move(mSurface),
                                                      std::move(vulkanPhysicalDevice),
                                                      std::move(vulkanDevice),
                                                      std::move(vulkanShader),
                                                      mFrameCount);
    }

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::setVertexShaderCode(std::vector<char> &&code) {
        mVertexShaderCode = std::move(code);
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::setVertices(std::vector<VulkanVertex> &&vertices) {
        mVertices = std::move(vertices);
        return *this;
    }

    VulkanGraphicsEngineBuilder &VulkanGraphicsEngineBuilder::setFragmentShaderCode(std::vector<char> &&code) {
        mFragmentShaderCode = std::move(code);
        return *this;
    }

} // engine