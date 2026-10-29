// SPDX-License-Identifier: GPL-2.0-or-later
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <shader_compiler/backend/spirv/emit_spirv.h>
#include <shader_compiler/common/settings.h>
#include <shader_compiler/frontend/maxwell/translate_program.h>
#include <shader_compiler/host_translate_info.h>
#include "environment.h"

// The production SKYrus ShaderManager supplies the same three logging functions.
namespace Shader::Log {
void Debug(const std::string&) {}
void Warn(const std::string& value) { std::cerr << value << '\n'; }
void Error(const std::string& value) { std::cerr << value << '\n'; }
}

static void CheckModule(const std::vector<u32>& words, u32 version) {
    if (words.size() < 5 || words[0] != 0x07230203U || words[1] != version) {
        throw std::runtime_error("Invalid SPIR-V module header");
    }
    bool entry{}, function{}, label{}, returned{};
    for (size_t i = 5; i < words.size();) {
        const u32 count = words[i] >> 16;
        const u32 opcode = words[i] & 0xffffU;
        if (!count || i + count > words.size()) throw std::runtime_error("Invalid SPIR-V word count");
        entry |= opcode == 15;
        function |= opcode == 54;
        label |= opcode == 248;
        returned |= opcode == 253;
        i += count;
    }
    if (!entry || !function || !label || !returned) throw std::runtime_error("Incomplete shader module");
}

int main(int argc, char** argv) {
    Shader::Settings::values = {.renderer_debug = true};
    for (const u32 version : {0x00010000U, 0x00010400U}) {
        for (const auto stage : {Shader::Stage::Compute, Shader::Stage::VertexB, Shader::Stage::Fragment}) {
            Shader::ObjectPool<Shader::Maxwell::Flow::Block> flow_pool;
            Shader::ObjectPool<Shader::IR::Inst> inst_pool;
            Shader::ObjectPool<Shader::IR::Block> block_pool;
            TestEnvironment env{stage};
            Shader::Maxwell::Flow::CFG cfg{env, flow_pool, Shader::Maxwell::Location{env.entry}};
            const Shader::HostTranslateInfo host{.support_snorm_render_buffer = true};
            auto program = Shader::Maxwell::TranslateProgram(inst_pool, block_pool, env, cfg, host);
            const Shader::Profile profile{.supported_spirv = version, .unified_descriptor_binding = true};
            Shader::Backend::Bindings bindings;
            const auto words = Shader::Backend::SPIRV::EmitSPIRV(profile, {}, program, bindings);
            CheckModule(words, version);
            if (argc > 1) {
                std::filesystem::create_directories(argv[1]);
                const auto file = std::filesystem::path{argv[1]} /
                    (std::to_string(version) + "-" + std::to_string(static_cast<u32>(stage)) + ".spv");
                std::ofstream stream{file, std::ios::binary};
                stream.write(reinterpret_cast<const char*>(words.data()), words.size() * sizeof(u32));
                if (!stream) throw std::runtime_error("Could not save SPIR-V fixture");
            }
            std::cout << "Maxwell -> IR -> SPIR-V " << std::hex << version << " stage "
                      << static_cast<u32>(stage) << ": " << std::dec << words.size() << " words\n";
        }
    }
}
