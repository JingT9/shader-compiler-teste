// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// The SKYrus host may put its own common/ include directory first in -I.
// This header is looked up relative to Zuyu's src/shader_recompiler/*.h,
// so forward explicitly to the UNMODIFIED common_funcs.h from the same Zuyu
// revision, not to the host's unrelated common/common_funcs.h.
//
// In particular, ProgramHeader requires INSERT_PADDING_BYTES_NOINIT and
// INSERT_PADDING_WORDS_NOINIT from the original Zuyu common header.
#include "../../../third_party/zuyu-original/src/common/common_funcs.h"
