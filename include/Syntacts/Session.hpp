#pragma once

#include <Syntacts/Error.hpp>
#include <Syntacts/Cue.hpp>
#include <string>

namespace tact {

/// Useful device information
struct Device {
    int index;                        ///< device index
    std::string name;                 ///< device name
    bool isDefault;                   ///< is this the default device?
    int apiIndex;                     ///< device API index
    std::string apiName;              ///< device API name
    bool isApiDefault;                ///< is this the default device for its API?
    int maxChannels;                  ///< maximum number of output channels
    std::vector<double> sampleRates;  ///< supported sample rates
};

class Session {
public:

    /// Constructor
    Session();

    /// Destructor
    ~Session();

    /// Opens the default device with its max channels and default sample rate
    int open();

    /// Opens a specific device with its max channels and default sample rate
    int open(const Device& device);

    /// Opens a specific device with a specified number of channels and sample rate
    int open(const Device& device, int channelCount, double sampleRate);

    /// Opens a specific device by index with a specified number of channels
    int open(int index);

    /// Opens a specific device by index with a specified number of channels and sample rate
    int open(int index, int channelCount, double sampleRate);

    /// Closes the currently opened device
    int close();

    /// Returns true if a device is open, false otherwise
    bool isOpen() const;

    /// Plays a Cue on the specified channel of the current device
    int play(int channel, Ptr<Cue> cue);

    /// Plays A cue on the specified channel of the current device with a delay in seconds
    int play(int channel, Ptr<Cue> cue, double inSeconds);

    /// Plays a Cue on all available channels of the current device
    int playAll(Ptr<Cue> cue);

    /// Stops playing Cues on the specified channel of the current device
    int stop(int channel);

    /// Stops playing Cues on all channels
    int stopAll();

    /// Pauses playihng Cues on the specified channel of the current device
    int pause(int channel);

    /// Pauses playing Cues on all channels
    int pauseAll();

    /// Resumes playihng Cues on the specified channel of the current device
    int resume(int channel);

    /// Resumes playing Cues on all channels
    int resumeAll();

    /// Sets teh vule on the specified channel of the current device
    int setVolume(int channel, float volume);

    /// Gets info for the currently opened device
    const Device& getCurrentDevice() const;

    /// Gets info for the default device
    const Device& getDefaultDevice() const;

    /// Gets info for all available devices, keyed by device index
    const std::map<int, Device>& getAvailableDevices() const; 

    /// Returns the number of available channels (0 if not open)
    int getChannelCount() const;

    /// Returns the current sampling rate in Hz
    double getSampleRate() const;

    /// Returns the CPU load (0 to 1) of the session
    double getCpuLoad() const;

    /// Opens the control panel of a device if supported
    void openControlPanel(int index);

public:

    /// Returns the number of active Sessions across the entire process
    static int count();

private:

    class Impl;                   ///< private implementation
    std::unique_ptr<Impl> m_impl; ///< pointer to implementation    
};

} // namespace tact