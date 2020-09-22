#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Tact/Library.hpp>
#include <Tact/Sequence.hpp>
#include <Tact/Curve.hpp>
#include <Tact/Oscillator.hpp>
#include <Tact/Envelope.hpp>
#include <Tact/Operator.hpp>
#include <Tact/Process.hpp>

#include <fstream>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <cctype>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/functional.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/utility.hpp>

#include <misc/AudioFile.h>

namespace fs = std::filesystem;

// Register Types (must be done in global namespace)

CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Scalar>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Time>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Ramp>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Noise>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Expression>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::PolyBezier>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Samples>);

CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Sum>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Product>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Sequence>);

CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Sine>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Square>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Saw>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Triangle>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Pwm>);

CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Envelope>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::KeyedEnvelope>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::ASR>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::ADSR>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::ExponentialDecay>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::SignalEnvelope>);

CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Repeater>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Stretcher>);
CEREAL_REGISTER_TYPE(tact::Signal::Model<tact::Reverser>);

CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Instant>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Delayed>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Linear>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Smoothstep>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Smootherstep>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Smootheststep>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quadratic::In>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quadratic::Out>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quadratic::InOut>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Cubic::In>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Cubic::Out>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Cubic::InOut>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quartic::In>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quartic::Out>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quartic::InOut>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quintic::In>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quintic::Out>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Quintic::InOut>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Sinusoidal::In>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Sinusoidal::Out>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Sinusoidal::InOut>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Exponential::In>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Exponential::Out>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Exponential::InOut>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Circular::In>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Circular::Out>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Circular::InOut>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Elastic::In>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Elastic::Out>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Elastic::InOut>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Back::In>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Back::Out>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Back::InOut>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Bounce::In>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Bounce::Out>);
CEREAL_REGISTER_TYPE(tact::Curve::Model<tact::Curves::Bounce::InOut>);

namespace tact
{

namespace Library
{

namespace
{


FileFormat getFormatFromExt(std::string ext)
{
    static std::unordered_map<std::string, FileFormat> extensions = {
        {".sig", FileFormat::SIG},
        {".wave", FileFormat::WAV},
        {".wav", FileFormat::WAV},
        {".aiff", FileFormat::AIFF},
        {".aif", FileFormat::AIFF},
        {".aifc", FileFormat::AIFF},
        {".csv", FileFormat::CSV},
        {".txt", FileFormat::CSV},
        {".json", FileFormat::JSON}};
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });
    if (extensions.count(ext))
        return extensions[ext];
    return FileFormat::Unknown;
}


std::string getExtFromFormat(FileFormat format) {
    if (format == FileFormat::SIG)
        return ".sig";
    else if (format == FileFormat::WAV)
        return ".wav";
    else if (format == FileFormat::AIFF)
        return ".aiff";
    else if (format == FileFormat::CSV)
        return ".csv";
    else if (format == FileFormat::JSON)
        return ".json";
    return "";
}

bool ensureDirectoryExists(fs::path path) {
    if (fs::exists(path) || path.empty())
        return true;
    if (!fs::create_directories(path))
    {
        std::cout << "Failed to create path: " << path << std::endl;
        return false;
    }
    return true;
}

} // namespace

const std::string &getLibraryDirectory()
{
#ifdef _WIN32
    static fs::path dirPath = std::string(std::getenv("APPDATA")) + std::string("\\Syntacts\\Library\\");
#elif __APPLE__
    static fs::path dirPath = std::string(getenv("HOME")) + "/Library/Syntacts/Library/";
#else
    static fs::path dirPath = "";
#endif
    static std::string dirStr = dirPath.generic_string();
    if (!fs::exists(dirStr) && !fs::create_directories(dirStr))
        std::cout << "Failed to create Syntacts Library folder" << std::endl; 
    return dirStr;
}

bool saveSignalEx(const Signal& signal, const std::string& dir, const std::string& name) {
    try
    {
        ensureDirectoryExists(dir);
        std::ofstream file;
        file.open(dir + name + ".sig", std::ios::binary);
        if (file)
        {
            cereal::BinaryOutputArchive archive(file);
            archive(signal);
            return true;
        }
        return false;
    }
    catch (cereal::Exception e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cout << "Unhandled Exception!" << std::endl;
        return false;
    }
}

bool loadSignalEx(Signal &signal, const std::string& dir, const std::string &name)
{
    try
    {
        std::ifstream file;
        file.open(dir + name + ".sig", std::ios::binary);
        if (file)
        {
            cereal::BinaryInputArchive archive(file);
            archive(signal);
            return true;
        }
        return false;
    }
    catch (cereal::Exception e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cout << "Unhandled Exception!" << std::endl;
        return false;
    }
}

