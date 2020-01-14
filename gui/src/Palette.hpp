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
    Filter,
    Repeater,
    Stretcher,
    Reverser,
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
    KeyedEnvelope,
    SignalEnvelope,
    PolyLine,
    PolyBezier
};

/// Returns the name of a Syntacts signal
const std::string& palletteString(PItem id);

class Palette : public Widget {
public:
    using Widget::Widget;
    void render() override;
};