#include <Syntacts/Serialization.hpp>
#include <Syntacts/Source.hpp>
#include <Syntacts/Oscillator.hpp>
#include <Syntacts/Envelope.hpp>
#include <Syntacts/Cue.hpp>

#include <fstream>
#include <filesystem>
#include <iostream>

#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
// #include <cereal/archives/json.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/functional.hpp>

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

namespace tact {

bool save(const std::shared_ptr<Cue>& cue, const std::string& name) {
    std::ofstream file(name);
    cereal::BinaryOutputArchive archive(file);
    archive(cue);  
    return true;
}

bool load(std::shared_ptr<Cue>& cue, const std::string& name) {
    std::ifstream file(name);
    cereal::BinaryInputArchive archive(file);
    archive(cue);
    return true;
}

}