# SPDX-License-Identifier: GPL-2.0-or-later
add_executable(skyrus_shader_compatibility compatibility.cpp settings_peer.cpp)
target_link_libraries(skyrus_shader_compatibility PRIVATE SKYrus::ShaderCompiler)
add_test(NAME skyrus_shader_compatibility COMMAND skyrus_shader_compatibility)

add_executable(skyrus_shader_smoke spirv_smoke.cpp)
target_link_libraries(skyrus_shader_smoke PRIVATE SKYrus::ShaderCompiler)
add_test(NAME skyrus_shader_smoke COMMAND skyrus_shader_smoke)
