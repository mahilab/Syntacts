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

#include <Tact/Error.hpp>
#include <Tact/Oscillator.hpp>
#include <Tact/Envelope.hpp>
#include <Tact/Sequence.hpp>
#include <Tact/Operator.hpp>
#include <Tact/Process.hpp>
#include <string>

namespace tact {

/// Lists all supported audio device driver APIs.
enum class API {
    Unknown         = 0,
    DirectSound     = 1,
    MME             = 2,
    ASIO            = 3,
    SoundManager    = 4,
    CoreAudio       = 5,
    OSS             = 7,
    ALSA            = 8,
    AL              = 9,
    BeOS            = 10,
    WDMKS           = 11,
    JACK            = 12,
    WASAPI          = 13,
    AudioScienceHPI = 14
};

/// Contains information about a specific audio device.
struct Device {
    Device();
    int index;                    ///< device index
    std::string name;             ///< device name
    bool isDefault;               ///< is this the default device?
    API api;                      ///< device API index
    std::string apiName;          ///< device API name
    bool isApiDefault;            ///< is this the default device for its API?
    int maxChannels;              ///< maximum number of output channels
    std::vector<int> sampleRates; ///< supported sample rates
    int defaultSampleRate;        ///< the device's default sample rate
};

/// Encapsulates a Syntacts device Session.
class Session {
public:

    /// Constructor.
    Session();

    /// Destructor.
    ~Session();

    /// Opens the default device with its max channels and default sample rate.
    int open();

    /// Opens the default device for a specific API with its max channels and default sample rate.
    int open(API api); 

     /// Opens a specific device by name and API with its max channels and default sample rate.
    int open(const std::string& name, API api);

    /// Opens a specific device by index with its max channels and default sample rate.
    int open(int index);

    /// Opens a specific device by index with a specified number of channels and sample rate.
    int open(int index, int channelCount, double sampleRate);

    /// Opens a specific device with its max channels and default sample rate.
    int open(const Device& device);

    /// Opens a specific device with a specified number of channels and sample rate.
    int open(const Device& device, int channelCount, double sampleRate);

    /// Closes the currently opened device.
    int close();

    /// Returns true if a device is open, false otherwise.
    bool isOpen() const;

    /// Plays a signal on the specified channel of the current device.
    int play(int channel, Signal signal);

    /// Returns true if a signal is playing on the specified channel.
    bool isPlaying(int channel);

    /// Plays a signal on all available channels of the current device.
    int playAll(Signal signal);

    /// Stops playing signals on the specified channel of the current device.
    int stop(int channel);

    /// Stops playing signals on all channels.
    int stopAll();

    /// Pauses playing signals on the specified channel of the current device.
    int pause(int channel);

    /// Pauses playing signals on all channels.
    int pauseAll();

    /// Returns true if the specified channel is in a paused state.
    bool isPaused(int channel);

    /// Resumes playing signals on the specified channel of the current device.
    int resume(int channel);

    /// Resumes playing signals on all channels.
    int resumeAll();

    /// Sets the volume on the specified channel of the current device.
    int setVolume(int channel, double volume);

    /// Gets the volume on the specified channel of the current device.
    double getVolume(int channel);

    /// Sets the pitch on the specified channel of the current device.
    int setPitch(int channel, double pitch);

    /// Gets the pitch on the specified channel of the current device.
    double getPitch(int channel);

    /// Gets the max output level between 0 and 1 for the most recent buffer (useful for visualizations).
    double getLevel(int channel);

    /// Gets info for the currently opened device.
    const Device& getCurrentDevice() const;

    /// Gets info for the default device.
    const Device& getDefaultDevice() const;

    /// Gets info for all available devices, keyed by device index.
    const std::map<int, Device>& getAvailableDevices() const; 

    /// Returns the number of available channels (0 if not open).
    int getChannelCount() const;

    /// Returns the current sampling rate in Hz.
    double getSampleRate() const;

    /// Returns the CPU core load (0 to 1) of the Session.
    double getCpuLoad() const;

    /// Opens the control panel of a device if supported.
    void openControlPanel(int index);

public:

    /// Returns the number of active Sessions across the entire process.
    static int count();

private:

    class Impl;                   ///< private implementation
    std::unique_ptr<Impl> m_impl; ///< pointer to implementation    
};

} // namespace tact