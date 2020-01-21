#pragma once

#include <Tact/Signal.hpp>
#include <string>

namespace tact {

/// Formats used for exporting/importing Signals
enum class FileFormat {
    SIG,     ///< Syntacts file form
    WAV,     ///< WAV audio file format
    AIFF,    ///< AIFF audio file format
    CSV,     ///< comman-separated-value format,
    JSON,    ///< human readable serialized format,
    Auto,    ///< automatic detection from file path extension
    Unknown  ///< unknown format (for internal use only)
};

namespace Library {

/// Returns the directory to which all library Signals are saved/loaded (usually C:\Users\[user]\AppData\Roaming\Syntacts\Library)
const std::string& getLibraryDirectory();

/// Saves a serialized Signal to the global Syntacts Signal library
bool saveSignal(const Signal& signal, const std::string& name);

/// Loads a serialized Signal from the global Syntacts Signal library
bool loadSignal(Signal& signal, const std::string& name);

/// Erases a Signal from the global Syntacts Signal library if it exists
bool deleteSignal(const std::string& name);

/// Saves a Signal as a specified file format
bool exportSignal(const Signal& signal, const std::string& filePath, FileFormat format = FileFormat::Auto, int sampleRate = 48000, double maxLength = 60); 

/// Imports a Signal of a specifi file format
bool importSignal(Signal& signal, const std::string& filePath, FileFormat format = FileFormat::Auto, int sampleRate = 48000);

} // namespace Library

} // namespace tact