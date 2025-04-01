//
// Created by leixing on 2025/3/30.
//

#pragma once

namespace engine {

    class VulkanBufferInterface {
    private:

    public:
        VulkanBufferInterface();

        ~VulkanBufferInterface();

        [[nodiscard]]
        virtual uint32_t getSize() const = 0;
    };

} // engine