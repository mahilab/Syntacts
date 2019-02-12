#include <TactorFX/ADSR.hpp>
#include <cmath>


ADSR::ADSR(void) {
    reset();
    setAttackRate(0);
    setDecayRate(0);
    setReleaseRate(0);
    setSustainLevel(1.0f);
    setTargetRatioA(0.3f);
    setTargetRatioDR(0.0001f);
}

ADSR::~ADSR(void) {
}

void ADSR::setAttackRate(float rate) {
    attackRate = rate;
    attackCoef = calcCoef(rate, targetRatioA);
    attackBase = (1.0f + targetRatioA) * (1.0f - attackCoef);
}

void ADSR::setDecayRate(float rate) {
    decayRate = rate;
    decayCoef = calcCoef(rate, targetRatioDR);
    decayBase = (sustainLevel - targetRatioDR) * (1.0f - decayCoef);
}

void ADSR::setReleaseRate(float rate) {
    releaseRate = rate;
    releaseCoef = calcCoef(rate, targetRatioDR);
    releaseBase = -targetRatioDR * (1.0f - releaseCoef);
}

float ADSR::calcCoef(float rate, float targetRatio) {
    return (rate <= 0.0f) ? 0.0f : std::exp(-std::log((1.0f + targetRatio) / targetRatio) / rate);
}

void ADSR::setSustainLevel(float level) {
    sustainLevel = level;
    decayBase = (sustainLevel - targetRatioDR) * (1.0f - decayCoef);
}

void ADSR::setTargetRatioA(float targetRatio) {
    if (targetRatio < 0.000000001f)
        targetRatio = 0.000000001f;  // -180 dB
    targetRatioA = targetRatio;
    attackCoef = calcCoef(attackRate, targetRatioA);
    attackBase = (1.0f + targetRatioA) * (1.0f - attackCoef);
}

void ADSR::setTargetRatioDR(float targetRatio) {
    if (targetRatio < 0.000000001f)
        targetRatio = 0.000000001f;  // -180 dB
    targetRatioDR = targetRatio;
    decayCoef = calcCoef(decayRate, targetRatioDR);
    releaseCoef = calcCoef(releaseRate, targetRatioDR);
    decayBase = (sustainLevel - targetRatioDR) * (1.0f - decayCoef);
    releaseBase = -targetRatioDR * (1.0f - releaseCoef);
}
