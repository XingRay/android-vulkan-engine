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

#ifndef NCV_VULKAN_CONTEXT_HPP
#define NCV_VULKAN_CONTEXT_HPP

#include <vk_util/vk_helpers.hpp>
#include <vk_util/vk_version.hpp>

namespace graphics {
    class vulkan_context {
    public:
        constexpr static uint32_t box_width = 70u;
        constexpr static uint32_t col_width = 11u;

        typedef ::vk_util::logger::align align;

        vulkan_context();

        virtual ~vulkan_context();

    protected:

        template<typename T>
        void log_system_info() const;

        std::vector<vk::LayerProperties> m_avail_layers;
        std::vector<std::vector<vk::ExtensionProperties>> m_avail_layer_extensions;
        std::vector<vk::ExtensionProperties> m_avail_extensions;

        void *m_libvulkan = nullptr;
    };

    template<typename T = void>
    inline void vulkan_context::log_system_info() const {
        auto cc = align::ccenter;

        if (m_avail_extensions.size() > 0) {

            for (auto &extension: m_avail_extensions) {

            }
        } else {

        }


        if (m_avail_layers.size() > 0) {

            for (uint32_t i = 0; i < m_avail_layers.size(); ++i) {

                if (m_avail_layer_extensions[i].size() > 0) {
                    for (auto &extension: m_avail_layer_extensions[i]) {}
                } else {}
            }
        } else {

        }
    }
}

#endif //NCV_VULKAN_CONTEXT_HPP