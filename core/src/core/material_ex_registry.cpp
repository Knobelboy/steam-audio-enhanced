//
// MaterialEx registry implementation
//

#include "material_ex_registry.h"
#include "../acoustics/transmission_model.h"

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
        mTauCache.erase(materialPtrs[i]);
    }
}

void MaterialExRegistry::getTauBands(const Material* materialPtr,
                                     double thetaRad,
                                     const MaterialEx& materialEx,
                                     float outTau[3])
{
    if (!materialPtr || !outTau)
        return;

    const double clampedTheta = std::max(0.0, std::min(thetaRad, M_PI_2));
    const int bin = static_cast<int>(std::floor((clampedTheta / (M_PI_2)) * kAngleBins));
    const int binIdx = std::max(0, std::min(kAngleBins - 1, bin));

    std::lock_guard<std::mutex> lock(mMutex);
    TauCache3& cache = mTauCache[materialPtr];
    if (!cache.initialized[binIdx])
    {
        const double fCenters[3] = { 200.0, 2000.0, 16000.0 };
        for (int j = 0; j < 3; ++j)
        {
            const double tau = acoustics::TransmissionTau(clampedTheta, fCenters[j], materialEx);
            cache.values[binIdx][j] = static_cast<float>(std::max(0.0, std::min(1.0, tau)));
        }
        cache.initialized[binIdx] = true;
    }

    outTau[0] = cache.values[binIdx][0];
    outTau[1] = cache.values[binIdx][1];
    outTau[2] = cache.values[binIdx][2];
}

}


