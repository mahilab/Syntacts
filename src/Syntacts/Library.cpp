#include <Syntacts/Serialization.hpp>
#include <Syntacts/Signal.hpp>
#include <Syntacts/Oscillator.hpp>
#include <Syntacts/Envelope.hpp>
#include <Syntacts/Cue.hpp>
#include <Syntacts/Tween.hpp>
#include <Syntacts/Library.hpp>
#include "AudioFile.hpp"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <cstdlib>

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
#include <cereal/types/atomic.hpp>

namespace fs = std::filesystem;

// Reigster Types (must be done in global namespace)

CEREAL_REGISTER_TYPE(tact::Scalar);
CEREAL_REGISTER_TYPE(tact::Ramp);

CEREAL_REGISTER_TYPE(tact::Oscillator);
CEREAL_REGISTER_TYPE(tact::Sine);
CEREAL_REGISTER_TYPE(tact::Square);
CEREAL_REGISTER_TYPE(tact::Saw);
CEREAL_REGISTER_TYPE(tact::Triangle);
CEREAL_REGISTER_TYPE(tact::SineFM);
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

namespace Library {

namespace {
    void ensureLibraryDirectoryExists() {
        fs::create_directories(getLibraryDirectory());
    }
}

const std::string& getLibraryDirectory() {
    static std::string appData   = std::string(std::getenv("APPDATA"));
    static std::string libFolder = appData + std::string("\\Syntacts\\Library\\");
    return libFolder;
}

bool saveCue(const Ptr<Cue>& cue, const std::string& name, SaveFormat format) {  
    try {  
        ensureLibraryDirectoryExists();
        if (format == SaveFormat::Binary) {
            std::ofstream file(getLibraryDirectory() + name + ".tact", std::ios::binary);
            cereal::BinaryOutputArchive archive(file);
            archive(cue);  
        }
        else if (format == SaveFormat::JSON) {
            std::ofstream file(getLibraryDirectory() + name + ".json");
            cereal::JSONOutputArchive archive(file);
            archive(cue); 
        }
        return true;
    }
    catch (...) {
        return false;
    }
}

bool loadCue(Ptr<Cue>& cue, const std::string& name, SaveFormat format) {
    try {
        ensureLibraryDirectoryExists();
        if (format == SaveFormat::Binary) {
            std::ifstream file(getLibraryDirectory() + name + ".tact", std::ios::binary);
            cereal::BinaryInputArchive archive(file);
            archive(cue);
        }
        else if (format == SaveFormat::JSON) {
            std::ifstream file(getLibraryDirectory() + name + ".json");
            cereal::JSONInputArchive archive(file);
            archive(cue);
        }
        return true;
    }
    catch (...) {
        return false;
    }
}

bool exportCue(const Ptr<Cue>& cue, const std::string& filePath, ExportFormat format) {    
    try {
        std::vector<float> buffer(cue->sampleCount(44100));
        double sampleLength = 1.0 / 44100;
        double t = 0;
        for (auto& sample : buffer) {
            sample = cue->sample(static_cast<float>(t));  
            t += sampleLength;
        }
        if (format == ExportFormat::WAV || format == ExportFormat::AIFF) {
            AudioFile<float>::AudioBuffer audioBuffer;
            audioBuffer.push_back(buffer);
            AudioFileFormat audioFormat = format == ExportFormat::WAV ? AudioFileFormat::Wave : AudioFileFormat::Aiff;
            AudioFile<float> file;
            if (!file.setAudioBuffer(audioBuffer))
                return false;
            file.setBitDepth(16);
            file.setSampleRate(44100);
            if (!file.save(filePath, audioFormat))
                return false;
        }
        else if (format == ExportFormat::CSV) {
            std::ofstream s;
            s.open(filePath);
            for (auto& sample : buffer)
                s << sample << std::endl;
            s.close();
        }
        return true;
    }
    catch (...) {
        return false;
    }
}

} // namespace Library

} // namespace tact