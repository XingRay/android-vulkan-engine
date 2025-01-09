//
// Created by leixing on 2025/1/8.
//

#include "StringListSelector.h"

#include <utility>
#include "Log.h"

namespace common {

    FixStringListSelector::FixStringListSelector(const std::vector<const char *> &selected)
            : mSelected(selected) {

    }

    FixStringListSelector::~FixStringListSelector() {
        mSelected.clear();
    }

    std::vector<const char *> FixStringListSelector::select(const std::vector<const char *> &candidate) const {
        return mSelected;
    }


    RequiredAndOptionalStringListSelector::RequiredAndOptionalStringListSelector(const std::vector<const char *> &required, const std::vector<const char *> &optional)
            : mRequired(required), mOptional(optional) {

    }

    RequiredAndOptionalStringListSelector::~RequiredAndOptionalStringListSelector() {
        mRequired.clear();
        mOptional.clear();
    }

    std::vector<const char *> RequiredAndOptionalStringListSelector::select(const std::vector<const char *> &candidate) const {
        std::vector<const char *> result;

        // 自定义比较函数：比较字符串内容
        auto compareStrings = [](const char *a, const char *b) {
            return std::strcmp(a, b) == 0;
        };

        // 1. 检查 mRequired 中的所有字符串是否都在 candidate 中
        for (const char *requiredStr: mRequired) {
            auto it = std::find_if(candidate.begin(), candidate.end(), [&](const char *c) {
                return compareStrings(c, requiredStr);
            });
            if (it == candidate.end()) {
                // 如果某个必需的字符串不在 candidate 中，抛出异常
                throw std::runtime_error("Required string not found in candidate: " + std::string(requiredStr));
            }
            // 如果找到，则添加到返回值中
            result.push_back(requiredStr);
        }

        // 2. 检查 mOptional 中的所有字符串是否在 candidate 中
        for (const char *optionalStr: mOptional) {
            auto it = std::find_if(candidate.begin(), candidate.end(), [&](const char *c) {
                return compareStrings(c, optionalStr);
            });
            if (it == candidate.end()) {
                // 如果某个可选的字符串不在 candidate 中，打日志提示
                LOG_W("Optional string not found in candidate: %s", optionalStr);
            } else {
                // 如果找到，则添加到返回值中
                result.push_back(optionalStr);
            }
        }

        return result;
    }


    LambdaStringListSelector::LambdaStringListSelector(std::function<std::vector<const char *>(const std::vector<const char *> &)> selector)
            : mSelector(std::move(selector)) {

    }

    LambdaStringListSelector::~LambdaStringListSelector() = default;

    std::vector<const char *> LambdaStringListSelector::select(const std::vector<const char *> &candidate) const {
        return mSelector(candidate);
    }

} // common