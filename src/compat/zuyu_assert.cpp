// SPDX-License-Identifier: GPL-2.0-or-later
// Only the Zuyu `common/assert.h` host hooks are supplied here. SKYrus has no
// `Settings::values.use_debug_asserts`; fail explicitly instead of inventing a
// mutable setting or silently allowing invalid IR state to continue.
#include <stdexcept>

void assert_fail_impl() {
    throw std::runtime_error("Zuyu shader compiler assertion failed");
}

[[noreturn]] void unreachable_impl() {
    throw std::runtime_error("Zuyu shader compiler reached unreachable code");
}
