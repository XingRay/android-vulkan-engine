//
// Created by leixing on 2025/4/25.
//

#include "WriteDescriptorSet.h"

namespace vklite {

    WriteDescriptorSet::WriteDescriptorSet() {

    }

    WriteDescriptorSet::~WriteDescriptorSet() {

    }

    const vk::WriteDescriptorSet &WriteDescriptorSet::getWriteDescriptorSet() const {
        return mWriteDescriptorSet;
    }

//    void c() {
//        vk::WriteDescriptorSet writeDescriptorSet{};
//
//        vk::DescriptorImageInfo descriptorImageInfo = imageInfo->createDescriptorImageInfo();
//        std::array<vk::DescriptorImageInfo, 1> descriptorImageInfos = {descriptorImageInfo};
//
//        writeDescriptorSet
//                .setDstSet(mDescriptorSets[frameIndex][set])
//                .setDstBinding(binding)
//                        // 当描述符是一个数组时，这个属性指定从数组的哪个元素开始更新
//                .setDstArrayElement(vulkanDescriptorBinding->getDescriptorOffset())
//                .setDescriptorCount(vulkanDescriptorBinding->getDescriptorRange())
//                .setDescriptorType(vulkanDescriptorBinding->getDescriptorType())
//                .setImageInfo(descriptorImageInfos);
//    }

} // engine