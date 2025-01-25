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

#include <vk_util/vk_helpers.hpp>

using namespace ::std;
using namespace ::vk;
using namespace ::utilities;

namespace vk_util {
    VKAPI_ATTR Bool32 VKAPI_CALL message_callback(DebugUtilsMessageSeverityFlagBitsEXT a_severity,
                                                  DebugUtilsMessageTypeFlagsEXT a_msg_type,
                                                  const DebugUtilsMessengerCallbackDataEXT *a_dbg_data,
                                                  void *a_user_data) {
        string general_type = "General";
        string performance_type = "Performance";
        string validation_type = "Validation";
        string msg_type;

        if (a_msg_type & DebugUtilsMessageTypeFlagBitsEXT::eGeneral)
            msg_type = general_type;
        else if (a_msg_type & DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
            msg_type = performance_type;
        else if (a_msg_type & DebugUtilsMessageTypeFlagBitsEXT::eValidation)
            msg_type = validation_type;

        if (a_severity & DebugUtilsMessageSeverityFlagBitsEXT::eInfo) {}
        else if (a_severity & DebugUtilsMessageSeverityFlagBitsEXT::eWarning) {}
        else if (a_severity & DebugUtilsMessageSeverityFlagBitsEXT::eVerbose) {}
        else {
            // Special treatment for a specific validation error in order to avoid premature app exit
            // when validation layers are enabled. For an explanation read the following.
            //
            // It seems that for the camera-specific format (vulkan image resource external format = 506)
            // of the development device (Nokia 6.1, Adreno 508, Driver 512.415.0, Vulkan API Version 1.1.87),
            // the feature flag VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT is missing as reported by the hardware
            // buffer format properties. Although this external format seems to be lacking the mentioned
            // feature, the application works if the validation check is omitted. This is weird because
            // in order for an external image buffer of unknown to vulkan format, to be mapped in vulkan
            // space and consumed inside a vulkan context (via the use of a ycbcr sampler), its format
            // MUST support sampling feature.
            //
            // For more details see below:
            // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#samplers-YCbCr-conversion
            // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#VUID-VkImageViewCreateInfo-usage-02274
            //
            // Maybe this is an error in camera format definitions of the specific device. Don't have
            // the resources to test this in other devices (To be investigated).

            string msg_str(a_dbg_data->pMessage);

            array<string, 3> VUID_02274 = {
                    "VUID-VkImageViewCreateInfo-usage-02274",
                    "VK_FORMAT_UNDEFINED",
                    "VK_IMAGE_TILING_OPTIMAL"
            };

            auto ret_flag = VK_TRUE;

            if (msg_str.find(VUID_02274[0]) != string::npos) {
                if (msg_str.find(VUID_02274[1]) != string::npos && msg_str.find(VUID_02274[2]) != string::npos)
                    ret_flag = VK_FALSE;
            }

            return ret_flag;
        }

        return VK_FALSE;
    }

    string readable_size(size_t a_size) {
        static const vector<string> suffixes{{"B", "KB", "MB", "GB", "TB", "PB"}};
        size_t suffixIndex = 0;
        while (suffixIndex < suffixes.size() - 1 && a_size > 1024) {
            a_size >>= 10;
            ++suffixIndex;
        }
        stringstream buffer;
        buffer << a_size << " " << suffixes[suffixIndex];
        return buffer.str();
    }

    perf_timer::perf_timer(const string &a_func)
            : m_func_name{a_func}, m_clock{std::chrono::high_resolution_clock::now()} {}

    perf_timer::~perf_timer() {
        using namespace std::chrono;
        auto ns = duration_cast<nanoseconds>(high_resolution_clock::now() - m_clock);
    }

    u_long perf_timer::elapsed_ns() const {
        using namespace std::chrono;
        return (duration_cast<nanoseconds>(high_resolution_clock::now() - m_clock)).count();
    }

    void logger::box_top() const {
        box_line("┌", "┐");
    }

    void logger::box_bottom() const {
        box_line("└", "┘");
    }

    void logger::box_separator() const {
        box_line();
    }

    void logger::write_multiline(const char *a_str, const vector <string> &a_vec, char a_fill) const {
    }

    void logger::write_line(const char *a_str, int a_align, char a_fill) const {
    }

    void logger::write_line_2col(const char *a_lstr, const char *a_rstr, int a_align, char a_fill) const {
    }

    void logger::box_line(const char *a_start, const char *a_end, char a_fill) const {
    }
}