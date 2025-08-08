//
// Physically-based airborne sound transmission model (angle- and frequency-dependent)
// Based on Imran, Heimes, & Vorländer (2021), Acta Acustica 5:19
//
#pragma once

#include <cstddef>

namespace ipl {

struct MaterialEx
{
    // Plate / wall physical parameters. Units in SI.
    double thickness_m {0.0};        // h [m]
    double density_kgpm3 {0.0};      // rho [kg/m^3]
    double youngsModulus_Pa {0.0};   // E [Pa]
    double poissonRatio {0.3};       // nu [-]
    double lossFactor {0.02};        // eta [-] structural damping loss factor

    // If > 0, use explicit critical frequency; else derive from other params.
    double criticalFreq_Hz {0.0};

    // Legacy 3-band fallback when physical params are not available / unset.
    float legacyTransmissionBands[3] {1.0f, 1.0f, 1.0f};
};

struct PlateGeom
{
    // Simple rectangular finite plate dimensions. Set to 0 for infinite/large plate assumption.
    double width_m {0.0};
    double height_m {0.0};
};

// Compute power transmission coefficient tau(theta, f) in linear scale [0..1].
// thetaRad in [0..pi/2], freqHz > 0. Returns [0, 1].
double TransmissionTau(double thetaRad, double freqHz, const MaterialEx& m);

// Optional finite plate radiation efficiency correction for windows etc.
double RadiationEfficiencyFinitePlate(double thetaRad, double freqHz, const MaterialEx& m, const PlateGeom& g);

// Utility: derive plate critical frequency from material parameters.
double ComputeCriticalFrequencyHz(const MaterialEx& m);

// Utility: clamp cos(theta) to valid range and produce angle from dot of ray and normal.
double AngleFromRayAndNormal(double rayDirDotNormal);

}


