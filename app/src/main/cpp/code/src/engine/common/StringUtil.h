//
// Created by leixing on 2025/1/9.
//

#pragma once

#include <string>
#include <vector>

namespace common {

    class StringUtil {
    public:
        static std::vector<uint32_t> parseVersion(const std::string &version);
    };

} // common
