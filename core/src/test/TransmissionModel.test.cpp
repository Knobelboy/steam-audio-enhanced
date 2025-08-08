//
// Unit tests for physically-based transmission model
//

#include <catch.hpp>

#include <acoustics/transmission_model.h>

using namespace ipl;

TEST_CASE("TransmissionTau falls back to legacy bands when physical params unset", "[TransmissionModel]")
{
    MaterialEx m{};
    m.legacyTransmissionBands[0] = 0.2f;
    m.legacyTransmissionBands[1] = 0.5f;
    m.legacyTransmissionBands[2] = 0.8f;

    // thickness = 0 enforces fallback
    m.thickness_m = 0.0;

    REQUIRE(TransmissionTau(0.0, 100.0, m) == Approx(0.2).margin(1e-6));
    REQUIRE(TransmissionTau(0.0, 1000.0, m) == Approx(0.5).margin(1e-6));
    REQUIRE(TransmissionTau(0.0, 10000.0, m) == Approx(0.8).margin(1e-6));
}

TEST_CASE("TransmissionTau decreases with grazing angle (angle factor)", "[TransmissionModel]")
{
    MaterialEx m{};
    m.thickness_m = 0.01;         // 1 cm plate
    m.density_kgpm3 = 800.0;      // kg/m^3 (wood-like)
    m.youngsModulus_Pa = 5.0e9;   // Pa
    m.poissonRatio = 0.3;
    m.lossFactor = 0.02;

    const double f = 1000.0; // 1 kHz
    const double tau0 = TransmissionTau(0.0, f, m);
    const double tau60 = TransmissionTau(M_PI / 3.0, f, m);
    REQUIRE(tau0 >= tau60);
}

TEST_CASE("ComputeCriticalFrequencyHz is positive for valid plate", "[TransmissionModel]")
{
    MaterialEx m{};
    m.thickness_m = 0.01;
    m.density_kgpm3 = 7800.0;     // steel-like
    m.youngsModulus_Pa = 2.0e11;
    m.poissonRatio = 0.3;
    const double fc = ComputeCriticalFrequencyHz(m);
    REQUIRE(fc > 0.0);
}


