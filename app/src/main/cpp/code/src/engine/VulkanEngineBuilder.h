//
// Created by leixing on 2025/1/8.
//

#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "common/StringListSelector.h"

#include "VulkanGraphicsEngine.h"
#include "VulkanComputerEngine.h"

namespace engine {

    class VulkanEngineBuilder {
    private:
        std::unique_ptr<common::StringListSelector> mExtensionsSelector;
        std::unique_ptr<common::StringListSelector> mLayersSelector;

        std::string mApplicationName = "App";
        uint32_t mApplicationVersion = VK_MAKE_VERSION(1, 0, 0);

        std::string mEngineName = "Engine";
        uint32_t mEngineVersion = VK_MAKE_VERSION(1, 0, 0);

    public:
        VulkanEngineBuilder &extensions(const std::vector<const char *> &required, const std::vector<const char *> &optional = {});

        VulkanEngineBuilder &extensionsSelector(std::unique_ptr<common::StringListSelector> &selector);

        VulkanEngineBuilder &extensionsSelector(const std::function<std::vector<const char *>(const std::vector<const char *> &)> selector);

        VulkanEngineBuilder &layers(const std::vector<const char *> &required, const std::vector<const char *> &optional = {});

        VulkanEngineBuilder &layersSelector(std::unique_ptr<common::StringListSelector> &selector);

        VulkanEngineBuilder &layersSelector(const std::function<std::vector<const char *>(const std::vector<const char *> &)> selector);

        VulkanEngineBuilder &applicationName(const char *applicationName);

        VulkanEngineBuilder &applicationVersion(uint32_t version);

        VulkanEngineBuilder &applicationVersion(const std::string &version);

        VulkanEngineBuilder &engineName(const char *engineName);

        VulkanEngineBuilder &engineVersion(uint32_t version);

        VulkanEngineBuilder &engineVersion(const std::string &version);

        std::unique_ptr<VulkanGraphicsEngine> asGraphics();

        std::unique_ptr<VulkanComputerEngine> asComputer();
    };

} // engine
