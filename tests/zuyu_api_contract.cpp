// SPDX-License-Identifier: GPL-2.0-or-later
// API contract only. No shader translation, IR or SPIR-V algorithms are changed.
#include <cassert>
#include <stdexcept>
#include <vector>
#include <shader_compiler/profile.h>
#include <shader_compiler/runtime_info.h>
#include <shader_compiler/shader_info.h>

int main() {
    Shader::Profile profile{};
    assert(profile.min_ssbo_alignment == 1);
    assert(profile.max_user_clip_distances == 8);
    assert(!profile.support_variable_pointers_storage_buffer);
    assert(!profile.has_broken_spirv_subgroup_shuffle);
    assert(profile.max_subgroup_size == 0);
    assert(!profile.has_broken_spirv_vector_access_chain);
    assert(!profile.disable_subgroup_shuffle);

    static_assert(static_cast<u32>(Shader::TexturePixelFormat::A8B8G8R8_UNORM) == 0);
    static_assert(static_cast<u32>(Shader::TexturePixelFormat::A8B8G8R8_SNORM) == 1);
    static_assert(static_cast<u32>(Shader::TexturePixelFormat::OTHER) == 0xffffffffu);

    Shader::RuntimeInfo runtime{};
    const std::vector<Shader::TransformFeedbackVarying> elements{
        {0, 16, 0, 4}, {1, 8, 4, 2},
    };
    runtime.SetTransformFeedbackVaryings(elements);
    assert(runtime.xfb_count == 2);
    assert(runtime.xfb_varyings[0].stride == 16);
    assert(runtime.xfb_varyings[1].offset == 4);

    bool rejected = false;
    try {
        runtime.SetTransformFeedbackVaryings(
            std::vector<Shader::TransformFeedbackVarying>(257));
    } catch (const std::length_error&) {
        rejected = true;
    }
    assert(rejected && runtime.xfb_count == 2);
}
