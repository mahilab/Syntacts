#include <Syntacts/Serialization.hpp>
#include <Syntacts/Source.hpp>
#include <Syntacts/Oscillator.hpp>
#include <Syntacts/Envelope.hpp>
#include <Syntacts/Cue.hpp>
#include <Syntacts/Tween.hpp>

#include <fstream>
#include <filesystem>
#include <iostream>

#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/functional.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/utility.hpp>

namespace fs = std::filesystem;

// Reigster Types (must be done in global namespace)

CEREAL_REGISTER_TYPE(tact::Scalar);
CEREAL_REGISTER_TYPE(tact::Ramp);

CEREAL_REGISTER_TYPE(tact::Oscillator);
CEREAL_REGISTER_TYPE(tact::SineWave);
CEREAL_REGISTER_TYPE(tact::SquareWave);
CEREAL_REGISTER_TYPE(tact::SawWave);
CEREAL_REGISTER_TYPE(tact::TriWave);
CEREAL_REGISTER_TYPE(tact::SineWaveFM);
CEREAL_REGISTER_TYPE(tact::Chirp);
CEREAL_REGISTER_TYPE(tact::PulseTrain);

CEREAL_REGISTER_TYPE(tact::Envelope);
CEREAL_REGISTER_TYPE(tact::KeyedEnvelope);
CEREAL_REGISTER_TYPE(tact::AmplitudeEnvelope);
CEREAL_REGISTER_TYPE(tact::ASR);
CEREAL_REGISTER_TYPE(tact::ADSR);
CEREAL_REGISTER_TYPE(tact::OscillatingEnvelope);

CEREAL_REGISTER_TYPE(tact::Tween::Instant);
CEREAL_REGISTER_TYPE(tact::Tween::Delayed);
CEREAL_REGISTER_TYPE(tact::Tween::Linear);
CEREAL_REGISTER_TYPE(tact::Tween::Smoothstep);
CEREAL_REGISTER_TYPE(tact::Tween::Smootherstep);
CEREAL_REGISTER_TYPE(tact::Tween::Smootheststep);
CEREAL_REGISTER_TYPE(tact::Tween::Quadratic::In);
CEREAL_REGISTER_TYPE(tact::Tween::Quadratic::Out);
CEREAL_REGISTER_TYPE(tact::Tween::Quadratic::InOut);
CEREAL_REGISTER_TYPE(tact::Tween::Cubic::In);
CEREAL_REGISTER_TYPE(tact::Tween::Cubic::Out);
CEREAL_REGISTER_TYPE(tact::Tween::Cubic::InOut);
CEREAL_REGISTER_TYPE(tact::Tween::Quartic::In);
CEREAL_REGISTER_TYPE(tact::Tween::Quartic::Out);
CEREAL_REGISTER_TYPE(tact::Tween::Quartic::InOut);
CEREAL_REGISTER_TYPE(tact::Tween::Quintic::In);
CEREAL_REGISTER_TYPE(tact::Tween::Quintic::Out);
CEREAL_REGISTER_TYPE(tact::Tween::Quintic::InOut);
CEREAL_REGISTER_TYPE(tact::Tween::Sinusoidal::In);
CEREAL_REGISTER_TYPE(tact::Tween::Sinusoidal::Out);
CEREAL_REGISTER_TYPE(tact::Tween::Sinusoidal::InOut);
CEREAL_REGISTER_TYPE(tact::Tween::Exponential::In);
CEREAL_REGISTER_TYPE(tact::Tween::Exponential::Out);
CEREAL_REGISTER_TYPE(tact::Tween::Exponential::InOut);
CEREAL_REGISTER_TYPE(tact::Tween::Circular::In);
CEREAL_REGISTER_TYPE(tact::Tween::Circular::Out);
CEREAL_REGISTER_TYPE(tact::Tween::Circular::InOut);
CEREAL_REGISTER_TYPE(tact::Tween::Elastic::In);
CEREAL_REGISTER_TYPE(tact::Tween::Elastic::Out);
CEREAL_REGISTER_TYPE(tact::Tween::Elastic::InOut);
CEREAL_REGISTER_TYPE(tact::Tween::Back::In);
CEREAL_REGISTER_TYPE(tact::Tween::Back::Out);
CEREAL_REGISTER_TYPE(tact::Tween::Back::InOut);
CEREAL_REGISTER_TYPE(tact::Tween::Bounce::In);
CEREAL_REGISTER_TYPE(tact::Tween::Bounce::Out);
CEREAL_REGISTER_TYPE(tact::Tween::Bounce::InOut);

namespace tact {

bool save(const Ptr<Cue>& cue, const std::string& name, SerialFormat format) {
    if (format == SerialFormat::Binary) {
        std::ofstream file(name, std::ios::binary);
        cereal::BinaryOutputArchive archive(file);
        archive(cue);  
    }
    else if (format == SerialFormat::JSON) {
        std::ofstream file(name);
        cereal::JSONOutputArchive archive(file);
        archive(cue); 
    }
    return true;
}

bool load(Ptr<Cue>& cue, const std::string& name, SerialFormat format) {
    if (format == SerialFormat::Binary) {
        std::ifstream file(name, std::ios::binary);
        cereal::BinaryInputArchive archive(file);
        archive(cue);
    }
    else if (format == SerialFormat::JSON) {
        std::ifstream file(name);
        cereal::JSONInputArchive archive(file);
        archive(cue);
    }
    return true;
}

}