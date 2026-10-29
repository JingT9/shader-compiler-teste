// SPDX-License-Identifier: MPL-2.0
// Copyright © 2021 Skyline Team and Contributors (https://github.com/skyline-emu/)

#pragma once

#include <stdexcept>

#include <exception>
#include <shader_compiler/common/log.h>

#define ASSERT(condition) do { if (!(condition)) throw std::runtime_error("Cannot fulfill condition: " #condition); } while (false)

#ifdef NDEBUG
#define DEBUG_ASSERT(condition) ((void)0)
#else
#define DEBUG_ASSERT(condition) ASSERT(condition)
#endif
