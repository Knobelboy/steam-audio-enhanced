Steam Audio Enhanced – Discovery Notes (integration points)

- Direct path simulation and transmission
  - core/src/core/direct_simulator.cpp → class `DirectSimulator`
    - `simulate(...)` fills `DirectSoundPath` including `distanceAttenuation`, `airAbsorption[3]`, `delay`, `directivity`, `occlusion`, and `transmission[3]`.
    - `transmission(...)` traces between listener and source, alternates ray directions, finds closest hits, and accumulates a PRODUCT of per-material `transmission[3]` across hits. If >1 hit, returns sqrt(product) to avoid double-counting paired interfaces.
    - Current transmission is purely 3-band and angle-independent; no plate physics.

- Direct audio effect (where filters/gains are applied)
  - core/src/core/direct_effect.cpp → class `DirectEffect`
    - `calculateGainAndEQ(...)` composes overall gain and per-band EQ from direct path values.
    - If `TransmissionType::FreqDependent`, the three-band `transmission[3]` is applied into `eqCoeffs[3]` with occlusion mixing; then `EQEffect::normalizeGains` is used to keep stability.
    - This is the hook for introducing a physically based τ(θ,f) response mapped to a filter (multi-band EQ or FIR) under a runtime toggle.

- Material definitions and usage
  - C API struct: `IPLMaterial` in `core/src/core/phonon.h` (and mirrored in `unity/include/phonon/phonon.h`, `fmod/include/...`, `wwise/include/...`)
    - Fields: `absorption[3]`, `scattering`, `transmission[3]` (fractions).
  - Core runtime: `core/src/core/material.h` → class `Material` with the same three-band layout.
  - Serialization: `core/src/core/material.fbs` stores `absorption:[float:3]`, `scattering:float`, `transmission:[float:3]` and is used by `static_mesh.fbs` and `scene.fbs`.
  - Unity C# bindings:
    - `SteamAudioMaterial.cs` exposes low/mid/high absorption, scattering, low/mid/high transmission in the inspector.
    - `SteamAudio.cs` defines interop struct `SteamAudio.Material` matching the native layout (sequential, 7 floats).

- Where Unity passes materials to native
  - `StaticMeshSettings` in `SteamAudio.cs` includes `numMaterials` and a pointer to an array of `Material` structs.
  - Native creation path: `core/src/core/api_scene.cpp` → `CScene::createStaticMesh` reads `settings->materials` as an array of `ipl::Material` and constructs the static mesh.

- Simulation manager
  - `core/src/core/simulation_manager.cpp` calls `mDirectSimulator->simulate(...)` with `numTransmissionRays`, `occlusionType`, etc.

Key integration strategy

- Keep backward compatibility: do not change `IPLMaterial` or the serialized `Material` schema. Introduce an optional extended material path for plate parameters.
  - Add a new internal, optional `MaterialEx` store keyed by (scene, static mesh, material index) with fields: thickness, density, Young’s modulus, Poisson’s ratio, loss factor, and optionally derived `f_c`.
  - Provide new C API calls to set/get these optional extended properties from Unity without breaking existing `StaticMeshSettings` layout.

- New acoustics module
  - Create `core/src/acoustics/transmission_model.h/.cpp` implementing τ(θ,f) per Imran, Heimes, & Vorländer (2021) with:
    - `double TransmissionTau(double thetaRad, double freqHz, const MaterialEx& m);`
    - `double RadiationEfficiencyFinitePlate(double thetaRad, double freqHz, const MaterialEx& m, const PlateGeom& g);`
    - Helpers for mass law region, below/above critical frequency, resonance/forced response handling.

- Integration points
  - In `DirectSimulator::transmission(...)`: when a transmissive hit is found, compute θ from ray and `hit.normal`, and accumulate frequency-dependent τ based on the extended material if present; otherwise fall back to existing 3-band values.
  - In `DirectEffect::calculateGainAndEQ(...)`: add a runtime mode switch:
    - Legacy (default): current 3-band behavior unchanged.
    - Physically based: sample τ(f) and map to filter:
      - Fast path: N-band EQ (8–16 bands) mapped onto `EQEffect` with cached band gains.
      - High-fidelity: generate minimum-phase FIR from τ(f) and apply via a new convolution effect (optional).

- Unity integration (high level)
  - Add inspector fields on `SteamAudioMaterial` for thickness [m], density [kg/m³], Young’s modulus [Pa], Poisson’s ratio [-], loss factor [-]. Defaults chosen for backward compatibility.
  - Add a project-level toggle: “Physically Based Transmission” and quality knobs (secondary source grid size, LOD).
  - Expose an editor-only visualization of τ(f) for selected surface and angle.

- Secondary source (surface-source) rendering
  - Hook after a transmissive direct hit is detected to spawn a small grid of secondary sources on the receiver side of the surface polygon, weighted and energy-consistent with τ.
  - Use existing direct path/HRTF pipeline for summation. Include quality/LOD and culling.

Notes
- Bands are defined in `bands.h`/`bands.cpp` (3 bands today). We will introduce additional fixed bands in the new module without modifying legacy banding.
- `Hit` contains `normal` and `material` pointer; θ is computed via `acos(dot(-ray.direction, normal))` (clamped).
- All changes should be feature-gated; when disabled or when extended materials are unset, behavior remains bitwise-identical to legacy.

