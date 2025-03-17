//
// Created by leixing on 2025/1/8.
//

#include "engine/VulkanEngineBuilder.h"
#include "common/StringUtil.h"

#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/common/Uint32Selector.h"

namespace engine {

    VulkanEngineBuilder::VulkanEngineBuilder() = default;

    VulkanEngineBuilder::~VulkanEngineBuilder() = default;

    VulkanEngineBuilder &VulkanEngineBuilder::extensions(const std::vector<std::string> required, const std::vector<std::string> optional) {
        mExtensionsSelector = std::make_unique<common::RequiredAndOptionalStringListSelector>(required, optional);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::extensionsSelector(std::unique_ptr<common::ListSelector<std::string>> &selector) {
        mExtensionsSelector = std::move(selector);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::extensionsSelector(std::function<std::vector<std::string>(const std::vector<std::string> &)> selector) {
        mExtensionsSelector = std::make_unique<common::LambdaStringListSelector>(selector);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::layers(const std::vector<std::string> required, const std::vector<std::string> optional) {
        mLayersSelector = std::make_unique<common::RequiredAndOptionalStringListSelector>(required, optional);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::layersSelector(std::unique_ptr<common::ListSelector<std::string>> &selector) {
        mLayersSelector = std::move(selector);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::layersSelector(std::function<std::vector<std::string>(const std::vector<std::string> &)> selector) {
        mLayersSelector = std::make_unique<common::LambdaStringListSelector>(selector);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::applicationName(const std::string &applicationName) {
        mApplicationName = applicationName;
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::applicationVersion(uint32_t version) {
        mApplicationVersion = version;
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::applicationVersion(const std::string &version) {
        std::vector<uint32_t> versionNumbers = common::StringUtil::parseVersion(version);

        if (versionNumbers.size() < 2) {
            throw std::invalid_argument("Invalid version format: at least major and minor versions are required");
        }

        if (versionNumbers.size() == 2) {
            versionNumbers.push_back(0);
        }

        mApplicationVersion = VK_MAKE_VERSION(versionNumbers[0], versionNumbers[1], versionNumbers[2]);

        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::engineName(const std::string &engineName) {
        mEngineName = engineName;
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::engineVersion(uint32_t version) {
        mEngineVersion = version;
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::engineVersion(const std::string &version) {
        std::vector<uint32_t> versionNumbers = common::StringUtil::parseVersion(version);

        if (versionNumbers.size() < 2) {
            throw std::invalid_argument("Invalid version format: at least major and minor versions are required");
        }

        if (versionNumbers.size() == 2) {
            versionNumbers.push_back(0);
        }

        mEngineVersion = VK_MAKE_VERSION(versionNumbers[0], versionNumbers[1], versionNumbers[2]);

        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::surfaceBuilder(std::unique_ptr<VulkanSurfaceBuilder> &&surfaceBuilder) {
        mSurfaceBuilder = std::move(surfaceBuilder);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::deviceExtensions(std::vector<std::string> &&deviceExtensions) {
        mDeviceExtensions = std::move(deviceExtensions);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::frameCount(uint32_t frameCount) {
        mFrameCount = frameCount;
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::physicalDeviceAsDefault() {
        mVulkanPhysicalDeviceProvider = std::make_unique<DefaultVulkanPhysicalDeviceProvider>();
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::physicalDevice(std::unique_ptr<VulkanPhysicalDevice> &&vulkanPhysicalDevice) {
        mVulkanPhysicalDeviceProvider = std::make_unique<SimpleVulkanPhysicalDeviceProvider>(std::move(vulkanPhysicalDevice));
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::physicalDeviceProvider(std::unique_ptr<VulkanPhysicalDeviceProvider> &&provider) {
        mVulkanPhysicalDeviceProvider = std::move(provider);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::enableMsaa() {
        mMsaaSelector = std::make_unique<common::MaxUint32Selector>(4);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::enableMsaa(uint32_t msaaSamples) {
        mMsaaSelector = std::make_unique<common::FixUint32Selector>(msaaSamples);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::enableMsaaMax() {
        mMsaaSelector = std::make_unique<common::MaxUint32Selector>();
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::enableMsaaMax(uint32_t msaaSamplesMax) {
        mMsaaSelector = std::make_unique<common::MaxUint32Selector>(msaaSamplesMax);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::enableMsaa(const std::function<uint32_t(const std::vector<uint32_t> &)> &selector) {
        mMsaaSelector = std::make_unique<common::LambdaUint32Selector>(selector);
        return *this;
    }

//    VulkanEngineBuilder &VulkanEngineBuilder::shader(const std::function<void(VulkanShaderConfigure &)> &configure) {
//        VulkanShaderConfigure builder(*this);
//        configure(builder);
//        builder.build();
//        return *this;
//    }

    VulkanEngineBuilder &VulkanEngineBuilder::graphicsPipeline(const std::function<void(VulkanGraphicsPipelineConfigure &)> &configure) {
        mVulkanGraphicsPipelineConfigure = std::make_unique<VulkanGraphicsPipelineConfigure>();
        configure(*mVulkanGraphicsPipelineConfigure);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::computePipeline(const std::function<void(VulkanComputePipelineConfigure &)> &configure) {
        return *this;
    }
//
//    VulkanEngineBuilder &VulkanEngineBuilder::setComputeShaderCode(std::vector<char> &&code) {
//        mComputeShaderCode = std::move(code);
//        return *this;
//    }
//
//    VulkanEngineBuilder &VulkanEngineBuilder::setVertexShaderCode(std::vector<char> &&code) {
//        mVertexShaderCode = std::move(code);
//        return *this;
//    }
//
//    VulkanEngineBuilder &VulkanEngineBuilder::setFragmentShaderCode(std::vector<char> &&code) {
//        mFragmentShaderCode = std::move(code);
//        return *this;
//    }
//
//    VulkanEngineBuilder &VulkanEngineBuilder::setVertices(std::vector<VulkanVertex> &&vertices) {
//        mVertices = std::move(vertices);
//        return *this;
//    }
//
//    VulkanEngineBuilder &VulkanEngineBuilder::setUniformSets(std::vector<VulkanDescriptorSet> &&uniformSets) {
//        mDescriptorSets = std::move(uniformSets);
//        return *this;
//    }
//
//    VulkanEngineBuilder &VulkanEngineBuilder::setPushConstants(std::vector<VulkanPushConstant> &&pushConstants) {
//        mPushConstants = std::move(pushConstants);
//        return *this;
//    }

//    std::unique_ptr<VulkanEngine> VulkanEngineBuilder::build() {
//        // instance
//        std::unique_ptr<VulkanInstance> instance = std::make_unique<VulkanInstance>(mApplicationName, mApplicationVersion,
//                                                                                    mEngineName, mEngineVersion,
//                                                                                    *mExtensionsSelector, *mLayersSelector);
//
//        // surface
//        std::unique_ptr<VulkanSurface> surface = mSurfaceBuilder->build(*instance);
//
//        // select physical device
//        std::unique_ptr<VulkanPhysicalDeviceCandidate> candidate = std::move(mVulkanPhysicalDeviceProvider->provide(*instance, *surface, mDeviceExtensions));
//        std::unique_ptr<VulkanPhysicalDevice> &vulkanPhysicalDevice = candidate->getPhysicalDevice();
//
//
//        // logical device
//        uint32_t sampleCount = 0;
//        if (mMsaaSelector != nullptr) {
//            sampleCount = mMsaaSelector->select(vulkanPhysicalDevice->querySupportedSampleCounts());
//        }
//        std::unique_ptr<VulkanDevice> vulkanDevice = std::make_unique<VulkanDevice>(*vulkanPhysicalDevice,
//                                                                                    candidate->getSurfaceSupport().value(),
//                                                                                    mDeviceExtensions,
//                                                                                    instance->getEnabledLayers(),
//                                                                                    sampleCount);
//
//
//
//        // swapchain
//        vk::Extent2D currentExtent = vulkanDevice->getCapabilities().currentExtent;
//        LOG_D("currentExtent:[%d x %d]", currentExtent.width, currentExtent.height);
//        std::unique_ptr<VulkanSwapchain> swapchain = std::make_unique<VulkanSwapchain>(*vulkanDevice, *surface, currentExtent.width, currentExtent.height);
//
//        // command pool
//        std::unique_ptr<VulkanCommandPool> commandPool = std::make_unique<VulkanCommandPool>(*vulkanDevice, mFrameCount);
//
////        std::unique_ptr<VulkanShader> vulkanShader = std::make_unique<VulkanShader>(*instance, *vulkanDevice, *commandPool, mFrameCount,
////                                                                                    mComputeShaderCode,
////                                                                                    mVertexShaderCode,
////                                                                                    mFragmentShaderCode,
////                                                                                    mVertices,
////                                                                                    mDescriptorSets,
////                                                                                    mPushConstants);
//
//        std::unique_ptr<VulkanRenderPass> renderPass = std::make_unique<VulkanRenderPass>(*vulkanDevice, *swapchain);
//
//        std::unique_ptr<VulkanGraphicsPipeline> vulkanGraphicsPipeline = nullptr;
//        if(mVulkanGraphicsPipelineConfigure!= nullptr){
//            LOG_D("create VulkanGraphicsPipeline");
//            vulkanGraphicsPipeline = mVulkanGraphicsPipelineConfigure->build(*vulkanDevice, *swapchain, *renderPass);
//        }
//
//        std::unique_ptr<VulkanComputePipeline> vulkanComputePipeline = nullptr;
//        if(mVulkanComputePipelineConfigure!= nullptr){
//            LOG_D("create VulkanGraphicsPipeline");
//            vulkanComputePipeline = mVulkanComputePipelineConfigure->build();//std::make_unique<VulkanGraphicsPipeline>(*vulkanDevice, *swapchain, *renderPass, *vulkanShader);
//        }
//
//
//        LOG_D("create VulkanFrameBuffer");
//        std::unique_ptr<VulkanFrameBuffer> frameBuffer = std::make_unique<VulkanFrameBuffer>(*vulkanDevice, *swapchain, *renderPass, *commandPool);
//        LOG_D("create VulkanSyncObject");
//        std::unique_ptr<VulkanSyncObject> syncObject = std::make_unique<VulkanSyncObject>(*vulkanDevice, mFrameCount);
//
//        return std::make_unique<VulkanEngine>(std::move(instance),
//                                              std::move(surface),
//                                              std::move(vulkanPhysicalDevice),
//                                              std::move(vulkanDevice),
//                                              std::move(commandPool),
//                                              std::move(swapchain),
//                                              std::move(renderPass),
//                                              std::move(vulkanGraphicsPipeline),
//                                              std::move(vulkanComputePipeline),
//                                              std::move(frameBuffer),
//                                              std::move(syncObject),
//                                              mFrameCount);
//    }

} // engine