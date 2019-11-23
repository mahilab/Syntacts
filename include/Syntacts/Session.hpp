#pragma once

#include <Syntacts/Error.hpp>
#include <Syntacts/Oscillator.hpp>
#include <Syntacts/Envelope.hpp>
#include <Syntacts/Sequence.hpp>
#include <Syntacts/Operator.hpp>
#include <string>

namespace tact {

/// Useful device information
struct Device {
    int index;                     ///< device index
    std::string name;              ///< device name
    bool isDefault;                ///< is this the default device?
    int apiIndex;                  ///< device API index
    std::string apiName;           ///< device API name
    bool isApiDefault;             ///< is this the default device for its API?
    int maxChannels;               ///< maximum number of output channels
    std::vector<int> sampleRates;  ///< supported sample rates
    int defaultSampleRate;         ///< the device's default sample rate
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

    /// Plays a signal on the specified channel of the current device
    int play(int channel, Signal signal);

    /// Plays A signal on the specified channel of the current device with a delay in seconds
    int play(int channel, Signal signal, double inSeconds);

    /// Plays a signal on all available channels of the current device
    int playAll(Signal signal);

    /// Stops playing signals on the specified channel of the current device
    int stop(int channel);

    /// Stops playing signals on all channels
    int stopAll();

    /// Pauses playing signals on the specified channel of the current device
    int pause(int channel);

    /// Pauses playing signals on all channels
    int pauseAll();

    /// Resumes playing signals on the specified channel of the current device
    int resume(int channel);

    /// Resumes playing signals on all channels
    int resumeAll();

    /// Sets the volume on the specified channel of the current device
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

    /// Returns the CPU core load (0 to 1) of the session
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