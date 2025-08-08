//
// Physically-based airborne sound transmission model (angle- and frequency-dependent)
// Based on Imran, Heimes, & Vorländer (2021), Acta Acustica 5:19
//

#include "transmission_model.h"

#include <algorithm>
#include <cmath>

namespace ipl {

static constexpr double kSpeedOfSound = 343.0;     // m/s

// Helper: square and clamp
template <typename T>
static inline T clamp(T v, T lo, T hi)
{
    return std::max(lo, std::min(hi, v));
}

double AngleFromRayAndNormal(double rayDirDotNormal)
{
    // rayDirDotNormal = dot(-ray.direction, normal) typically
    auto x = clamp(rayDirDotNormal, -1.0, 1.0);
    auto theta = std::acos(x);
    // enforce [0, pi/2]
    if (theta < 0.0) theta = 0.0;
    if (theta > M_PI_2) theta = M_PI_2;
    return theta;
}

// Critical frequency for a thin plate (approx.)
// fc = (c^2 / (2*pi)) * sqrt(m' / D)
// where m' = rho * h (mass per unit area), D = E h^3 / (12(1 - nu^2))
double ComputeCriticalFrequencyHz(const MaterialEx& m)
{
    if (m.criticalFreq_Hz > 0.0)
        return m.criticalFreq_Hz;

    const double massPerArea = m.density_kgpm3 * m.thickness_m; // kg/m^2
    const double bendingStiffness = (m.youngsModulus_Pa * std::pow(m.thickness_m, 3)) / (12.0 * (1.0 - m.poissonRatio * m.poissonRatio));
    if (massPerArea <= 0.0 || bendingStiffness <= 0.0)
        return 0.0;

    const double term = std::sqrt(massPerArea / bendingStiffness);
    const double fc = (kSpeedOfSound * kSpeedOfSound / (2.0 * M_PI)) * term;
    return fc;
}

// Simplified transmission per Imran2021 trends:
// - Below mass–spring resonance: low τ
// - Mass-law region (below fc): TL ≈ 20 log10(m' f) - 47 (approx.) → τ = 10^(-TL/10)
// - Near coincidence (around fc): coincidence dip → higher τ (lower TL)
// - Above fc: radiation efficiency increases; use a gentle slope.
static double MassLawTau(double freqHz, double massPerArea)
{
    // Guard
    if (freqHz <= 0.0 || massPerArea <= 0.0)
        return 1.0;
    // Approximate transmission loss (dB) mass law for single leaf
    // TL ≈ 20 log10(m' f) - 47  (m' in kg/m^2, f in Hz)
    const double TL = 20.0 * std::log10(massPerArea * freqHz) - 47.0;
    const double tau = std::pow(10.0, -TL / 10.0);
    return clamp(tau, 0.0, 1.0);
}

double RadiationEfficiencyFinitePlate(double /*thetaRad*/, double /*freqHz*/, const MaterialEx& /*m*/, const PlateGeom& g)
{
    // Placeholder: simple area-based rolloff to avoid over-radiation for tiny plates.
    // A better model could use modal density and radiation efficiency curves.
    const double area = g.width_m * g.height_m;
    if (area <= 0.0)
        return 1.0;
    // Soft saturation: larger plates ~1, very small plates reduced
    const double scale = std::tanh(area / 0.25); // 0.5 m^2 scale
    return clamp(scale, 0.2, 1.0);
}

double TransmissionTau(double thetaRad, double freqHz, const MaterialEx& m)
{
    // If physical params are not set, fall back to mid-band legacy average.
    const bool hasPhysical = (m.thickness_m > 0.0 && m.density_kgpm3 > 0.0 && m.youngsModulus_Pa > 0.0);
    if (!hasPhysical)
    {
        // Map frequency to legacy 3 bands: 0..800, 800..8k, >8k (as in Steam Audio docs)
        if (freqHz < 800.0)
            return clamp(static_cast<double>(m.legacyTransmissionBands[0]), 0.0, 1.0);
        else if (freqHz < 8000.0)
            return clamp(static_cast<double>(m.legacyTransmissionBands[1]), 0.0, 1.0);
        else
            return clamp(static_cast<double>(m.legacyTransmissionBands[2]), 0.0, 1.0);
    }

    // Angle-dependent correction: effective path increases by 1/cos(theta),
    // and radiation into air reduces at grazing. Use a simple cosine factor.
    const double cosTheta = std::cos(clamp(thetaRad, 0.0, M_PI_2));
    const double angleFactor = clamp(cosTheta, 0.15, 1.0); // avoid singular behavior near 90°

    // Core plate model pieces
    const double mprime = m.density_kgpm3 * m.thickness_m; // kg/m^2
    const double fc = ComputeCriticalFrequencyHz(m);
    const double eta = (m.lossFactor > 0.0) ? m.lossFactor : 0.02;

    // Base τ via mass law away from coincidence
    double tau = MassLawTau(freqHz, mprime);

    // Coincidence correction: around fc, reduce TL → increase τ.
    if (fc > 0.0 && freqHz > 0.0)
    {
        const double r = freqHz / fc;
        // Broad bell around r=1; width controlled by eta
        const double width = std::max(0.2, 2.5 * eta + 0.3);
        const double peakGainLinear = 4.0; // up to ~+6 dB reduction of TL → ~x4 τ
        const double boost = 1.0 + (peakGainLinear - 1.0) * std::exp(-0.5 * std::pow((std::log10(r) / width), 2.0));
        tau = clamp(tau * boost, 0.0, 1.0);
    }

    // High frequency gentle slope (stiffen slightly):
    if (fc > 0.0 && freqHz > fc)
    {
        const double over = freqHz / fc;
        const double soften = 1.0 / (1.0 + 0.15 * std::log10(over + 1.0));
        tau = clamp(tau * soften, 0.0, 1.0);
    }

    // Apply angle factor
    tau = clamp(tau * angleFactor, 0.0, 1.0);

    return tau;
}

}


