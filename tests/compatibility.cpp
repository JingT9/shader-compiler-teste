// SPDX-License-Identifier: GPL-2.0-or-later
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <shader_compiler/backend/bindings.h>
#include <shader_compiler/common/bit_cast.h>
#include <shader_compiler/common/settings.h>
#include <shader_compiler/exception.h>
#include <shader_compiler/host_translate_info.h>
#include <shader_compiler/object_pool.h>
#include <shader_compiler/profile.h>
#include <shader_compiler/runtime_info.h>
#include "environment.h"

Shader::Settings::Values* SettingsFromOtherTranslationUnit();

static void Check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

struct Tracked {
    static inline int alive{};
    explicit Tracked(int value_) : value{value_} { ++alive; }
    ~Tracked() { --alive; }
    int value;
};

int main() {
    static_assert(!std::is_abstract_v<TestEnvironment>);
    static_assert(std::is_trivially_copyable_v<Shader::Backend::Bindings>);
    static_assert(sizeof(Shader::Backend::Bindings) == 7 * sizeof(u32));
    static_assert(!std::is_copy_constructible_v<Shader::ObjectPool<Tracked>>);

    Check(SettingsFromOtherTranslationUnit() == &Shader::Settings::values,
          "Host and compiler must share one settings object");
    Check(Shader::Settings::values.renderer_debug &&
              Shader::Settings::values.resolution_info.up_scale == 3,
          "Settings writes must cross translation units");

    TestEnvironment env;
    Check(env.IsPropietaryDriver() && env.IsProprietaryDriver(), "Driver aliases disagree");
    static_cast<Shader::Environment&>(env).Dump(1, 42);
    Check(env.last_dump == 42, "Dump overload must call the SKYrus override");

    Shader::HostTranslateInfo host{.needs_byte_swap_workaround = true};
    Shader::Profile profile{
        .support_variable_pointers_storage_buffer = true,
        .min_ssbo_alignment = 256,
        .max_user_clip_distances = 8,
        .has_broken_spirv_subgroup_shuffle = true,
        .max_subgroup_size = 64,
        .has_broken_spirv_vector_access_chain = true,
        .disable_subgroup_shuffle = true,
    };
    Check(host.needs_byte_swap_workaround && profile.min_ssbo_alignment == 256,
          "Missing SKYrus driver profile fields");
    Shader::RuntimeInfo runtime;
    runtime.xfb_varyings.push_back({});
    Check(runtime.xfb_varyings.size() == 1, "SKYrus transform-feedback vector is unavailable");

    const Shader::Backend::Bindings bindings{1, 2, 3, 4, 5, 6, 7};
    u32 raw[7]{};
    std::memcpy(raw, &bindings, sizeof(raw));
    for (u32 i = 0; i < 7; ++i) Check(raw[i] == i + 1, "Rust shader cache binding layout changed");
    Check(Common::BitCast<u32>(1.0f) == 0x3f800000U, "BitCast must preserve IEEE bits");

    {
        Shader::ObjectPool<Tracked> pool{2};
        for (int i = 0; i < 7; ++i) Check(pool.Create(i)->value == i, "Pool allocation failed");
        Check(Tracked::alive == 7, "Pool lost live objects");
        pool.ReleaseContents();
        Check(Tracked::alive == 0, "Pool reset leaked objects");
        Check(pool.Create(9)->value == 9, "Pool cannot be reused");
    }
    Check(Tracked::alive == 0, "Pool destruction leaked objects");
    const Shader::InvalidArgument diagnostic{"Stage {}", Shader::Stage::Compute};
    Check(std::string{diagnostic.what()}.find("Stage ") == 0, "Enum diagnostics failed");
    std::cout << "SKYrus API, settings linkage, bindings ABI and object-pool lifetime: passed\n";
}
