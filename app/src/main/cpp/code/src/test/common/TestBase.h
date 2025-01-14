//
// Created by leixing on 2025/1/4.
//

#pragma once

#include <string>
#include <utility>

namespace test {
    class TestBase {
    private:

        const std::string mName; // Name 字段

    public:
        // 构造函数，初始化 name 字段
        explicit TestBase(std::string name) : mName(std::move(name)) {}

        virtual ~TestBase() = default;

        // 纯虚函数
        virtual void init() = 0;

        virtual bool isReady() = 0;

        virtual void drawFrame() = 0;

        virtual void cleanup() = 0;

        // 返回 name 字段
        [[nodiscard]]
        const std::string &getName() const {
            return mName;
        }
    };
}
