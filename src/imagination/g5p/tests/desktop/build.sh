#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Generated-by: LLM. Manual standalone rungs; no default build/install effects.
set -euo pipefail
P=$(cd "$(dirname "$0")" && pwd)
O=${1:?output directory};mkdir -p "$O"
FLAGS=()
if [ -n "${DGL_PREFIX:-}" ];then FLAGS=(-I"$DGL_PREFIX/include" -L"$DGL_PREFIX/lib64" -Wl,-rpath-link,"$DGL_PREFIX/lib64");fi
for bin in gl3-ladder desktop-probe mrt8-gl gl32-rungs;do
 ${CC:-cc} -std=gnu11 -O1 -g0 -Wall -Wextra -Werror "${FLAGS[@]}" "$P/$bin.c" -o "$O/$bin" -lEGL
done
${CC:-cc} -std=gnu11 -O1 -g0 -Wall -Wextra -Werror "${FLAGS[@]}" "$P/punch-ds.c" -o "$O/punch-ds" -lEGL -lGLESv2
bash "$P/../formats/build.sh" "$O"
glslangValidator -V --target-env vulkan1.0 --vn spv_geometry "$P/refusal.geom" -o "$O/geometry-spirv.h" >/dev/null
glslangValidator -V --target-env vulkan1.0 --vn spv_depth_vert "$P/primitive.vert" -o "$O/depth-spirv.h" >/dev/null
for bin in conditional-vk gs-refusal-vk;do
 ${CC:-cc} -std=gnu11 -O1 -g0 -Wall -Wextra -Werror -I"$O" -I"$P/../formats" "$P/$bin.c" -o "$O/$bin" -lvulkan -lm
done
