# A9 standalone desktop qualification rungs

Generated-by: LLM

These MIT programs are compiled manually and do not participate in Mesa's default build or installation. They contain source shaders and CPU references, not captured GPU programs. Build with `bash src/imagination/g5p/tests/desktop/build.sh OUT`; optionally set DGL_PREFIX to the candidate's installed `/usr` prefix. SPIR-V headers are generated locally with glslangValidator and remain build outputs.

Select the candidate ICD and EGL/Gallium libraries, use surfaceless EGL and Zink, and opt into the experimental Vulkan driver with `PVR_I_WANT_A_BROKEN_VULKAN_DRIVER=1`. No GL version or extension override is required. Every pixel test reads the rendered image; `--record-only`, `--no-verify` or `--record` modes are frontend recordings and never certify pixels.

| Unit | Invocation / scope |
|---|---|
| Default profile | `desktop-probe`; the version string alone is not conformance. |
| RGBA32 integer | `gl3-ladder 1 core --uint32-only`, `--sint32-only`, and both with `compat` instead of `core`. |
| Integer clear boundary | Existing `formats` suite, PVR_G5P_FORMATS=probe for RG32 and RGBA16UI; narrow rendering beyond CLEAR is refused. |
| MRT5..8 | `PVR_G5P_MRT8=probe mrt8-gl`:104 images,26,624 pixels, actual5/6/7/8 attachments; clear/draw/LOAD/fractional patterns. |
| Logic | `PVR_G5P_LOGIC_OP=probe gl32-rungs logic-op`:all16 operators plus masked-channel controls, normalized single-sample colour only. |
| Sample fetch | `PVR_G5P_MS_TEXTURE=probe gl32-rungs ms-fetch`:20 edge/sample references. |
| Sample mask | Also PVR_G5P_SAMPLE_MASK=probe; `gl32-rungs sample-mask`:144 pipeline/shader/intersection references. |
| Sample ID | Also PVR_G5P_SAMPLE_SHADING=probe; `gl32-rungs sample-shading`:4 stored sample colours, minSampleShading=1 without interpolation/DS/blend. |
| Primary predicates | `PVR_G5P_CONDITIONAL=probe conditional-vk`:12 cases/14 images, unconditional clears and continuation LOAD. |
| GS rejection | `gs-refusal-vk`:geometryShader=false, named pipeline rejection, zero submissions. |
| Discard/DS | `punch-ds RUNG`:exact binary colours; depth-write, depth-two-first, depth-readonly, depth-order, depth-order-load, depth-two-order, depth-blend-order, stencil-colour, stencil-mask0-blend, stencil-mask0-blend-load, stencil-blend, stencil-two-mask0-blend. Requires the separate punch-through topic. |

The publication capability guards disable sampleRateShading by default. For the GLSL sample-mask test, set both PVR_G5P_SAMPLE_MASK=probe and PVR_G5P_SAMPLE_SHADING=probe in addition to the sampled-image probe. The latter exposes GL_ARB_sample_shading, which declares gl_SampleMask; the mask pipeline itself still requests ordinary fragment frequency.

A9 hardware evidence: integer-r4 plus desktop batch2/batch3; batch3 passed the104 MRT,144 mask and4 sample-ID images. The other retained GL32 units include26 cube references,16 RGTC images and8 synchronization paths. Border and strict-limit diagnostics remain failing and do not qualify those features. Depth-clamp and native GS execution are excluded from this driver series. Probe-only feature scopes must not be promoted to complete default API support from these subsets.

The punch topic separately passed all12 exact witnesses and13 arbitrary-colour blend-ladder processes; the same recipe on proven RAM/RT policy fixed full-panel KMS Hyprland blur/drag. A live compositor result is separate from pixel qualification and driver conformance.
