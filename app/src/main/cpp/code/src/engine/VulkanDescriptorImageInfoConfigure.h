//
// Created by leixing on 2025/4/8.
//

#pragma once

#include <memory>

#include "engine/ImageInterface.h"


namespace engine {

    class VulkanDescriptorImageInfoConfigure {
    private:




        // create ImageView
//        std::unique_ptr<VulkanBufferBuilder> mVulkanBufferBuilder;
//        vk::DeviceSize mVulkanBufferCapacity;

        // create BufferView
//        uint32_t mVulkanBufferOffset;
//        uint32_t mVulkanBufferRange;

        // update buffer
//        std::vector<uint8_t> mVulkanBufferData;

//        std::shared_ptr<ImageInterface> mImage;

    public:
        VulkanDescriptorImageInfoConfigure();

        ~VulkanDescriptorImageInfoConfigure();
    };

} // engine
