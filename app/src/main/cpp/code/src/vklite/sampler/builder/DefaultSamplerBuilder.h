//
// Created by leixing on 2025/4/11.
//

#pragma once

#include "SamplerBuilderInterface.h"

namespace vklite {

    class DefaultSamplerBuilder : public SamplerBuilderInterface {
    private:
        float mMaxLoad;

    public:

        DefaultSamplerBuilder();

        ~DefaultSamplerBuilder() override;

        DefaultSamplerBuilder &setMaxLoad(float maxLoad);

        std::unique_ptr<SamplerInterface> build(const VulkanDevice &vulkanDevice) override;

    };

} // engine
