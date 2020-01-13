#pragma once

#include <Tact/Signal.hpp>
#include <string>

namespace tact {

/// Formats used for exporting Signals
enum class FileFormat {
    WAV,   ///< WAV audio file format
    AIFF,  ///< AIFF audio file format
    CSV,   ///< comman-separated-value format,
    JSON   ///< human readable serialized format
};

namespace Library {

/// Returns the directory to which all library Signals are saved/loaded (usually C:\Users\[user]\AppData\Roaming\Syntacts\Library)
const std::string& getLibraryDirectory();

/// Saves a serialized Signal to the global Syntacts Signal library
bool saveSignal(const Signal& signal, const std::string& name);

/// Loads a serialized Signal from the global Syntacts Signal library
bool loadSignal(Signal& signal, const std::string& name);

/// Saves a Signal to an audio file or CSV
bool exportSignal(const Signal& signal, const std::string& filePath, FileFormat format, int sampleRate = 48000, double maxLength = 60); 

} // namespace Library

} // namespace tact