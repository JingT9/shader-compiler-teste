// SPDX-License-Identifier: GPL-2.0-or-later
// Integration test only. Does not alter any Zuyu shaders.
#include <cassert>
#include <shader_compiler/common/settings.h>
#include <common/settings.h>
int main() {
    static_assert(sizeof(Shader::Settings::Values::ResolutionScalingInfo) >= 18);
    assert(&Settings::values == &Shader::Settings::values);
    Settings::values.renderer_debug = true;
    assert(Shader::Settings::values.renderer_debug);
    Shader::Settings::values.resolution_info.up_scale = 2;
    assert(Settings::values.resolution_info.up_scale == 2);
}
