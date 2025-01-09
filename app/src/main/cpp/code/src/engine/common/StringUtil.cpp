//
// Created by leixing on 2025/1/9.
//

#include "StringUtil.h"

#include <sstream>

namespace common {

    std::vector<uint32_t> StringUtil::parseVersion(const std::string& version){
        std::vector<uint32_t> numbers;
        std::istringstream iss(version);
        std::string token;

        // 按 '.' 分割字符串
        while (std::getline(iss, token, '.')) {
            try {
                // 将每个部分转换为整数
                uint32_t number = static_cast<uint32_t>(std::stoul(token));
                numbers.push_back(number);
            } catch (const std::exception&) {
                throw std::invalid_argument("Invalid version format: non-numeric value found");
            }
        }

        return numbers;
    }

} // common