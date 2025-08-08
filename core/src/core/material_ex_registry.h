//
// Registry for extended material parameters (MaterialEx), keyed by Material*.
// Keeps optional physical properties for transmission modeling without changing
// the legacy Material ABI or serialized formats.
//
#pragma once

#include <unordered_map>
#include <mutex>

#include "material.h"
#include "../acoustics/transmission_model.h"

namespace ipl {

class MaterialExRegistry
{
public:
    static MaterialExRegistry& instance();

    void setFor(const Material* materialPtr, const MaterialEx& materialEx);
    const MaterialEx* getFor(const Material* materialPtr) const;

    // Remove a single entry.
    void removeFor(const Material* materialPtr);

    // Remove many entries at once (used when a static mesh is destroyed).
    void removeFor(const Material* const* materialPtrs, int count);

private:
    MaterialExRegistry() = default;
    MaterialExRegistry(const MaterialExRegistry&) = delete;
    MaterialExRegistry& operator=(const MaterialExRegistry&) = delete;

    struct PtrHash { size_t operator()(const Material* p) const noexcept { return std::hash<const void*>{}(p); } };
    struct PtrEq { bool operator()(const Material* a, const Material* b) const noexcept { return a == b; } };

    mutable std::mutex mMutex;
    std::unordered_map<const Material*, MaterialEx, PtrHash, PtrEq> mMap;
};

// Convenience free functions
inline const MaterialEx* GetMaterialEx(const Material* materialPtr)
{
    return MaterialExRegistry::instance().getFor(materialPtr);
}

inline void SetMaterialEx(const Material* materialPtr, const MaterialEx& materialEx)
{
    MaterialExRegistry::instance().setFor(materialPtr, materialEx);
}

}


