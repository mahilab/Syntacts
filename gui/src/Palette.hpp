#pragma once
#include <string>
#include "Widget.hpp"

enum class PItem {
    Unknown = 0,
    Time,           
    Scalar,
    Ramp,
    Noise,
    Expression,
    Sum,
    Product,
    Repeater,
    Stretcher,
    Reverser,
    Sequencer,
    Sine,
    Square,
    Saw,
    Triangle,
    FM,
    Chirp,
    Pwm,
    Envelope,
    ASR,
    ADSR,
    ExponentialDecay,
    KeyedEnvelope,
    SignalEnvelope,
    PolyBezier
};

/// Returns the name of a Syntacts signal
const std::string& paletteName(PItem id);

class Palette : public Widget {
public:
    using Widget::Widget;
    void update() override;
};