//
// MaterialEx registry implementation
//

#include "material_ex_registry.h"

namespace ipl {

MaterialExRegistry& MaterialExRegistry::instance()
{
    static MaterialExRegistry gInstance;
    return gInstance;
}

void MaterialExRegistry::setFor(const Material* materialPtr, const MaterialEx& materialEx)
{
    if (!materialPtr)
        return;
    std::lock_guard<std::mutex> lock(mMutex);
    mMap[materialPtr] = materialEx;
}

const MaterialEx* MaterialExRegistry::getFor(const Material* materialPtr) const
{
    if (!materialPtr)
        return nullptr;
    std::lock_guard<std::mutex> lock(mMutex);
    auto it = mMap.find(materialPtr);
    if (it == mMap.end())
        return nullptr;
    return &it->second;
}

void MaterialExRegistry::removeFor(const Material* materialPtr)
{
    if (!materialPtr)
        return;
    std::lock_guard<std::mutex> lock(mMutex);
    mMap.erase(materialPtr);
}

void MaterialExRegistry::removeFor(const Material* const* materialPtrs, int count)
{
    if (!materialPtrs || count <= 0)
        return;
    std::lock_guard<std::mutex> lock(mMutex);
    for (int i = 0; i < count; ++i)
    {
        mMap.erase(materialPtrs[i]);
    }
}

}


