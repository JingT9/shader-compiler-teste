// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <stdexcept>
#include <shader_compiler/environment.h>

// Implements precisely the Environment contract used by SKYrus ShaderManager.
class TestEnvironment final : public Shader::Environment {
public:
    explicit TestEnvironment(Shader::Stage value = Shader::Stage::Compute) {
        stage = value;
        start_address = 0;
        is_propietary_driver = true;
        entry = value == Shader::Stage::Compute ? 8U : sizeof(Shader::ProgramHeader);
    }

    u64 ReadInstruction(u32 address) override {
        if (address != entry) {
            throw std::out_of_range("Unexpected Maxwell instruction read");
        }
        // EXIT with PT and FlowTest::T, following the scheduler word.
        return 0xe30000000007000fULL;
    }
    u32 ReadCbufValue(u32, u32) override { return 0; }
    Shader::TextureType ReadTextureType(u32) override { return Shader::TextureType::Color2D; }
    Shader::TexturePixelFormat ReadTexturePixelFormat(u32) override {
        return Shader::TexturePixelFormat::OTHER;
    }
    bool IsTexturePixelFormatInteger(u32) override { return false; }
    u32 ReadViewportTransformState() override { return 1; }
    u32 TextureBoundBuffer() const override { return 2; }
    u32 LocalMemorySize() const override { return 0; }
    u32 SharedMemorySize() const override { return 0; }
    std::array<u32, 3> WorkgroupSize() const override { return {1, 1, 1}; }
    bool HasHLEMacroState() const override { return false; }
    std::optional<Shader::ReplaceConstant> GetReplaceConstBuffer(u32, u32) override {
        return std::nullopt;
    }
    void Dump(u64 hash) override { last_dump = hash; }

    u32 entry{};
    u64 last_dump{};
};
