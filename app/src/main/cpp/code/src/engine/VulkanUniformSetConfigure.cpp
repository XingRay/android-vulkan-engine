//
// Created by leixing on 2025/1/15.
//

#include "VulkanUniformSetConfigure.h"

namespace engine {

    VulkanUniformSetConfigure::VulkanUniformSetConfigure(VulkanShaderConfigure &builder)
            : mBuilder(builder), mUniformSet({}) {

    }

    VulkanUniformSetConfigure::~VulkanUniformSetConfigure() = default;

    VulkanUniformSetConfigure &VulkanUniformSetConfigure::set(uint32_t set) {
        mUniformSet.set = set;
        return *this;
    }

    VulkanUniformSetConfigure &VulkanUniformSetConfigure::addNormalBinding(uint32_t binding){
        mUniformSet.uniforms.push_back(VulkanNormalUniform(binding));
        return *this;
    }

    VulkanUniformSetConfigure &VulkanUniformSetConfigure::addSamplerBinding(uint32_t binding){
        mUniformSet.uniforms.push_back(VulkanSamplerUniform(binding));
        return *this;
    }

    VulkanShaderConfigure &VulkanUniformSetConfigure::build() {
        mBuilder.addUniformSet(mUniformSet);

        return mBuilder;
    }

} // engine