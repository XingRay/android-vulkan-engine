//
// Created by leixing on 2025/1/8.
//

#pragma once

#include <vector>
#include <functional>

namespace common {

    class StringListSelector {
    public:
        virtual ~StringListSelector() = default;

        virtual std::vector<const char *> select(const std::vector<const char *> &candidate) const = 0;
    };

    class FixStringListSelector : public StringListSelector {
    private:
        std::vector<const char *> mSelected;

    public:
        FixStringListSelector(const std::vector<const char *> &selected);

        ~FixStringListSelector();

        std::vector<const char *> select(const std::vector<const char *> &candidate) const, override;
    };

    class RequiredAndOptionalStringListSelector : public StringListSelector {
    private:
        std::vector<const char *> mRequired;
        std::vector<const char *> mOptional;
    public:
        explicit RequiredAndOptionalStringListSelector(const std::vector<const char *> &required, const std::vector<const char *> &optional = {});

        ~RequiredAndOptionalStringListSelector();

        std::vector<const char *> select(const std::vector<const char *> &candidate) const, override;
    };


    class LambdaStringListSelector : public common::StringListSelector {
    private:
        std::function<std::vector<const char *>(const std::vector<const char *> &)> mSelector;

    public:
        explicit LambdaStringListSelector(std::function<std::vector<const char *>(const std::vector<const char *> &)> selector);

        ~LambdaStringListSelector();

        std::vector<const char *> select(const std::vector<const char *> &candidate) const, override;
    };

} // common
