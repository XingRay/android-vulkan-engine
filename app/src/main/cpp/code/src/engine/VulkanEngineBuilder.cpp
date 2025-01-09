//
// Created by leixing on 2025/1/8.
//

#include "VulkanEngineBuilder.h"
#include "common/StringUtil.h"

namespace engine {

    VulkanEngineBuilder &VulkanEngineBuilder::extensions(const std::vector<const char *> &required, const std::vector<const char *> &optional) {
        mExtensionsSelector = std::make_unique<common::RequiredAndOptionalStringListSelector>(required, optional);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::extensionsSelector(std::unique_ptr<common::StringListSelector> &selector) {
        mExtensionsSelector = std::move(selector);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::extensionsSelector(const std::function<std::vector<const char *>(const std::vector<const char *> &)> selector) {
        mExtensionsSelector = std::make_unique<common::LambdaStringListSelector>(selector);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::layers(const std::vector<const char *> &required, const std::vector<const char *> &optional) {
        mLayersSelector = std::make_unique<common::RequiredAndOptionalStringListSelector>(required, optional);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::layersSelector(std::unique_ptr<common::StringListSelector> &selector) {
        mLayersSelector = std::move(selector);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::layersSelector(const std::function<std::vector<const char *>(const std::vector<const char *> &)> selector) {
        mLayersSelector = std::make_unique<common::LambdaStringListSelector>(selector);
        return *this;
    }

    VulkanEngineBuilder &VulkanEngineBuilder::applicationName(const char *applicationName) {
        if (applicationName) {
            mApplicationName.assign(applicationName, applicationName + std::strlen(applicationName) + 1);
        } else {
            mApplicationName.clear();
        }
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

    VulkanEngineBuilder &VulkanEngineBuilder::engineName(const char *engineName) {
        if (engineName) {
            mEngineName.assign(engineName, engineName + std::strlen(engineName) + 1);
        } else {
            mEngineName.clear();
        }
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

    std::unique_ptr<VulkanGraphicsEngine> VulkanEngineBuilder::asGraphics() {
        return std::make_unique<VulkanGraphicsEngine>(mApplicationName, mApplicationVersion,
                                                      mEngineName, mEngineVersion,
                                                      *mExtensionsSelector, *mLayersSelector);
    }

    std::unique_ptr<VulkanComputerEngine> asComputer() {
        // todo
        return std::make_unique<VulkanComputerEngine>();
    }

} // engine