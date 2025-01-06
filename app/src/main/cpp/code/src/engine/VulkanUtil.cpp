//
// Created by leixing on 2024/12/30.
//

#include "VulkanUtil.h"
#include "Log.h"

namespace engine {

    std::pair<vk::Buffer, vk::DeviceMemory> VulkanUtil::createBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties) {
        const vk::Device device = vulkanDevice.getDevice();
        const vk::PhysicalDevice physicalDevice = vulkanDevice.getPhysicalDevice();

//        std::vector<uint32_t> queueIndices = vulkanDevice.getQueueFamilyIndices();
//        vk::SharingMode sharingMode;
//        if (queueIndices.size() == 1) {
//            sharingMode = vk::SharingMode::eExclusive;
//        } else {
//            sharingMode = vk::SharingMode::eConcurrent;
//        }

        vk::BufferCreateInfo bufferCreateInfo{};
        bufferCreateInfo.setSize(size)
                .setUsage(usage)
                .setSharingMode(vk::SharingMode::eExclusive);
//                .setSharingMode(sharingMode)
//                .setQueueFamilyIndices(queueIndices);

        vk::Buffer buffer = device.createBuffer(bufferCreateInfo);
        vk::MemoryRequirements memoryRequirements = device.getBufferMemoryRequirements(buffer);
        vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

        uint32_t memoryType = findMemoryType(memoryProperties, memoryRequirements.memoryTypeBits, properties);
        vk::MemoryAllocateInfo memoryAllocateInfo{};
        memoryAllocateInfo
                .setAllocationSize(memoryRequirements.size)
                .setMemoryTypeIndex(memoryType);

        vk::DeviceMemory bufferMemory = device.allocateMemory(memoryAllocateInfo);
        device.bindBufferMemory(buffer, bufferMemory, 0);

        return {buffer, bufferMemory};
    }

    uint32_t VulkanUtil::findMemoryType(const vk::PhysicalDeviceMemoryProperties &memoryProperties, uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
        for (int i = 0; i < memoryProperties.memoryTypeCount; i++) {
            if (typeFilter & (1 << i) && ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type !");
    }

    void VulkanUtil::recordCopyBufferCommand(const vk::CommandBuffer &commandBuffer,
                                             const vk::Buffer &srcBuffer,
                                             const vk::Buffer &dstBuffer,
                                             vk::DeviceSize size) {
        vk::BufferCopy bufferCopy;
        bufferCopy.setSrcOffset(0)
                .setDstOffset(0)
                .setSize(size);

        commandBuffer.copyBuffer(srcBuffer, dstBuffer, bufferCopy);
    }

    std::pair<vk::Image, vk::DeviceMemory> VulkanUtil::createImage(
            const vk::Device &device,
            vk::PhysicalDeviceMemoryProperties properties,
            uint32_t width,
            uint32_t height,
            uint32_t mipLevels,
            vk::SampleCountFlagBits numSamples,
            vk::Format format,
            vk::ImageTiling imageTiling,
            vk::ImageUsageFlags imageUsage,
            vk::MemoryPropertyFlags memoryProperty) {

        vk::Extent3D extent;
        extent.setWidth(width)
                .setHeight(height)
                .setDepth(1);
        vk::ImageCreateInfo imageCreateInfo;
        imageCreateInfo.setImageType(vk::ImageType::e2D)
                .setExtent(extent)
                .setMipLevels(mipLevels)
                .setArrayLayers(1)
                .setFormat(format)
                .setTiling(imageTiling)
                .setInitialLayout(vk::ImageLayout::eUndefined)
                .setUsage(imageUsage)
                .setSharingMode(vk::SharingMode::eExclusive)
                .setSamples(numSamples)
                .setFlags(vk::ImageCreateFlags{});

        vk::Image image = device.createImage(imageCreateInfo);

        vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(image);

        vk::MemoryAllocateInfo memoryAllocateInfo;

        uint32_t memoryType = VulkanUtil::findMemoryType(properties, memoryRequirements.memoryTypeBits, memoryProperty);
        memoryAllocateInfo
                .setAllocationSize(memoryRequirements.size)
                .setMemoryTypeIndex(memoryType);

        vk::DeviceMemory imageMemory = device.allocateMemory(memoryAllocateInfo);

        device.bindImageMemory(image, imageMemory, 0);

        return {image, imageMemory};
    }

    vk::ImageView VulkanUtil::createImageView(const vk::Device &device, const vk::Image &image, vk::Format format, vk::ImageAspectFlags imageAspect, uint32_t mipLevels) {
        vk::ImageViewCreateInfo imageViewCreateInfo{};
        imageViewCreateInfo.setImage(image)
                .setViewType(vk::ImageViewType::e2D)
                .setFormat(format);
//            .setSubresourceRange(imageSubresourceRange)
//            .setComponents(componentMapping);

        vk::ImageSubresourceRange &imageSubresourceRange = imageViewCreateInfo.subresourceRange;
        imageSubresourceRange.setAspectMask(imageAspect)
                .setBaseMipLevel(0)
                .setLevelCount(mipLevels)
                .setBaseArrayLayer(0)
                .setLayerCount(1);

        vk::ComponentMapping &componentMapping = imageViewCreateInfo.components;
        componentMapping.setR(vk::ComponentSwizzle::eIdentity)
                .setG(vk::ComponentSwizzle::eIdentity)
                .setB(vk::ComponentSwizzle::eIdentity)
                .setA(vk::ComponentSwizzle::eIdentity);

        return device.createImageView(imageViewCreateInfo);
    }


    void VulkanUtil::printPhysicalDeviceInfo(const vk::PhysicalDevice &physicalDevice) {
        // 获取物理设备的属性
        vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();

        LOG_D("Physical Device Properties:");
        LOG_D("  Device Name: %s", properties.deviceName.data());
        LOG_D("  API Version: %u.%u.%u",
              VK_VERSION_MAJOR(properties.apiVersion),
              VK_VERSION_MINOR(properties.apiVersion),
              VK_VERSION_PATCH(properties.apiVersion));
        LOG_D("  Driver Version: %u", properties.driverVersion);
        LOG_D("  Vendor ID: 0x%04X", properties.vendorID);
        LOG_D("  Device ID: 0x%04X", properties.deviceID);
        LOG_D("  Device Type: %s", vk::to_string(properties.deviceType).c_str());
        LOG_D("  Pipeline Cache UUID: ");
        for (uint8_t byte: properties.pipelineCacheUUID) {
            LOG_D("    %02X", byte);
        }

        // 打印 PhysicalDeviceLimits
        const vk::PhysicalDeviceLimits &limits = properties.limits;

        LOG_D("Physical Device Limits:");
        LOG_D("  Max Image Dimension 1D: %u", limits.maxImageDimension1D);
        LOG_D("  Max Image Dimension 2D: %u", limits.maxImageDimension2D);
        LOG_D("  Max Image Dimension 3D: %u", limits.maxImageDimension3D);
        LOG_D("  Max Image Dimension Cube: %u", limits.maxImageDimensionCube);
        LOG_D("  Max Image Array Layers: %u", limits.maxImageArrayLayers);
        LOG_D("  Max Texel Buffer Elements: %u", limits.maxTexelBufferElements);
        LOG_D("  Max Uniform Buffer Range: %u", limits.maxUniformBufferRange);
        LOG_D("  Max Storage Buffer Range: %u", limits.maxStorageBufferRange);
        LOG_D("  Max Push Constants Size: %u", limits.maxPushConstantsSize);
        LOG_D("  Max Memory Allocation Count: %u", limits.maxMemoryAllocationCount);
        LOG_D("  Max Sampler Allocation Count: %u", limits.maxSamplerAllocationCount);
        LOG_D("  Buffer Image Granularity: %zu", limits.bufferImageGranularity);
        LOG_D("  Sparse Address Space Size: %zu", limits.sparseAddressSpaceSize);
        LOG_D("  Max Bound Descriptor Sets: %u", limits.maxBoundDescriptorSets);
        LOG_D("  Max Per Stage Descriptor Samplers: %u", limits.maxPerStageDescriptorSamplers);
        LOG_D("  Max Per Stage Descriptor Uniform Buffers: %u", limits.maxPerStageDescriptorUniformBuffers);
        LOG_D("  Max Per Stage Descriptor Storage Buffers: %u", limits.maxPerStageDescriptorStorageBuffers);
        LOG_D("  Max Per Stage Descriptor Sampled Images: %u", limits.maxPerStageDescriptorSampledImages);
        LOG_D("  Max Per Stage Descriptor Storage Images: %u", limits.maxPerStageDescriptorStorageImages);
        LOG_D("  Max Per Stage Descriptor Input Attachments: %u", limits.maxPerStageDescriptorInputAttachments);
        LOG_D("  Max Per Stage Resources: %u", limits.maxPerStageResources);
        LOG_D("  Max Descriptor Set Samplers: %u", limits.maxDescriptorSetSamplers);
        LOG_D("  Max Descriptor Set Uniform Buffers: %u", limits.maxDescriptorSetUniformBuffers);
        LOG_D("  Max Descriptor Set Uniform Buffers Dynamic: %u", limits.maxDescriptorSetUniformBuffersDynamic);
        LOG_D("  Max Descriptor Set Storage Buffers: %u", limits.maxDescriptorSetStorageBuffers);
        LOG_D("  Max Descriptor Set Storage Buffers Dynamic: %u", limits.maxDescriptorSetStorageBuffersDynamic);
        LOG_D("  Max Descriptor Set Sampled Images: %u", limits.maxDescriptorSetSampledImages);
        LOG_D("  Max Descriptor Set Storage Images: %u", limits.maxDescriptorSetStorageImages);
        LOG_D("  Max Descriptor Set Input Attachments: %u", limits.maxDescriptorSetInputAttachments);
        LOG_D("  Max Vertex Input Attributes: %u", limits.maxVertexInputAttributes);
        LOG_D("  Max Vertex Input Bindings: %u", limits.maxVertexInputBindings);
        LOG_D("  Max Vertex Input Attribute Offset: %u", limits.maxVertexInputAttributeOffset);
        LOG_D("  Max Vertex Input Binding Stride: %u", limits.maxVertexInputBindingStride);
        LOG_D("  Max Vertex Output Components: %u", limits.maxVertexOutputComponents);
        LOG_D("  Max Tessellation Generation Level: %u", limits.maxTessellationGenerationLevel);
        LOG_D("  Max Tessellation Patch Size: %u", limits.maxTessellationPatchSize);
        LOG_D("  Max Tessellation Control Per Vertex Input Components: %u", limits.maxTessellationControlPerVertexInputComponents);
        LOG_D("  Max Tessellation Control Per Vertex Output Components: %u", limits.maxTessellationControlPerVertexOutputComponents);
        LOG_D("  Max Tessellation Control Per Patch Output Components: %u", limits.maxTessellationControlPerPatchOutputComponents);
        LOG_D("  Max Tessellation Control Total Output Components: %u", limits.maxTessellationControlTotalOutputComponents);
        LOG_D("  Max Tessellation Evaluation Input Components: %u", limits.maxTessellationEvaluationInputComponents);
        LOG_D("  Max Tessellation Evaluation Output Components: %u", limits.maxTessellationEvaluationOutputComponents);
        LOG_D("  Max Geometry Shader Invocations: %u", limits.maxGeometryShaderInvocations);
        LOG_D("  Max Geometry Input Components: %u", limits.maxGeometryInputComponents);
        LOG_D("  Max Geometry Output Components: %u", limits.maxGeometryOutputComponents);
        LOG_D("  Max Geometry Output Vertices: %u", limits.maxGeometryOutputVertices);
        LOG_D("  Max Geometry Total Output Components: %u", limits.maxGeometryTotalOutputComponents);
        LOG_D("  Max Fragment Input Components: %u", limits.maxFragmentInputComponents);
        LOG_D("  Max Fragment Output Attachments: %u", limits.maxFragmentOutputAttachments);
        LOG_D("  Max Fragment Dual Src Attachments: %u", limits.maxFragmentDualSrcAttachments);
        LOG_D("  Max Fragment Combined Output Resources: %u", limits.maxFragmentCombinedOutputResources);
        LOG_D("  Max Compute Shared Memory Size: %u", limits.maxComputeSharedMemorySize);
        LOG_D("  Max Compute Work Group Count (x, y, z): %u, %u, %u", limits.maxComputeWorkGroupCount[0], limits.maxComputeWorkGroupCount[1], limits.maxComputeWorkGroupCount[2]);
        LOG_D("  Max Compute Work Group Invocations: %u", limits.maxComputeWorkGroupInvocations);
        LOG_D("  Max Compute Work Group Size (x, y, z): %u, %u, %u", limits.maxComputeWorkGroupSize[0], limits.maxComputeWorkGroupSize[1], limits.maxComputeWorkGroupSize[2]);
        LOG_D("  Sub Pixel Precision Bits: %u", limits.subPixelPrecisionBits);
        LOG_D("  Sub Texel Precision Bits: %u", limits.subTexelPrecisionBits);
        LOG_D("  Mipmap Precision Bits: %u", limits.mipmapPrecisionBits);
        LOG_D("  Max Draw Indexed Index Value: %u", limits.maxDrawIndexedIndexValue);
        LOG_D("  Max Draw Indirect Count: %u", limits.maxDrawIndirectCount);
        LOG_D("  Max Sampler Lod Bias: %f", limits.maxSamplerLodBias);
        LOG_D("  Max Sampler Anisotropy: %f", limits.maxSamplerAnisotropy);
        LOG_D("  Max Viewports: %u", limits.maxViewports);
        LOG_D("  Max Viewport Dimensions (x, y): %u, %u", limits.maxViewportDimensions[0], limits.maxViewportDimensions[1]);
        LOG_D("  Viewport Bounds Range (min, max): %f, %f", limits.viewportBoundsRange[0], limits.viewportBoundsRange[1]);
        LOG_D("  Viewport Sub Pixel Bits: %u", limits.viewportSubPixelBits);
        LOG_D("  Min Memory Map Alignment: %zu", limits.minMemoryMapAlignment);
        LOG_D("  Min Texel Buffer Offset Alignment: %zu", limits.minTexelBufferOffsetAlignment);
        LOG_D("  Min Uniform Buffer Offset Alignment: %zu", limits.minUniformBufferOffsetAlignment);
        LOG_D("  Min Storage Buffer Offset Alignment: %zu", limits.minStorageBufferOffsetAlignment);
        LOG_D("  Min Texel Offset: %d", limits.minTexelOffset);
        LOG_D("  Max Texel Offset: %u", limits.maxTexelOffset);
        LOG_D("  Min Texel Gather Offset: %d", limits.minTexelGatherOffset);
        LOG_D("  Max Texel Gather Offset: %u", limits.maxTexelGatherOffset);
        LOG_D("  Min Interpolation Offset: %f", limits.minInterpolationOffset);
        LOG_D("  Max Interpolation Offset: %f", limits.maxInterpolationOffset);
        LOG_D("  Sub Pixel Interpolation Offset Bits: %u", limits.subPixelInterpolationOffsetBits);
        LOG_D("  Max Framebuffer Width: %u", limits.maxFramebufferWidth);
        LOG_D("  Max Framebuffer Height: %u", limits.maxFramebufferHeight);
        LOG_D("  Max Framebuffer Layers: %u", limits.maxFramebufferLayers);
        LOG_D("  Framebuffer Color Sample Counts: %s", sampleCountFlagsToString(limits.framebufferColorSampleCounts).c_str());
        LOG_D("  Framebuffer Depth Sample Counts: %s", sampleCountFlagsToString(limits.framebufferDepthSampleCounts).c_str());
        LOG_D("  Framebuffer Stencil Sample Counts: %s", sampleCountFlagsToString(limits.framebufferStencilSampleCounts).c_str());
        LOG_D("  Framebuffer No Attachments Sample Counts: %s", sampleCountFlagsToString(limits.framebufferNoAttachmentsSampleCounts).c_str());
        LOG_D("  Max Color Attachments: %u", limits.maxColorAttachments);
        LOG_D("  Sampled Image Color Sample Counts: %s", sampleCountFlagsToString(limits.sampledImageColorSampleCounts).c_str());
        LOG_D("  Sampled Image Integer Sample Counts: %s", sampleCountFlagsToString(limits.sampledImageIntegerSampleCounts).c_str());
        LOG_D("  Sampled Image Depth Sample Counts: %s", sampleCountFlagsToString(limits.sampledImageDepthSampleCounts).c_str());
        LOG_D("  Sampled Image Stencil Sample Counts: %s", sampleCountFlagsToString(limits.sampledImageStencilSampleCounts).c_str());
        LOG_D("  Storage Image Sample Counts: %s", sampleCountFlagsToString(limits.storageImageSampleCounts).c_str());
        LOG_D("  Max Sample Mask Words: %u", limits.maxSampleMaskWords);
        LOG_D("  Timestamp Compute and Graphics: %s", limits.timestampComputeAndGraphics ? "Supported" : "Not Supported");
        LOG_D("  Timestamp Period: %f", limits.timestampPeriod);
        LOG_D("  Max Clip Distances: %u", limits.maxClipDistances);
        LOG_D("  Max Cull Distances: %u", limits.maxCullDistances);
        LOG_D("  Max Combined Clip and Cull Distances: %u", limits.maxCombinedClipAndCullDistances);
        LOG_D("  Discrete Queue Priorities: %u", limits.discreteQueuePriorities);
        LOG_D("  Point Size Range: %f, %f", limits.pointSizeRange[0], limits.pointSizeRange[1]);
        LOG_D("  Line Width Range: %f, %f", limits.lineWidthRange[0], limits.lineWidthRange[1]);
        LOG_D("  Point Size Granularity: %f", limits.pointSizeGranularity);
        LOG_D("  Line Width Granularity: %f", limits.lineWidthGranularity);
        LOG_D("  Strict Lines: %s", limits.strictLines ? "Supported" : "Not Supported");
        LOG_D("  Standard Sample Locations: %s", limits.standardSampleLocations ? "Supported" : "Not Supported");
        LOG_D("  Optimal Buffer Copy Offset Alignment: %zu", limits.optimalBufferCopyOffsetAlignment);
        LOG_D("  Optimal Buffer Copy Row Pitch Alignment: %zu", limits.optimalBufferCopyRowPitchAlignment);
        LOG_D("  Non-Coherent Atom Size: %zu", limits.nonCoherentAtomSize);

        // 打印 PhysicalDeviceSparseProperties
        const vk::PhysicalDeviceSparseProperties &sparseProperties = properties.sparseProperties;
        LOG_D("Physical Device Sparse Properties:");
        LOG_D("  Residency Standard 2D Block Shape: %s", sparseProperties.residencyStandard2DBlockShape ? "Supported" : "Not Supported");
        LOG_D("  Residency Standard 2D Multisample Block Shape: %s", sparseProperties.residencyStandard2DMultisampleBlockShape ? "Supported" : "Not Supported");
        LOG_D("  Residency Standard 3D Block Shape: %s", sparseProperties.residencyStandard3DBlockShape ? "Supported" : "Not Supported");
        LOG_D("  Residency Aligned Mip Size: %s", sparseProperties.residencyAlignedMipSize ? "Supported" : "Not Supported");
        LOG_D("  Residency Non-Resident Strict: %s", sparseProperties.residencyNonResidentStrict ? "Supported" : "Not Supported");

        // 获取设备支持的特性
        auto deviceFeatures = physicalDevice.getFeatures();
        LOG_D("PhysicalDeviceFeatures:");
        LOG_D("robustBufferAccess: %s", deviceFeatures.robustBufferAccess ? "Enabled" : "Disabled");
        LOG_D("fullDrawIndexUint32: %s", deviceFeatures.fullDrawIndexUint32 ? "Enabled" : "Disabled");
        LOG_D("imageCubeArray: %s", deviceFeatures.imageCubeArray ? "Enabled" : "Disabled");
        LOG_D("independentBlend: %s", deviceFeatures.independentBlend ? "Enabled" : "Disabled");
        LOG_D("geometryShader: %s", deviceFeatures.geometryShader ? "Enabled" : "Disabled");
        LOG_D("tessellationShader: %s", deviceFeatures.tessellationShader ? "Enabled" : "Disabled");
        LOG_D("sampleRateShading: %s", deviceFeatures.sampleRateShading ? "Enabled" : "Disabled");
        LOG_D("dualSrcBlend: %s", deviceFeatures.dualSrcBlend ? "Enabled" : "Disabled");
        LOG_D("logicOp: %s", deviceFeatures.logicOp ? "Enabled" : "Disabled");
        LOG_D("multiDrawIndirect: %s", deviceFeatures.multiDrawIndirect ? "Enabled" : "Disabled");
        LOG_D("drawIndirectFirstInstance: %s", deviceFeatures.drawIndirectFirstInstance ? "Enabled" : "Disabled");
        LOG_D("depthClamp: %s", deviceFeatures.depthClamp ? "Enabled" : "Disabled");
        LOG_D("depthBiasClamp: %s", deviceFeatures.depthBiasClamp ? "Enabled" : "Disabled");
        LOG_D("fillModeNonSolid: %s", deviceFeatures.fillModeNonSolid ? "Enabled" : "Disabled");
        LOG_D("depthBounds: %s", deviceFeatures.depthBounds ? "Enabled" : "Disabled");
        LOG_D("wideLines: %s", deviceFeatures.wideLines ? "Enabled" : "Disabled");
        LOG_D("largePoints: %s", deviceFeatures.largePoints ? "Enabled" : "Disabled");
        LOG_D("alphaToOne: %s", deviceFeatures.alphaToOne ? "Enabled" : "Disabled");
        LOG_D("multiViewport: %s", deviceFeatures.multiViewport ? "Enabled" : "Disabled");
        LOG_D("samplerAnisotropy: %s", deviceFeatures.samplerAnisotropy ? "Enabled" : "Disabled");
        LOG_D("textureCompressionETC2: %s", deviceFeatures.textureCompressionETC2 ? "Enabled" : "Disabled");
        LOG_D("textureCompressionASTC_LDR: %s", deviceFeatures.textureCompressionASTC_LDR ? "Enabled" : "Disabled");
        LOG_D("textureCompressionBC: %s", deviceFeatures.textureCompressionBC ? "Enabled" : "Disabled");
        LOG_D("occlusionQueryPrecise: %s", deviceFeatures.occlusionQueryPrecise ? "Enabled" : "Disabled");
        LOG_D("pipelineStatisticsQuery: %s", deviceFeatures.pipelineStatisticsQuery ? "Enabled" : "Disabled");
        LOG_D("vertexPipelineStoresAndAtomics: %s", deviceFeatures.vertexPipelineStoresAndAtomics ? "Enabled" : "Disabled");
        LOG_D("fragmentStoresAndAtomics: %s", deviceFeatures.fragmentStoresAndAtomics ? "Enabled" : "Disabled");
        LOG_D("shaderTessellationAndGeometryPointSize: %s", deviceFeatures.shaderTessellationAndGeometryPointSize ? "Enabled" : "Disabled");
        LOG_D("shaderImageGatherExtended: %s", deviceFeatures.shaderImageGatherExtended ? "Enabled" : "Disabled");
        LOG_D("shaderStorageImageExtendedFormats: %s", deviceFeatures.shaderStorageImageExtendedFormats ? "Enabled" : "Disabled");
        LOG_D("shaderStorageImageMultisample: %s", deviceFeatures.shaderStorageImageMultisample ? "Enabled" : "Disabled");
        LOG_D("shaderStorageImageReadWithoutFormat: %s", deviceFeatures.shaderStorageImageReadWithoutFormat ? "Enabled" : "Disabled");
        LOG_D("shaderStorageImageWriteWithoutFormat: %s", deviceFeatures.shaderStorageImageWriteWithoutFormat ? "Enabled" : "Disabled");
        LOG_D("shaderUniformBufferArrayDynamicIndexing: %s", deviceFeatures.shaderUniformBufferArrayDynamicIndexing ? "Enabled" : "Disabled");
        LOG_D("shaderSampledImageArrayDynamicIndexing: %s", deviceFeatures.shaderSampledImageArrayDynamicIndexing ? "Enabled" : "Disabled");
        LOG_D("shaderStorageBufferArrayDynamicIndexing: %s", deviceFeatures.shaderStorageBufferArrayDynamicIndexing ? "Enabled" : "Disabled");
        LOG_D("shaderStorageImageArrayDynamicIndexing: %s", deviceFeatures.shaderStorageImageArrayDynamicIndexing ? "Enabled" : "Disabled");
        LOG_D("shaderClipDistance: %s", deviceFeatures.shaderClipDistance ? "Enabled" : "Disabled");
        LOG_D("shaderCullDistance: %s", deviceFeatures.shaderCullDistance ? "Enabled" : "Disabled");
        LOG_D("shaderFloat64: %s", deviceFeatures.shaderFloat64 ? "Enabled" : "Disabled");
        LOG_D("shaderInt64: %s", deviceFeatures.shaderInt64 ? "Enabled" : "Disabled");
        LOG_D("shaderInt16: %s", deviceFeatures.shaderInt16 ? "Enabled" : "Disabled");
        LOG_D("shaderResourceResidency: %s", deviceFeatures.shaderResourceResidency ? "Enabled" : "Disabled");
        LOG_D("shaderResourceMinLod: %s", deviceFeatures.shaderResourceMinLod ? "Enabled" : "Disabled");
        LOG_D("sparseBinding: %s", deviceFeatures.sparseBinding ? "Enabled" : "Disabled");
        LOG_D("sparseResidencyBuffer: %s", deviceFeatures.sparseResidencyBuffer ? "Enabled" : "Disabled");
        LOG_D("sparseResidencyImage2D: %s", deviceFeatures.sparseResidencyImage2D ? "Enabled" : "Disabled");
        LOG_D("sparseResidencyImage3D: %s", deviceFeatures.sparseResidencyImage3D ? "Enabled" : "Disabled");
        LOG_D("sparseResidency2Samples: %s", deviceFeatures.sparseResidency2Samples ? "Enabled" : "Disabled");
        LOG_D("sparseResidency4Samples: %s", deviceFeatures.sparseResidency4Samples ? "Enabled" : "Disabled");
        LOG_D("sparseResidency8Samples: %s", deviceFeatures.sparseResidency8Samples ? "Enabled" : "Disabled");
        LOG_D("sparseResidency16Samples: %s", deviceFeatures.sparseResidency16Samples ? "Enabled" : "Disabled");
        LOG_D("sparseResidencyAliased: %s", deviceFeatures.sparseResidencyAliased ? "Enabled" : "Disabled");
        LOG_D("variableMultisampleRate: %s", deviceFeatures.variableMultisampleRate ? "Enabled" : "Disabled");
        LOG_D("inheritedQueries: %s", deviceFeatures.inheritedQueries ? "Enabled" : "Disabled");

        // 获取队列族属性
        auto queueFamilies = physicalDevice.getQueueFamilyProperties();
        LOG_D("Queue Family Properties:");
        for (size_t i = 0; i < queueFamilies.size(); ++i) {
            LOG_D("  Queue Family #%zu:", i);
            LOG_D("    Queue Count: %u", queueFamilies[i].queueCount);
            LOG_D("    Queue Flags: %s", vk::to_string(queueFamilies[i].queueFlags).c_str());
            LOG_D("    Timestamp Valid Bits: %u", queueFamilies[i].timestampValidBits);
            LOG_D("    Min Image Transfer Granularity: (%u, %u, %u)",
                  queueFamilies[i].minImageTransferGranularity.width,
                  queueFamilies[i].minImageTransferGranularity.height,
                  queueFamilies[i].minImageTransferGranularity.depth);
        }

        // 获取设备支持的扩展
        auto extensions = physicalDevice.enumerateDeviceExtensionProperties();
        LOG_D("Supported Extensions:");
        for (const auto &ext: extensions) {
            LOG_D("  %s (version %u)", ext.extensionName.data(), ext.specVersion);
        }

        // 获取设备的内存属性
        auto memoryProperties = physicalDevice.getMemoryProperties();
        LOG_D("Memory Heaps:");
        for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; ++i) {
            LOG_D("  Heap #%u: Size = %s, Flags = %s", i,
                  formatDeviceSize(memoryProperties.memoryHeaps[i].size).c_str(),
                  vk::to_string(memoryProperties.memoryHeaps[i].flags).c_str());
        }
        LOG_D("Memory Types:");
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
            LOG_D("  Type #%u: HeapIndex = %u, Flags = %s", i,
                  memoryProperties.memoryTypes[i].heapIndex,
                  vk::to_string(memoryProperties.memoryTypes[i].propertyFlags).c_str());
        }
    }


    void VulkanUtil::printPhysicalDeviceInfoWithSurface(const vk::PhysicalDevice &physicalDevice, const vk::SurfaceKHR &surface) {
        // 获取交换链支持详情并直接打印
        auto capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
        LOG_D("Surface Capabilities:");
        LOG_D("  minImageCount: %u", capabilities.minImageCount);
        LOG_D("  maxImageCount: %u", capabilities.maxImageCount);
        LOG_D("  currentExtent: (%u, %u)", capabilities.currentExtent.width, capabilities.currentExtent.height);
        LOG_D("  minImageExtent: (%u, %u)", capabilities.minImageExtent.width, capabilities.minImageExtent.height);
        LOG_D("  maxImageExtent: (%u, %u)", capabilities.maxImageExtent.width, capabilities.maxImageExtent.height);
        LOG_D("  maxImageArrayLayers: %u", capabilities.maxImageArrayLayers);
        LOG_D("  supportedTransforms: %s", vk::to_string(capabilities.supportedTransforms).c_str());
        LOG_D("  currentTransform: %s", vk::to_string(capabilities.currentTransform).c_str());
        LOG_D("  supportedCompositeAlpha: %s", vk::to_string(capabilities.supportedCompositeAlpha).c_str());
        LOG_D("  supportedUsageFlags: %s", vk::to_string(capabilities.supportedUsageFlags).c_str());

        // 打印格式信息
        auto formats = physicalDevice.getSurfaceFormatsKHR(surface);
        LOG_D("Surface Formats:");
        for (const auto &format: formats) {
            LOG_D("  Format: %s, ColorSpace: %s",
                  vk::to_string(format.format).c_str(),
                  vk::to_string(format.colorSpace).c_str());
        }

        // 打印呈现模式信息
        auto presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
        LOG_D("Surface Present Modes:");
        for (const auto &presentMode: presentModes) {
            LOG_D("  PresentMode: %s", vk::to_string(presentMode).c_str());
        }
    }


    // 转换 SampleCountFlags 为字符串
    std::string VulkanUtil::sampleCountFlagsToString(vk::SampleCountFlags flags) {
        std::string result;

        if (flags & vk::SampleCountFlagBits::e1) result += "1 ";
        if (flags & vk::SampleCountFlagBits::e2) result += "2 ";
        if (flags & vk::SampleCountFlagBits::e4) result += "4 ";
        if (flags & vk::SampleCountFlagBits::e8) result += "8 ";
        if (flags & vk::SampleCountFlagBits::e16) result += "16 ";
        if (flags & vk::SampleCountFlagBits::e32) result += "32 ";
        if (flags & vk::SampleCountFlagBits::e64) result += "64 ";

        if (result.empty()) return "None";
        return result;
    }

    // 将 vk::DeviceSize 转换为更易读的单位
    std::string VulkanUtil::formatDeviceSize(vk::DeviceSize size) {
        constexpr vk::DeviceSize KB = 1024;
        constexpr vk::DeviceSize MB = KB * 1024;
        constexpr vk::DeviceSize GB = MB * 1024;

        if (size >= GB) {
            return std::to_string(size / GB) + " GB";
        } else if (size >= MB) {
            return std::to_string(size / MB) + " MB";
        } else if (size >= KB) {
            return std::to_string(size / KB) + " KB";
        } else {
            return std::to_string(size) + " bytes";
        }
    }

    void VulkanUtil::recordTransitionImageLayoutCommand(const vk::CommandBuffer &commandBuffer,
                                                               vk::Image image,
                                                               vk::Format format,
                                                               vk::ImageLayout oldImageLayout,
                                                               vk::ImageLayout newImageLayout,
                                                               uint32_t mipLevels) {

        vk::ImageSubresourceRange imageSubresourceRange;
        imageSubresourceRange
                .setBaseMipLevel(0)
                .setLevelCount(mipLevels)
                .setBaseArrayLayer(0)
                .setLayerCount(1);

        // 注意这里一定要是 vk::ImageLayout::eDepthStencilAttachmentOptimal ， 写成了 vk::ImageLayout::eStencilAttachmentOptimal 后面会报警告
        if (newImageLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
            if (hasStencilComponent(format)) {
                imageSubresourceRange.setAspectMask(vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil);
            } else {
                imageSubresourceRange.setAspectMask(vk::ImageAspectFlagBits::eDepth);
            }
        } else {
            imageSubresourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor);
        }


        vk::ImageMemoryBarrier imageMemoryBarrier;
        imageMemoryBarrier.setOldLayout(oldImageLayout)
                .setNewLayout(newImageLayout)
                .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
                .setDstQueueFamilyIndex(vk::QueueFamilyIgnored)
                .setImage(image)
                .setSubresourceRange(imageSubresourceRange)
                .setSrcAccessMask(vk::AccessFlags{})
                .setDstAccessMask(vk::AccessFlags{});

        vk::PipelineStageFlags sourceStage;
        vk::PipelineStageFlags destinationStage;

        if (oldImageLayout == vk::ImageLayout::eUndefined && newImageLayout == vk::ImageLayout::eTransferDstOptimal) {
            imageMemoryBarrier.setSrcAccessMask(static_cast<vk::AccessFlags>(0))
                    .setDstAccessMask(vk::AccessFlagBits::eTransferWrite);

            sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
            destinationStage = vk::PipelineStageFlagBits::eTransfer;
        } else if (oldImageLayout == vk::ImageLayout::eTransferDstOptimal && newImageLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
            imageMemoryBarrier.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
                    .setDstAccessMask(vk::AccessFlagBits::eShaderRead);

            sourceStage = vk::PipelineStageFlagBits::eTransfer;
            destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
        } else if (oldImageLayout == vk::ImageLayout::eUndefined && newImageLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
            imageMemoryBarrier.setSrcAccessMask(vk::AccessFlags{})
                    .setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite);

            sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
            destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
        } else {
            throw std::runtime_error("unsupported layout transition!");
        }

        commandBuffer.pipelineBarrier(sourceStage,
                                      destinationStage,
                                      vk::DependencyFlags{},
                // 内存屏障
                                      {},
                // 缓冲区内存屏障
                                      {},
                // 图像内存屏障
                                      {imageMemoryBarrier});


    }

    bool VulkanUtil::hasStencilComponent(vk::Format format) {
        return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
    }

    vk::Format VulkanUtil::findDepthFormat(const vk::PhysicalDevice &physicalDevice) {
        return VulkanUtil::findSupportedFormat(
                physicalDevice,
                {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
                vk::ImageTiling::eOptimal,
                vk::FormatFeatureFlagBits::eDepthStencilAttachment
        );
    }

    vk::Format VulkanUtil::findSupportedFormat(const vk::PhysicalDevice &physicalDevice, const std::vector<vk::Format> &candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) {
        for (const auto &format: candidates) {
            vk::FormatProperties properties = physicalDevice.getFormatProperties(format);
            if (tiling == vk::ImageTiling::eLinear && (properties.linearTilingFeatures & features) == features) {
                return format;
            } else if (tiling == vk::ImageTiling::eOptimal && (properties.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        throw std::runtime_error("failed to find supported format !");
    }

} // engine