## Steam Audio Enhanced – TODO

Legend: [x] done, [ ] pending, [>] in progress, [~] blocked

### 0) Project hygiene
- [x] Create discovery notes (`DISCOVERY.md`) capturing integration points
- [x] Add initial acoustics module scaffolding (`acoustics/transmission_model.*`) and register in build

### 1) Data model and API (extend materials without breaking legacy)
- [>] Core: MaterialEx in-memory registry
  - [x] Define `MaterialEx` store keyed by (scene -> static mesh -> material index)
  - [x] Access helpers to query by `Hit` -> material index -> `MaterialEx` (via `Material*` key)
- [ ] Core C API (non-breaking) for extended parameters
  - [x] Functions: `iplStaticMeshSetMaterialEx`, `iplStaticMeshGetMaterialEx`, `iplStaticMeshClearMaterialEx`
  - [x] Validation layer: bounds/unit checks; safe defaults
  - [ ] Thread-safety: lock or document callsite expectations
- [ ] Unity bindings
  - [x] C#: P/Invoke for `iplStaticMeshSetMaterialEx/Get/Clear`
  - [x] Runtime: call setters when exporting static meshes (per material index)
  - [x] Inspector: add fields on `SteamAudioMaterial` (thickness [m], density [kg/m³], E [Pa], ν [-], η [-]) with tooltips/units
  - [x] Backward compatibility: if fields unset, do not call setters; fall back to legacy

### 2) Transmission math module
- [x] Create `TransmissionTau`, `RadiationEfficiencyFinitePlate`, helpers
- [>] Unit tests (C++)
  - [x] Angle dependence sanity (grazing vs. normal incidence)
  - [x] Edge cases (missing params → legacy bands; numeric stability)
  - [ ] Sanity trends vs. digitized curves (below/near/above critical frequency)
- [ ] Expose configurable constants (e.g., coincidence boost width/amount) with reasonable defaults

### 3) Integration into direct path
- [ ] Feature toggle
  - [ ] New runtime flag: “Physically Based Transmission” (project-level; Unity UI)
  - [ ] Mode A legacy (default), Mode B physically based
- [ ] Direct simulator changes
  - [ ] In `DirectSimulator::transmission`: compute θ from `hit.normal` and ray, look up `MaterialEx`
  - [ ] Fast path B1: map τ(θ,f) → fixed N bands (8–16)
  - [ ] Cache τ over (angle bins × frequency bands) per material
- [ ] Direct effect changes
  - [ ] If Mode B1: feed N-band gains into existing EQ or new multi-band wrapper
  - [ ] If Mode B2 (optional): min-phase FIR from τ(f); apply via convolution effect
  - [ ] Ensure normalization (`EQEffect::normalizeGains`) and smooth transitions

### 4) Secondary sources (surface-source rendering)
- [ ] Detection
  - [ ] On transmissive hit, retrieve surface plane and polygon (triangle/mesh patch)
- [ ] Spawning
  - [ ] Grid generation on receiver side (N×N), epsilon offset
  - [ ] Polygon clipping for points
  - [ ] Weighting (Gaussian/cosine), energy-consistent with τ across patch
  - [ ] Submit as virtual direct contributions (reuse HRTF/distance attenuation)
- [ ] Quality controls
  - [ ] Grid size control (3×3, 5×5), distance-based LOD
  - [ ] Culling threshold and coalescing close sources
  - [ ] Hard caps: max sources per hit and globally

### 5) Performance and stability
- [ ] Caches for τ vs (θ, f); per-surface normal bins
- [ ] SIMD in tight loops; avoid allocations in audio thread
- [ ] Precompute band edges for N-band mapping
- [ ] Telemetry (dev-only): CPU time, source counts

### 6) Unity integration and sample
- [ ] UI
  - [ ] Toggle: “Physically Based Transmission”
  - [ ] Quality knobs: secondary grid size, LOD thresholds
- [ ] Gizmos to visualize secondary sources on surfaces
- [ ] Demo scene: talker behind wall, traffic behind window; A/B toggle
- [ ] τ(f) visualization panel for selected surface and angle

### 7) Validation and evaluation utilities
- [ ] DnT computation from simulation output, export CSV
- [ ] Script to compare against paper figures (Figs. 7, 10, 11)
- [ ] Profiling overlay (Editor-only) to show CPU delta vs. legacy
- [ ] A/B listening test protocol (short doc)

### 8) Documentation
- [ ] `docs/phys-transmission.md`: overview, parameters, assumptions, units
- [ ] How to enable/configure; performance guidance
- [ ] Validation instructions, DnT computation, known limitations
- [ ] Reference the paper (DOI) and included PDF if present

---

### Next up (active)
- [>] 1.1 Core: `MaterialEx` registry (keys and storage layout)

Subtasks:
- [x] Define key type (scene pointer + static mesh handle + material index)
- [x] Store: unordered_map keyed by `Material*` → `MaterialEx`
- [x] API to fetch `MaterialEx*` from a `Hit` via `Hit.material`

Reasonable done criterion:
- Query and set paths unit-tested with fake meshes; no changes to FlatBuffers or `IPLMaterial` ABI.