bool saveSignal(const Signal &signal, const std::string &name)
{
    return saveSignalEx(signal, getLibraryDirectory(), name);
}

bool loadSignal(Signal &signal, const std::string &name)
{
    ensureDirectoryExists(getLibraryDirectory());
    return loadSignalEx(signal, getLibraryDirectory(), name);
}

bool deleteSignal(const std::string& name) {
    auto path = getLibraryDirectory() + name + ".sig";
    return fs::remove(path);
}

bool exportSignal(const Signal &signal, const std::string &filePath, FileFormat format, int sampleRate, double maxLength)
{
    if (format == FileFormat::Unknown) {
        std::cout << "Unknown file format" << std::endl;
        return false;
    }
    try
    {
        auto path = fs::path(filePath);
        if (format == FileFormat::Auto) {
            format = getFormatFromExt(path.extension().string());
            if (format == FileFormat::Unknown) {
                std::cout << "Unknown file format" << std::endl;
                return false;
            }
        }
        else if (!path.has_extension()) {
            path.replace_extension(getExtFromFormat(format));
        }      

        // create folder(s) if they don't exist
        if (path.has_parent_path()) {
            if (!ensureDirectoryExists(path.parent_path())) {
                return false;        
            }
        }

        if (format == FileFormat::SIG) {
            std::string dir = path.has_parent_path() ? path.parent_path().string() + "/" : "";
            std::string name = path.stem().string();
            return saveSignalEx(signal, dir, name);
        }

        // export using cereal if JSON
        if (format == FileFormat::JSON)
        {
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
        for (auto &sample : buffer)
        {
            sample = signal.sample(t);
            t += sampleLength;
        }

        if (format == FileFormat::WAV || format == FileFormat::AIFF)
        {
            AudioFile<double>::AudioBuffer audioBuffer;
            audioBuffer.emplace_back(std::move(buffer));
            AudioFileFormat audioFormat = format == FileFormat::WAV ? AudioFileFormat::Wave : AudioFileFormat::Aiff;
            AudioFile<double> file;
            if (!file.setAudioBuffer(audioBuffer)) {
                std::cout << "Failed to set file audio buffer" << std::endl;
                return false;
            }
            file.setBitDepth(16);
            file.setSampleRate(sampleRate);
            if (!file.save(path.string(), audioFormat)) {
                std::cout << "Failed to save " << path << std::endl;
                return false;
            }
            return true;
        }
        else if (format == FileFormat::CSV)
        {
            std::ofstream file;
            file.open(path);
            for (auto &sample : buffer)
                file << sample << std::endl;
            file.close();
            return true;
        }

        return false;
    }
    catch (cereal::Exception e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cout << "Unhandled Exception!" << std::endl;
        return false;
    }
}

bool importSignal(Signal &signal, const std::string &filePath, FileFormat format, int sampleRate)
{
    if (format == FileFormat::Unknown)
        return false;
    try
    {
        auto path = fs::path(filePath);
        if (format == FileFormat::Auto) {
            format = getFormatFromExt(path.extension().string());
            if (format == FileFormat::Unknown)
                return false;
        }
        else if (!path.has_extension()) {
            path.replace_extension(getExtFromFormat(format)); // should this happen?
        }      

        if (format == FileFormat::SIG) {
            std::string dir = path.has_parent_path() ? path.parent_path().string() + "/" : "";
            std::string name = path.stem().string();
            return loadSignalEx(signal, dir, name);
        }

        // export using cereal if JSON
        if (format == FileFormat::JSON)
        {
            std::ifstream file;
            file.open(path);
            cereal::JSONInputArchive archive(file);
            archive(signal);
            return true;
        }

        if (format == FileFormat::AIFF || format == FileFormat::WAV) {
            AudioFile<float> audioFile;
            if (!audioFile.load(path.string()))
            {
                std::cout << "Failed to load audio file!" << std::endl;
                return false;
            }
            signal = Samples(std::move(audioFile.samples[0]), (double)audioFile.getSampleRate());
            return true;
        }

        if (format == FileFormat::CSV) {
            std::cout << "CSV Import not yet supported" << std::endl;
            return false;
        }

        return false;

    }
    catch (cereal::Exception e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cout << "Unhandled Exception!" << std::endl;
        return false;
    }
}

} // namespace Library

} // namespace tact