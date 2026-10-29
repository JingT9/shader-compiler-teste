// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
// SKYrus logging ABI; logging implementations live in the SKYrus application.
#include <string>
#include <fmt/format.h>

namespace Shader::Log {
void Debug(const std::string& message);
void Warn(const std::string& message);
void Error(const std::string& message);
}

#define LOG_DEBUG(tag, message, ...) ::Shader::Log::Debug(fmt::format("Shader Compiler (" #tag "): " message __VA_OPT__(,) __VA_ARGS__))
#define LOG_WARNING(tag, message, ...) ::Shader::Log::Warn(fmt::format("Shader Compiler (" #tag "): " message __VA_OPT__(,) __VA_ARGS__))
#define LOG_ERROR(tag, message, ...) ::Shader::Log::Error(fmt::format("Shader Compiler (" #tag "): " message __VA_OPT__(,) __VA_ARGS__))
