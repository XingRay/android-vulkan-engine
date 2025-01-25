/*
 * Copyright 2020 Konstantinos Tzevanidis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef NCV_VK_HELPERS_HPP
#define NCV_VK_HELPERS_HPP

#include <utilities/helpers.hpp>
#include "vulkan/vulkan.hpp"

namespace vk_util
{
    class perf_timer
    {
    public:

        perf_timer(const std::string& a_func);
        ~perf_timer();
        u_long elapsed_ns() const;

    private:

        std::string m_func_name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_clock;
    };

    class logger
    {
    public:

        enum align {cleft = 0, ccenter, cright};

        void box_top() const;
        void box_bottom() const;
        void box_separator() const;
        void write_multiline(const char* a_str, const std::vector<std::string>& a_vec, char a_fill = ' ') const;
        void write_line(const char* a_str, int a_align = 0, char a_fill = ' ') const;
        void write_line_2col(const char* a_lstr, const char* a_rstr, int a_align = 0, char a_fill = ' ') const;

    private:

        void box_line(const char* a_start = "|", const char* a_end = "|", char a_fill = '-') const;

        uint32_t m_bw, m_cw;
    };

    VKAPI_ATTR vk::Bool32 VKAPI_CALL message_callback(vk::DebugUtilsMessageSeverityFlagBitsEXT a_severity,
        vk::DebugUtilsMessageTypeFlagsEXT a_msg_type, const vk::DebugUtilsMessengerCallbackDataEXT *a_dbg_data,
        void* a_user_data);

    std::string readable_size(size_t a_size);

    template <typename T>
    inline std::vector<std::string> split_to_vector(const T& a_vk_arg)
    {
        auto pos = std::string::npos;
        auto str = to_string(a_vk_arg);
        str = str.erase(0, str.find_first_not_of("{\t\n\v\f\r "));
        str = str.erase(str.find_last_not_of("{\t\n\v\f\r "));
        while((pos = str.find("| ")) != std::string::npos)
            str.erase(pos, 2);
        std::stringstream sstr(str);
        std::istream_iterator<std::string> begin{sstr}, end;
        return std::vector<std::string>(begin, end);
    }
}

#endif //NCV_VK_HELPERS_HPP
