#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Tact/Library.hpp>
#include <Tact/Sequence.hpp>
#include <Tact/Curve.hpp>
#include <Tact/Oscillator.hpp>
#include <Tact/Envelope.hpp>
#include <Tact/Operator.hpp>

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

#include <misc/AudioFile.h>

namespace fs = std::filesystem;

// Register Types (must be done in global namespace)

// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::ZeroSignal>);
// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Scalar>);
// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Ramp>);
// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Noise>);

// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Sum>);
// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Product>);

// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Sequence>);

// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Sine>);
// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Square>);
// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Saw>);
// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Triangle>);
// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::SineFM>);
// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Chirp>);
// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::PulseTrain>);

// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Envelope>);
// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::KeyedEnvelope>);
// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::ASR>);
// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::ADSR>);
// CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::SignalEnvelope>);

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
    bool ensureLibraryDirectoryExists() {
        return fs::create_directories(getLibraryDirectory());
    }
}

const std::string& getLibraryDirectory() {
    static std::string appData   = std::string(std::getenv("APPDATA"));
    static std::string libFolder = appData + std::string("\\Syntacts\\Library\\");
    return libFolder;
}

bool saveSignal(const Signal& signal, const std::string& name) {  
    try {  
        ensureLibraryDirectoryExists();
        std::ofstream file;
        file.open(getLibraryDirectory() + name + ".tact", std::ios::binary);
        if (file) {
            cereal::BinaryOutputArchive archive(file);
            archive(signal);  
            return true;
        }
        return false;
    }
    catch (cereal::Exception e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    catch (...) {
        return false;
    }
}

bool loadSignal(Signal& signal, const std::string& name) {
    try {
        ensureLibraryDirectoryExists();
        std::ifstream file;
        file.open(getLibraryDirectory() + name + ".tact", std::ios::binary);
        if (file) {
            cereal::BinaryInputArchive archive(file);
            archive(signal);
            return true;
        }
        return false;
    }
    catch (cereal::Exception e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    catch (...) {
        return false;
     }
}

bool exportSignal(const Signal& signal, const std::string& filePath, FileFormat format, int sampleRate, float maxLength) {
    try {

        auto path = fs::path(filePath);
        if (format == FileFormat::WAV) {
            path.replace_extension(".wav");
        }
        else if (format == FileFormat::AIFF)
            path.replace_extension(".aiff");
        else if (format == FileFormat::CSV)
            path.replace_extension(".csv");
        else if (format == FileFormat::JSON)
            path.replace_extension(".json");


        // export using cereal if JSON
        if (format == FileFormat::JSON) {
            std::ofstream file;
            file.open(path);
            cereal::JSONOutputArchive archive(file);
            archive(signal); 
            return true;
        }

        double sampleLength = 1.0 / sampleRate;
        auto length = signal.length() > maxLength ? maxLength : signal.length();
        std::vector<double> buffer(static_cast<std::size_t>(length * sampleRate));
        double t = 0;
        for (auto& sample : buffer) {
            sample = signal.sample(static_cast<float>(t));  
            t += sampleLength;
        }

        if (format == FileFormat::WAV || format == FileFormat::AIFF) {
            AudioFile<double>::AudioBuffer audioBuffer;
            audioBuffer.emplace_back(std::move(buffer));
            AudioFileFormat audioFormat = format == FileFormat::WAV ? AudioFileFormat::Wave : AudioFileFormat::Aiff;
            AudioFile<double> file;
            if (!file.setAudioBuffer(audioBuffer))
                return false;
            file.setBitDepth(16);
            file.setSampleRate(sampleRate);
            if (!file.save(path.generic_string(), audioFormat))
                return false;
        }
        else if (format == FileFormat::CSV) {
            std::ofstream file;
            file.open(path);
            for (auto& sample : buffer)
                file << sample << std::endl;
            file.close();
        }
        return true;
    }
    catch(cereal::Exception e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    catch (...) {
        return false;
    }
}

} // namespace Library

} // namespace tact