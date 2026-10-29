// SPDX-License-Identifier: GPL-2.0-or-later
#include <shader_compiler/common/settings.h>

Shader::Settings::Values* SettingsFromOtherTranslationUnit() {
    Shader::Settings::values.renderer_debug = true;
    Shader::Settings::values.resolution_info.up_scale = 3;
    return &Shader::Settings::values;
}
