#pragma once

#include <Syntacts/Config.hpp>
#include <Syntacts/Memory.hpp>
#include <memory>
#include <string>

//============================================================================
// LIBRARY FUNCTIONS
//============================================================================

namespace tact {

class Cue;

/// Formats used for saving and loading seralized Cues
enum class SaveFormat {
    Binary, ///< fast saving/loading and compact, but not human readable
    JSON    ///< slower saving/loading and large, but human readable
};

/// Formats used for exporting Cues
enum class ExportFormat {
    WAV,   ///< WAV audio file format
    AIFF,  ///< AIFF audio file format
    CSV    ///< comman-separated-value format
};

namespace Library {

/// Returns the directory to which all library cues are saved/loaded (usually C:\Users\[user]\AppData\Roaming\Syntacts\Library)
const std::string& getLibraryDirectory();

/// Saves a serialized cue to the global Syntacts cue library
bool saveCue(const Ptr<Cue>& cue, const std::string& name, SaveFormat format = SaveFormat::Binary);

/// Loads a serialized Cue from the global Syntacts Cue library
bool loadCue(Ptr<Cue>& cue, const std::string& name, SaveFormat format = SaveFormat::Binary);

/// Saves a Cue to an audio file or CSV
bool exportCue(const Ptr<Cue>& cue, const std::string& filePath, ExportFormat format = ExportFormat::WAV); 

} // namespace Library

} // namespace tact