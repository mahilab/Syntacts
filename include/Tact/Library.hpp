// MIT License
//
// Copyright (c) 2020 Mechatronics and Haptic Interfaces Lab
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Author(s): Evan Pezent (epezent@rice.edu)

#pragma once

#include <Tact/Signal.hpp>
#include <string>

namespace tact {

/// Formats used for exporting/importing Signals.
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

/// Returns the directory to which all library Signals are saved/loaded (usually C:\Users\[user]\AppData\Roaming\Syntacts\Library).
const std::string& getLibraryDirectory();

/// Saves a serialized Signal to the global Syntacts Signal library.
bool saveSignal(const Signal& signal, const std::string& name);

/// Loads a serialized Signal from the global Syntacts Signal library.
bool loadSignal(Signal& signal, const std::string& name);

/// Erases a Signal from the global Syntacts Signal library if it exists.
bool deleteSignal(const std::string& name);

/// Saves a Signal as a specified file format.
bool exportSignal(const Signal& signal, const std::string& filePath, FileFormat format = FileFormat::Auto, int sampleRate = 48000, double maxLength = 60); 

/// Imports a Signal of a specific file format.
bool importSignal(Signal& signal, const std::string& filePath, FileFormat format = FileFormat::Auto, int sampleRate = 48000);

} // namespace Library

} // namespace tact