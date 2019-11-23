#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Syntacts/Cue.hpp>
#include <Syntacts/Library.hpp>
#include <Syntacts/Sequence.hpp>
#include <Syntacts/Curve.hpp>
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

// Register Types (must be done in global namespace)

// CEREAL_REGISTER_TYPE(tact::Scalar);
// CEREAL_REGISTER_TYPE(tact::Ramp);
// CEREAL_REGISTER_TYPE(tact::Noise);

// CEREAL_REGISTER_TYPE(tact::IOperator);
// CEREAL_REGISTER_TYPE(tact::Sum);
// CEREAL_REGISTER_TYPE(tact::Difference);
// CEREAL_REGISTER_TYPE(tact::Product);

// CEREAL_REGISTER_TYPE(tact::Sequence);

// CEREAL_REGISTER_TYPE(tact::IOscillator);
// CEREAL_REGISTER_TYPE(tact::Sine);
// CEREAL_REGISTER_TYPE(tact::Square);
// CEREAL_REGISTER_TYPE(tact::Saw);
// CEREAL_REGISTER_TYPE(tact::Triangle);
// CEREAL_REGISTER_TYPE(tact::SineFM);
// CEREAL_REGISTER_TYPE(tact::Chirp);
// CEREAL_REGISTER_TYPE(tact::PulseTrain);

// CEREAL_REGISTER_TYPE(tact::IEnvelope)
// CEREAL_REGISTER_TYPE(tact::Envelope);
// CEREAL_REGISTER_TYPE(tact::KeyedEnvelope);
// CEREAL_REGISTER_TYPE(tact::ASR);
// CEREAL_REGISTER_TYPE(tact::ADSR);
// CEREAL_REGISTER_TYPE(tact::OscillatingEnvelope);

// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Instant>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Delayed>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Linear>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Smoothstep>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Smootherstep>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Smootheststep>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quadratic::In>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quadratic::Out>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quadratic::InOut>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Cubic::In>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Cubic::Out>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Cubic::InOut>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quartic::In>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quartic::Out>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quartic::InOut>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quintic::In>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quintic::Out>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quintic::InOut>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Sinusoidal::In>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Sinusoidal::Out>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Sinusoidal::InOut>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Exponential::In>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Exponential::Out>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Exponential::InOut>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Circular::In>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Circular::Out>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Circular::InOut>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Elastic::In>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Elastic::Out>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Elastic::InOut>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Back::In>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Back::Out>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Back::InOut>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Bounce::In>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Bounce::Out>);
// CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Bounce::InOut>);

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

Ptr<Cue> loadCue(const std::string& name, SaveFormat format) {
    Ptr<Cue> cue = nullptr;
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
    }
    catch (...) { }
    return cue;
}

bool exportCue(const Ptr<Cue>& cue, const std::string& filePath, ExportFormat format, double sampleRate) {    
    double sampleLength = 1.0 / sampleRate;
    try {
        std::vector<float> buffer(cue->sampleCount((int)sampleRate));
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
            file.setSampleRate((int)sampleRate);
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