// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
// Host logging adapter only. The unchanged Zuyu translation units emit logs
// through these macros; SKYrus already implements Shader::Log::{Debug,Warn,Error}.
#include <shader_compiler/common/log.h>

#ifndef LOG_DEBUG
#define LOG_DEBUG(tag, message, ...) LOG_DEBUG_IMPL(tag, message __VA_OPT__(,) __VA_ARGS__)
#define LOG_DEBUG_IMPL(tag, message, ...) ::Shader::Log::Debug(fmt::format("Zuyu (" #tag "): " message __VA_OPT__(,) __VA_ARGS__))
#endif
#ifndef LOG_INFO
#define LOG_INFO(tag, message, ...) ::Shader::Log::Debug(fmt::format("Zuyu (" #tag "): " message __VA_OPT__(,) __VA_ARGS__))
#endif
#ifndef LOG_WARNING
#define LOG_WARNING(tag, message, ...) ::Shader::Log::Warn(fmt::format("Zuyu (" #tag "): " message __VA_OPT__(,) __VA_ARGS__))
#endif
#ifndef LOG_ERROR
#define LOG_ERROR(tag, message, ...) ::Shader::Log::Error(fmt::format("Zuyu (" #tag "): " message __VA_OPT__(,) __VA_ARGS__))
#endif
#ifndef LOG_CRITICAL
#define LOG_CRITICAL(tag, message, ...) ::Shader::Log::Error(fmt::format("Zuyu (" #tag "): " message __VA_OPT__(,) __VA_ARGS__))
#endif
