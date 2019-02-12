#pragma once

class ADSR {
public:
	ADSR(void);
	~ADSR(void);
	float process(void);
    float getOutput(void);
    int getState(void);
	void gate(int on);
    void setAttackRate(float rate);
    void setDecayRate(float rate);
    void setReleaseRate(float rate);
	void setSustainLevel(float level);
    void setTargetRatioA(float targetRatio);
    void setTargetRatioDR(float targetRatio);
    void reset(void);

    enum State {
        Idle = 0,
        Attack,
        Decay,
        Sustain,
        Release
    };

protected:
	int state;
	float output;
	float attackRate;
	float decayRate;
	float releaseRate;
	float attackCoef;
	float decayCoef;
	float releaseCoef;
	float sustainLevel;
    float targetRatioA;
    float targetRatioDR;
    float attackBase;
    float decayBase;
    float releaseBase;
 
    float calcCoef(float rate, float targetRatio);
};

inline float ADSR::process() {
	switch (state) {
        case Idle:
            break;
        case Attack:
            output = attackBase + output * attackCoef;
            if (output >= 1.0) {
                output = 1.0;
                state = Decay;
            }
            break;
        case Decay:
            output = decayBase + output * decayCoef;
            if (output <= sustainLevel) {
                output = sustainLevel;
                state = Sustain;
            }
            break;
        case Sustain:
            break;
        case Release:
            output = releaseBase + output * releaseCoef;
            if (output <= 0.0) {
                output = 0.0;
                state = Idle;
            }
	}
	return output;
}

inline void ADSR::gate(int gate) {
	if (gate)
		state = Attack;
	else if (state != Idle)
        state = Release;
}

inline int ADSR::getState() {
    return state;
}

inline void ADSR::reset() {
    state = Idle;
    output = 0.0;
}

inline float ADSR::getOutput() {
	return output;
}