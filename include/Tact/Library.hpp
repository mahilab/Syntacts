#pragma once

#include <Tact/Signal.hpp>
#include <string>

namespace tact {

/// Formats used for exporting/importing Signals
enum class FileFormat {
    Unknown = -1,  ///< unknown format (for internal use only)
    Auto = 0,      ///< automatic detection from file path extension
    SIG = 1,       ///< Syntacts file form
    WAV = 2,       ///< WAV audio file format
    AIFF = 3,      ///< AIFF audio file format
    CSV = 4,       ///< comman-separated-value format,
    JSON = 5       ///< human readable serialized format
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