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
static inline bool pvr_g5p_logic_probe(void)
{
   const char *v=getenv("PVR_G5P_LOGIC_OP");
   return v && !strcmp(v,"probe");
}
static inline bool pvr_g5p_ms_texture_probe(void)
{
   const char *v=getenv("PVR_G5P_MS_TEXTURE");
   return v && !strcmp(v,"probe");
}
static inline bool pvr_g5p_sample_mask_probe(void)
{
   const char *v=getenv("PVR_G5P_SAMPLE_MASK");
   return v && !strcmp(v,"probe");
}
static inline bool pvr_g5p_sample_shading_probe(void)
{
   const char *v=getenv("PVR_G5P_SAMPLE_SHADING");
   return v && !strcmp(v,"probe");
}
#endif
