/* SPDX-License-Identifier: MIT
 * Generated-by: LLM
 * Qualification switches only; none changes default feature exposure.
 */
#ifndef PVR_G5P_DGL_PROBE_H
#define PVR_G5P_DGL_PROBE_H
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
static inline bool pvr_g5p_mrt8_probe(void)
{
   const char *v = getenv("PVR_G5P_MRT8");
   return v && !strcmp(v, "probe");
}
#endif
