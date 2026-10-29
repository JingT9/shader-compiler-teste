// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// Integration-only settings contract. Zuyu's shader code reads ::Settings::values;
// SKYrus configures ::Shader::Settings::values. Both names refer to one state.
// These are the exact settings members read by the supplied Zuyu shader sources.
#include <cstdint>

namespace Shader::Settings {
struct Values {
    bool renderer_debug{};
    bool disable_shader_loop_safety_checks{};
    struct ResolutionScalingInfo {
        std::uint32_t up_scale{1};
        std::uint32_t down_shift{};
        float up_factor{1.0f};
        float down_factor{1.0f};
        bool active{};
        bool downscale{};
    } resolution_info{};
};
inline Values values{};
}
namespace Settings {
inline auto& values = ::Shader::Settings::values;
}
