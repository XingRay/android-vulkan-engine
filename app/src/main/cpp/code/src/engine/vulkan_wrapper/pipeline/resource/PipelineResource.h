//
// Created by leixing on 2025/4/18.
//

#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>

#include "PipelineResourceSlot.h"

/**
 * resources for a frame used in pipeline
 * resources: uniform buffer, vk::ImageView ...
 */
namespace engine {

    class PipelineResource {
    private:
        // set -> binding -> offset -> slot
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::unordered_map<uint32_t, PipelineResourceSlot>>> mPipelineResourceSlots;

    public:

        PipelineResource();

        ~PipelineResource();
    };

} // engine
