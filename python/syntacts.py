# MIT License
#
# Copyright (c) 2020 Mechatronics and Haptic Interfaces Lab
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Author(s): Evan Pezent (epezent@rice.edu)

import platform
from ctypes import *
from enum import Enum

# DLL Import
# get the right filename
if platform.uname()[0] == "Windows":
    _lib_name = "./syntacts_c.dll"
elif platform.uname()[0] == "Darwin":
    _lib_name = "./libsyntacts_c.dylib"
elif platform.uname()[0] == "Linux":
    _lib_name = "./libsyntacts_c.so"

_tact = cdll.LoadLibrary(_lib_name)

###############################################################################
## DEVICE
###############################################################################

class API(Enum):
    '''Lists all supported audio device drivers APIs.'''
    Unknown         = 0
    DirectSound     = 1
    MME             = 2
    ASIO            = 3
    SoundManager    = 4
    CoreAudio       = 5
    OSS             = 7
    ALSA            = 8
    AL              = 9
    BeOS            = 10
    WDMKS           = 11
    JACK            = 12
    WASAPI          = 13
    AudioScienceHPI = 14

class Device:
    '''
    Contains information about a specific audio device.
    Attributes:
        index               : int     -> device index
        name                : str     -> device name
        is_default          : bool    -> is this the default device?
        api                 : API     -> device API index
        api_name            : str     -> device API name
        is_api_default      : bool    -> is this the default device for its API?
        max_channels        : int     -> maximum number of output channels
        sample_rates        : [float] -> supported sample rates
        default_sample_rate : float   -> the device's default sample rate
    '''
    def __init__(self, session_handle, index):
        self.index = index
        if (index != -1):
            size = _tact.Device_nameLength(session_handle, index)
            buf = (c_char * (size + 1))()
            _tact.Device_name(session_handle, index, cast(buf, c_char_p))
            self.name = buf.value.decode()
            self.is_default = _tact.Device_isDefault(session_handle, index)
            self.api  = API(_tact.Device_api(session_handle, index))
            size = _tact.Device_apiNameLength(session_handle, index)
            buf = (c_char * (size + 1))()
            _tact.Device_apiName(session_handle, index, cast(buf, c_char_p))
            self.api_name = buf.value.decode()
            self.is_api_default = _tact.Device_isApiDefault(session_handle, index)
            self.max_channels = _tact.Device_maxChannels(session_handle, index)
            size = _tact.Device_sampleRatesCount(session_handle, index)
            buf = (c_int * size)()
            _tact.Device_sampleRates(session_handle, index, cast(buf, POINTER(c_int)))
            self.sample_rates = buf[:]
            self.default_sample_rate = _tact.Device_defaultSampleRate(session_handle, index)
        else:
            self.name = self.is_default = self.api = self.api_name = self.is_api_default = self.max_channels = self.sample_rates = self.default_sample_rate = None

###############################################################################
## SESSION
###############################################################################

class Session:
    '''Encapsulates a Syntacts device Session.'''
    def __init__(self):
        '''Constructor'''
        self._handle = _tact.Session_create()
        n = _tact.Session_getAvailableDevicesCount(self._handle)
        indices = (c_int * n)()
        _tact.Session_getAvailableDevices(self._handle, cast(indices, POINTER(c_int)))
        self.available_devices = []
        for d in indices:
            self.available_devices.append(Device(self._handle, d))
    def __del__(self):
        '''Destructor'''
        _tact.Session_delete(self._handle)

    def open(self, index=None, channelCount=0, sampleRate=0, name=None, api=None):
        '''
        Opens a device with the specified input parameters. Possible overloads:
            open()                                   # open default device
            open(22)                                 # open device by index
            open(22,8,44100)                         # open device by index with specified parameters
            open(api=API.ASIO)                       # open default device for driver API
            open(name='MOTU Pro Audio',api=API.ASIO) # open device by name under specified driver API
        '''
        if index:
            if isinstance(index, API):
                return _tact.Session_open4(self._handle, index.value)
            return _tact.Session_open3(self._handle, index, channelCount, sampleRate)
        elif (name and api):
            return _tact.Session_open5(self._handle, c_char_p(name.encode()), api.value)
        elif (api):
            return _tact.Session_open4(self._handle, api.value)
        else:
            return _tact.Session_open1(self._handle)

    def close(self):
        '''Closes the currently opened device.'''
        return _tact.Session_close(self._handle)

    def is_open(self):
        '''Returns true if a device is open.'''
        return _tact.Session_isOpen(self._handle)

    def play(self, channel, signal):
        '''Plays a signal on the specified channel of the current device.'''
        return _tact.Session_play(self._handle, channel, signal._handle)

    def play_all(self, signal):
        '''Plays a signal on all channels.'''
        return _tact.Session_playAll(self._handle, signal._handle)

    def stop(self, channel):
        '''Stops playing signals on the specified channel of the current device.'''
        return _tact.Session_stop(self._handle, channel)

    def stop_all(self):
        '''Stops playing signals on all channels.'''
        return _tact.Session_stopAll(self._handle)

    def pause(self, channel):
        '''Pauses playing signals on the specified channel of the current device.'''
        return _tact.Session_pause(self._handle, channel)

    def pause_all(self):
        '''Pauses playing signals on all channels.'''
        return _tact.Session_pauseAll(self._handle)

    def resume(self, channel):
        '''Resumes playing signals on the specified channel of the current device.'''
        return _tact.Session_resume(self._handle, channel)

    def resume_all(self):
        '''Resumes playing signals on all channels.'''
        return _tact.Session_resumeAll(self._handle)

    def is_playing(self, channel):
        '''Returns true if a signal is playing on the specified channel.'''
        return _tact.Session_isPlaying(self._handle, channel)

    def is_paused(self, channel):
        '''Returns true if the specified channel is in a paused state.'''
        return _tact.Session_isPaused(self._handle, channel)

    def set_volume(self, channel, volume):
        '''Sets the volume on the specified channel of the current device.'''
        return _tact.Session_setVolume(self._handle, channel, volume)

    def get_volume(self, channel):
        ''' Gets the volume on the specified channel of the current device.'''
        return _tact.Session_getVolume(self._handle, channel)

    def set_pitch(self, channel, pitch):
        '''Sets the pitch on the specified channel of the current device.'''
        return _tact.Session_setPitch(self._handle, channel, pitch)

    def get_pitch(self, channel):
        '''Gets the pitch on the specified channel of the current device.'''
        return _tact.Session_getPitch(self._handle, channel)

    def get_level(self, channel):
        '''Gets the max output level between 0 and 1 for the most recent buffer (useful for visualizations).'''
        return _tact.Session_getLevel(self._handle, channel)

    @property
    def current_device(self):
        '''Gets info for the currently opened device.'''
        idx = _tact.Session_getCurrentDevice(self._handle)
        return Device(self._handle, idx)

    @property
    def default_device(self):
        '''Gets info for the default device.'''
        idx = _tact.Session_getDefaultDevice(self._handle)
        return Device(self._handle, idx)

    @property
    def channel_count(self):
        '''The number of available channels (0 if not open).'''
        return _tact.Session_getChannelCount(self._handle)

    @property
    def sample_rate(self):
        '''The current sampling rate in Hz.'''
        return _tact.Session_getSampleRate(self._handle)

    @property
    def cpu_load(self):
        '''The CPU core load (0 to 1) of the Session.'''
        return _tact.Session_getCpuLoad(self._handle)

    @staticmethod
    def count():
        '''The number of currently open Sessions.'''
        return _tact.Session_count()

###############################################################################
## SPATIALIZER
###############################################################################

class Spatializer:
    '''Syntacts Spatializer interface.'''
    def __init__(self, session=None):
        '''Constructor'''
        if (session):
            self._handle = _tact.Spatializer_create(session._handle)
        else:
            self._handle = _tact.Spatializer_create(None)

    def __del__(self):
        '''Destructor'''
        _tact.Spatializer_delete(self._handle)

    def bind(self, session):
        '''Binds this Spatializer to a Session.'''
        _tact.Spatializer_bind(self._handle, session._handle)

    def unbind(self):
        '''Unbinds the Spatializer from its current Session.'''
        _tact.Spatializer_unbind(self._handle)

    def set_position(self, channel, p):
        '''Set the position of a channel. The channel will be added if it's not already in the Spatializer.'''
        _tact.Spatializer_setPosition(self._handle, channel, p[0], p[1])

    def get_position(self, channel):
        '''Gets the position of a channel if it is in the Spatializer.'''
        x = c_double()
        y = c_double()
        _tact.Spatializer_getPosition(self._handle, channel, byref(x), byref(y))
        return [x.value,y.value]

    def create_grid(self, rows, cols):
        '''Quickly create a grid of channels.'''
        return _tact.Spatializer_createGrid(self._handle, rows, cols)

    def clear(self):
        '''Remove all channels from the Spatializer.'''
        _tact.Spatializer_clear(self._handle)

    def remove(self, channel):
        '''Remove a channel from the Spatializer.'''
        _tact.Spatializer_remove(self._handle, channel)

    def has_channel(self, channel):
        '''Returns true if a channel is in the Spatializer.'''
        return _tact.Spatializer_hasChannel(self._handle, channel)

    def play(self, signal):
        '''Play a Signal on the Spatializer.'''
        _tact.Spatializer_play(self._handle, signal._handle)

    def stop(self):
        '''Stop all Signals playing in the Spatializer.'''
        _tact.Spatializer_stop(self._handle)

    def update(self):
        '''Explicitly update the pitch/volume of all channels in the Spatializer.'''
        _tact.Spatializer_update(self._handle)

    def auto_update(self, enable):
        '''Enable/disable automatic updating of pitch/volume of all channels when Spatializer target or channels change (enabled by default).'''
        _tact.Spatializer_autoUpdate(self._handle, enable)

    @property
    def target(self):
        '''The Spatializer target position.'''
        x = c_double()
        y = c_double()
        _tact.Spatializer_getTarget(self._handle, byref(x), byref(y))
        return (x.value(),y.value())

    @target.setter
    def target(self, p):
        _tact.Spatializer_setTarget(self._handle, p[0], p[1])

    @property
    def radius(self):
        '''The Spatializer target radius.'''
        return _tact.Spatializer_getRadius(self._handle)

    @radius.setter
    def radius(self, r):
        _tact.Spatializer_setRadius(self._handle, r)

    @property
    def roll_off(self):
        '''
        The Spatializer target roll-off method. Possible values are:
        "linear", "smoothstep", "smootherstep", "smootheststep", "log", or "exp"
        '''
        names = ["linear", "smoothstep", "smootherstep", "smootheststep", "log", "exp"]
        i = _tact.Spatializer_getRollOff(self._handle)
        return names[i]

    @roll_off.setter
    def roll_off(self, str):
        if str == 'linear':
            _tact.Spatializer_setRollOff(self._handle, 0)
        elif str == 'smoothstep':
            _tact.Spatializer_setRollOff(self._handle, 1)
        elif str == 'smootherstep':
            _tact.Spatializer_setRollOff(self._handle, 2)
        elif str == 'smootheststep':
            _tact.Spatializer_setRollOff(self._handle, 3)
        elif str == 'log':
            _tact.Spatializer_setRollOff(self._handle, 4)
        elif str == 'exp':
            _tact.Spatializer_setRollOff(self._handle, 5)

    @property
    def wrap(self):
        '''Spatializer wrapping interval for x and y axes. A value of 0 disables wrapping on that axis.'''
        x = c_double()
        y = c_double()
        _tact.Spatializer_getWrap(self._handle, byref(x), byref(y))
        return (x.value(),y.value())

    @wrap.setter
    def wrap(self, p):
        _tact.Spatializer_setWrap(self._handle, p[0], p[1])

    @property
    def channel_count(self):
        '''The number of channels in the Spatializer.'''
        return _tact.Spatializer_getChannelCount(self._handle)

    @property
    def volume(self):
        '''The global volume of the Spatializer.'''
        return _tact.Spatializer_getVolume(self._handle)

    @volume.setter
    def volume(self, v):
        _tact.Spatializer_setVolume(self._handle, v)

    @property
    def pitch(self):
        '''The global pitch of the Spatializer.'''
        return _tact.Spatializer_getPitch(self._handle)

    @pitch.setter
    def pitch(self, p):
        _tact.Spatializer_setPitch(self._handle, p)

    @property
    def valid(self):
        '''True if the Spatializer Handle is currently valid.'''
        return _tact.Spatializer_valid(self._handle)

###############################################################################
## SIGNAL
###############################################################################

class Signal:
    '''An object that returns time variant samples for a length of time.'''
    def __init__(self, handle):
        '''Constructor.'''
        self._handle = handle

    def __del__(self):
        '''Destructor.'''
        _tact.Signal_delete(self._handle)

    def sample(self, t):
        '''Samples the Signal at time t in seconds.'''
        return _tact.Signal_sample(self._handle, t)

    @property
    def length(self):
        '''The length of the Signal in seconds or infinity.'''
        return _tact.Signal_length(self._handle)

    def __mul__(self, other):
        if isinstance(other, Signal):
            return Product(self, other)
        elif isinstance(other, (int, float)):
            return Signal(_tact.Mul_SigFlt(self._handle, other))
        else:
            raise TypeError("other must be Signal, int, or float")

    def __rmul__(self, other):
        if isinstance(other, (int, float)):
            return Signal(_tact.Mul_FltSig(other, self._handle))
        else:
            raise TypeError("other must be int or float")

    def __add__(self, other):
        if isinstance(other, Signal):
            return Sum(self, other)
        elif isinstance(other, (int, float)):
            return Signal(_tact.Add_SigFlt(self._handle, other))
        else:
            raise TypeError("other must be Signal, int, or float")

    def __radd__(self, other):
        if isinstance(other, (int, float)):
            return Signal(_tact.Add_FltSig(other, self._handle))
        else:
            raise TypeError("other must be int or float")

    def __sub__(self, other):
        if isinstance(other, (int, float)):
            return Signal(_tact.Sub_SigFlt(self._handle, other))
        else:
            raise TypeError("other must be Signal, int, or float")

    def __rsub__(self, other):
        if isinstance(other, (int, float)):
            return Signal(_tact.Sub_FltSig(other, self._handle))
        else:
            raise TypeError("other must be int or float")

    def __lshift__(self, other):
        if isinstance(other, Signal):
            return Sequence(_tact.Sequence_SigSig(self._handle, other._handle))
        elif isinstance(other, (int, float)):
            return Sequence(_tact.Sequence_SigFlt(self._handle, other))
        else:
            raise TypeError("other must be Signal, int, or float")

    def __rlshift__(self, other):
        if isinstance(other, (int, float)):
            return Sequence(_tact.Sequence_FltSig(other, self._handle))
        else:
            raise TypeError("other must be int or float")

    @staticmethod
    def count():
        '''The current count of Signals allocated in this process.'''
        return _tact.Signal_count()

###############################################################################
# OPERATORS
###############################################################################

class Product(Signal):
    '''A Signal which is the product of two other signals.'''
    def __init__(self, lhs, rhs):
        self._handle = _tact.Product_create(lhs._handle, rhs._handle)

class Sum(Signal):
    '''A Signal which is the sum or difference of two other signals.'''
    def __init__(self, lhs, rhs):
        self._handle = _tact.Sum_create(lhs._handle, rhs._handle)

###############################################################################
# SEQUENCE
###############################################################################

class Sequence(Signal):
    '''A Signal formed by the sequencing of other Signals at various time points.'''
    def __init__(self, handle = None):
        '''Constructor'''
        if handle:
            self._handle = handle
        else:
            self._handle =  _tact.Sequence_create()

    @property
    def head(self):
        '''The current insertion head position/time.'''
        return _tact.Sequence_getHead(self._handle)

    @head.setter
    def head(self, newHead):
        _tact.Sequence_setHead(self._handle, newHead)

    def push(self, other):
        '''Pushes a sequence, signal or value at the head position and moves the head forward.'''
        if isinstance(other, Sequence):
            _tact.Sequence_SeqSeq(self._handle, other._handle)
        elif isinstance(other, Signal):
            _tact.Sequence_SeqSig(self._handle, other._handle)
        elif isinstance(other, (int, float)):
            _tact.Sequence_SeqFlt(self._handle, other)
        else:
            raise TypeError("other must be Sequence, Signal, int, or float")
        return self

    def insert(self, other, t):
        '''Inserts a sequence or signal at position t in this sequence but does NOT move head.'''
        if isinstance(other, Sequence):
            _tact.Sequence_insertSeq(self._handle, other._handle, t)
        elif isinstance(other, Signal):
            _tact.Sequence_insertSig(self._handle, other._handle, t)
        else:
            raise TypeError("other must be Sequence or Signal")
        return self

    def clear(self):
        '''Clears a sequence.'''
        _tact.Sequence_clear(self._handle)

    def __lshift__(self, other):
        '''Creates a new sequence from a sequence, signal or value and moves the head forward (a.k.a "<<").'''
        if isinstance(other, Sequence):
            _tact.Sequence_SeqSeq(self._handle, other._handle)
        elif isinstance(other, Signal):
            _tact.Sequence_SeqSig(self._handle, other._handle)
        elif isinstance(other, (int, float)):
            _tact.Sequence_SeqFlt(self._handle, other)
        else:
            raise TypeError("other must be Sequence, Signal, int, or float")
        return self

###############################################################################
# GENERAL
###############################################################################

class Time(Signal):
    '''A signal that simple returns the time passed to it.'''
    def __init__(self):
        self._handle = _tact.Time_create()

class Scalar(Signal):
    '''A Signal that emits a constant value over time.'''
    def __init__(self, value):
        self._handle = _tact.Scalar_create(value)

class Ramp(Signal):
    '''A Signal that increases or decreases over time.'''
    def __init__(self, initial, arg1, arg2=None):
        if arg2:
            self._handle = _tact.Ramp_create2(initial, arg2, arg1)
        else:
            self._handle = _tact.Ramp_create1(initial, arg1)

class Noise(Signal):
    '''A signal that generates white noise.'''
    def __init__(self):
        self._handle = _tact.Noise_create()

class Expression(Signal):
    '''A signal that returns the evaluation of an expression f(t).'''
    def __init__(self, expr):
        self._handle = _tact.Expression_create(c_char_p(expr.encode()))

# TODO: class Samples (not sure how to handle float* yet)

###############################################################################
# PROCESS
###############################################################################

class Repeater(Signal):
    '''A Signal which repeats another Signal for a number of repetitions.'''
    def __init__(self, signal, repetitions, delay):
        self._handle = _tact.Repeater_create(signal._handle, repetitions, delay)

class Stretcher(Signal):
    '''A Signal which stretches or compresses another Signal temporally by a factor.'''
    def __init__(self, signal, factor):
        self._handle = _tact.Stretcher_create(signal._handle, factor)

class Reverser(Signal):
    '''A Signal which reverses the input Signal.'''
    def __init__(self, signal):
        self._handle = _tact.Reverser_create(signal._handle)

###############################################################################
# ENVELOPE
###############################################################################

class Envelope(Signal):
    '''A basic envelope, providing a duration and constant amplitude.'''
    def __init__(self, duration, amplitude=1):
        self._handle = _tact.Envelope_create(duration, amplitude)

class ASR(Signal):
    '''Attack-Sustain-Release Envelope.'''
    def __init__(self, a, s, r, amplitude=1):
        self._handle = _tact.ASR_create(a,s,r,amplitude)

class ADSR(Signal):
    '''Attack-Decay-Sustain-Release Envelope.'''
    def __init__(self, a, d, s, r, amp1=1, amp2=0.5):
        self._handle = _tact.ADSR_create(a,d,s,r,amp1,amp2)

class ExponentialDecay(Signal):
    '''Exponential decay according to the law y = A*e^(-Bt).'''
    def __init__(self, amplitude=1, decay=6.907755):
        self._handle = _tact.ExponentialDecay_create(amplitude, decay)

class SignalEnvelope(Signal):
    '''Constructs an Envelope from a Signal.'''
    def __init__(self, signal,duration=1,amplitude=1):
        if (isinstance(signal, Signal)):
            self._handle = _tact.SignalEnvelope_create(signal._handle, duration, amplitude)
        else:
            raise TypeError("The #signal argument must be a Signal")

###############################################################################
# OSCILLATOR
###############################################################################

class Sine(Signal):
    '''A sine wave Oscillator.'''
    def __init__(self, arg1, arg2=None, arg3=None):
        '''
        Constructor. Possible overloads:
            Sine(150)               -> constant frequency
            Sine(150,100)           -> chirp (ramping frequency)
            Sine(150, Sine(10), 2) -> frequency modulated by second argument where third arg is FM index
            Sine(x)                 -> parametrized input Signal x
        '''
        if arg3:
            self._handle = _tact.Sine_create4(arg1,arg2._handle,arg3)
        elif arg2:
            self._handle = _tact.Sine_create3(arg1, arg2)
        elif isinstance(arg1, (int, float)):
            self._handle = _tact.Sine_create2(arg1)
        elif isinstance(arg1, Signal):
            self._handle = _tact.Sine_create1(arg1._handle)
        else:
            raise TypeError("Invalid arguments passed to Sine")

class Square(Signal):
    '''A square wave Oscillator.'''
    def __init__(self, arg1, arg2=None, arg3=None):
        '''
        Constructor. Possible overloads:
            Square(150)               -> constant frequency
            Square(150,100)           -> chirp (ramping frequency)
            Square(150, Sine(10), 2) -> frequency modulated by second argument where third arg is FM index
            Square(x)                 -> parametrized input Signal x
        '''
        if arg3:
            self._handle = _tact.Square_create4(arg1,arg2._handle,arg3)
        elif arg2:
            self._handle = _tact.Square_create3(arg1, arg2)
        elif isinstance(arg1, (int, float)):
            self._handle = _tact.Square_create2(arg1)
        elif isinstance(arg1, Signal):
            self._handle = _tact.Square_create1(arg1._handle)
        else:
            raise TypeError("Invalid arguments passed to Sine")

class Saw(Signal):
    '''A saw wave Oscillator.'''
    def __init__(self, arg1, arg2=None, arg3=None):
        '''
        Constructor. Possible overloads:
            Saw(150)               -> constant frequency
            Saw(150,100)           -> chirp (ramping frequency)
            Saw(150, Sine(10), 2) -> frequency modulated by second argument where third arg is FM index
            Saw(x)                 -> parametrized input Signal x
        '''
        if arg3:
            self._handle = _tact.Saw_create4(arg1,arg2._handle,arg3)
        elif arg2:
            self._handle = _tact.Saw_create3(arg1, arg2)
        elif isinstance(arg1, (int, float)):
            self._handle = _tact.Saw_create2(arg1)
        elif isinstance(arg1, Signal):
            self._handle = _tact.Saw_create1(arg1._handle)
        else:
            raise TypeError("Invalid arguments passed to Sine")

class Triangle(Signal):
    '''A triangle wave Oscillator.'''
    def __init__(self, arg1, arg2=None, arg3=None):
        '''
        Constructor. Possible overloads:
            Triangle(150)               -> constant frequency
            Triangle(150,100)           -> chirp (ramping frequency)
            Triangle(150, Sine(100), 2) -> frequency modulated by second argument where third arg is FM index
            Triangle(x)                 -> parametrized input Signal x
        '''
        if arg3:
            self._handle = _tact.Triangle_create4(arg1,arg2._handle,arg3)
        elif arg2:
            self._handle = _tact.Triangle_create3(arg1, arg2)
        elif isinstance(arg1, (int, float)):
            self._handle = _tact.Triangle_create2(arg1)
        elif isinstance(arg1, Signal):
            self._handle = _tact.Triangle_create1(arg1._handle)
        else:
            raise TypeError("Invalid arguments passed to Sine")

class Pwm(Signal):
    '''A PWM square wave with adjustable frequency and duty cycle.'''
    def __init__(self, frequency, dutyCycle=0.5):
        self._handle = _tact.Pwm_create(frequency, dutyCycle)

###############################################################################

class Library:
    '''Contains Syntacts Library functions.'''
    @staticmethod
    def save_signal(signal, name):
        '''Saves a serialized Signal to the global Syntacts Signal library.'''
        return _tact.Library_saveSignal(signal._handle, c_char_p(name.encode()))

    @staticmethod
    def load_signal(name):
        '''Loads a serialized Signal from the global Syntacts Signal library.'''
        handle = _tact.Library_loadSignal(c_char_p(name.encode()))
        if handle:
            return Signal(handle)
        return None

    @staticmethod
    def delete_signal(name):
        '''Erases a Signal from the global Syntacts Signal library if it exists.'''
        return _tact.Library_deleteSignal(c_char_p(name.encode()))

    @staticmethod
    def export_signal(signal, filePath, format=0, sampleRate=48000, maxLength=60):
        '''Saves a Signal as a specified file format.'''
        return _tact.Library_exportSignal(signal._handle, c_char_p(filePath.encode()), format, sampleRate, maxLength)

    @staticmethod
    def import_signal(filePath, format=0, sampleRate=48000):
        '''Imports a Signal of a specific file format.'''
        handle = _tact.Library_importSignal(c_char_p(filePath.encode()), format, sampleRate)
        if handle:
            return Signal(handle)
        return None

###############################################################################

class Debug:
    @staticmethod
    def sig_map_size():
        return _tact.Debug_sigMapSize()

###############################################################################
# DLL IMPORTS
###############################################################################

Handle = c_void_p

def lib_func(func, restype, argtypes):
    func.restype = restype
    func.argtypes = argtypes

# Session

lib_func(_tact.Session_create, Handle, None)
lib_func(_tact.Session_delete, None, [Handle])
lib_func(_tact.Session_open1, c_int, [Handle])
lib_func(_tact.Session_open2, c_int, [Handle, c_int])
lib_func(_tact.Session_open3, c_int, [Handle, c_int, c_int, c_double])
lib_func(_tact.Session_open4, c_int, [Handle, c_int])
lib_func(_tact.Session_open5, c_int, [Handle, c_char_p, c_int])
lib_func(_tact.Session_close, c_int, [Handle])
lib_func(_tact.Session_isOpen, c_bool, [Handle])

lib_func(_tact.Session_play, c_int, [Handle, c_int, Handle])
lib_func(_tact.Session_playAll, c_int, [Handle, Handle])
lib_func(_tact.Session_stop, c_int, [Handle, c_int])
lib_func(_tact.Session_stopAll, c_int, [Handle])
lib_func(_tact.Session_pause, c_int, [Handle, c_int])
lib_func(_tact.Session_pauseAll, c_int, [Handle])
lib_func(_tact.Session_resume, c_int, [Handle, c_int])
lib_func(_tact.Session_resumeAll, c_int, [Handle])
lib_func(_tact.Session_isPlaying, c_bool, [Handle, c_int])
lib_func(_tact.Session_isPaused, c_bool, [Handle, c_int])

lib_func(_tact.Session_setVolume, c_int, [Handle, c_int, c_double])
lib_func(_tact.Session_getVolume, c_double, [Handle, c_int])
lib_func(_tact.Session_setPitch, c_int, [Handle, c_int, c_double])
lib_func(_tact.Session_getPitch, c_double, [Handle, c_int])
lib_func(_tact.Session_getLevel, c_double, [Handle, c_int])
lib_func(_tact.Session_getChannelCount, c_int, [Handle])
lib_func(_tact.Session_getSampleRate, c_double, [Handle])
lib_func(_tact.Session_getCpuLoad, c_double, [Handle])

lib_func(_tact.Session_getCurrentDevice, c_int, [Handle])
lib_func(_tact.Session_getDefaultDevice, c_int, [Handle])
lib_func(_tact.Session_getAvailableDevicesCount, c_int, [Handle])
lib_func(_tact.Session_getAvailableDevices, None, [Handle, POINTER(c_int)])

lib_func(_tact.Session_count, c_int, None)

# Devices

lib_func(_tact.Device_nameLength, c_int, [Handle, c_int])
lib_func(_tact.Device_name, None, [Handle, c_int, c_char_p])
lib_func(_tact.Device_isDefault, c_bool, [Handle, c_int])
lib_func(_tact.Device_api, c_int, [Handle, c_int])
lib_func(_tact.Device_apiNameLength, c_int, [Handle, c_int])
lib_func(_tact.Device_apiName, None, [Handle, c_int, c_char_p])
lib_func(_tact.Device_isApiDefault, c_bool, [Handle, c_int])
lib_func(_tact.Device_maxChannels, c_int, [Handle, c_int])
lib_func(_tact.Device_sampleRatesCount, c_int, [Handle, c_int])
lib_func(_tact.Device_sampleRates, None, [Handle, c_int, POINTER(c_int)])
lib_func(_tact.Device_defaultSampleRate, c_int, [Handle, c_int])

# Spatializer

lib_func(_tact.Spatializer_create, Handle, [Handle])
lib_func(_tact.Spatializer_delete, None, [Handle])
lib_func(_tact.Spatializer_valid, c_bool, [Handle])
lib_func(_tact.Spatializer_bind, None, [Handle, Handle])
lib_func(_tact.Spatializer_unbind, None, [Handle])
lib_func(_tact.Spatializer_setPosition, None, [Handle, c_int, c_double, c_double])
lib_func(_tact.Spatializer_getPosition, None, [Handle, c_int, POINTER(c_double), POINTER(c_double)])
lib_func(_tact.Spatializer_setTarget, None, [Handle, c_double, c_double])
lib_func(_tact.Spatializer_getTarget, None, [Handle, POINTER(c_double), POINTER(c_double)])
lib_func(_tact.Spatializer_setRadius, None, [Handle, c_double])
lib_func(_tact.Spatializer_getRadius, c_double, [Handle])
lib_func(_tact.Spatializer_setRollOff, None, [Handle, c_int])
lib_func(_tact.Spatializer_getRollOff, c_int, [Handle])
lib_func(_tact.Spatializer_setWrap, None, [Handle, c_double, c_double])
lib_func(_tact.Spatializer_getWrap, None, [Handle, POINTER(c_double), POINTER(c_double)])
lib_func(_tact.Spatializer_createGrid, c_bool, [Handle, c_int, c_int])
lib_func(_tact.Spatializer_clear, None, [Handle])
lib_func(_tact.Spatializer_remove, None, [Handle, c_int])
lib_func(_tact.Spatializer_getChannelCount, c_int , [Handle])
lib_func(_tact.Spatializer_hasChannel, c_bool, [Handle, c_int])
lib_func(_tact.Spatializer_play, None, [Handle, Handle])
lib_func(_tact.Spatializer_stop, None, [Handle])
lib_func(_tact.Spatializer_setVolume, None, [Handle, c_double])
lib_func(_tact.Spatializer_getVolume, c_double, [Handle])
lib_func(_tact.Spatializer_setPitch, None, [Handle, c_double])
lib_func(_tact.Spatializer_getPitch, c_double, [Handle])
lib_func(_tact.Spatializer_autoUpdate, None, [Handle, c_bool])
lib_func(_tact.Spatializer_update, None, [Handle])

# Signal

lib_func(_tact.Signal_delete, None, [Handle])
lib_func(_tact.Signal_valid, c_bool, [Handle])
lib_func(_tact.Signal_sample, c_double, [Handle, c_double])
lib_func(_tact.Signal_length, c_double, [Handle])
lib_func(_tact.Signal_setGain, None, [Handle, c_double])
lib_func(_tact.Signal_getGain, c_double, [Handle])
lib_func(_tact.Signal_setBias, None, [Handle, c_double])
lib_func(_tact.Signal_getBias, c_double, [Handle])
lib_func(_tact.Signal_count, c_int, None)

# Operators

lib_func(_tact.Product_create, Handle, [Handle, Handle])
lib_func(_tact.Sum_create, Handle, [Handle, Handle])

lib_func(_tact.Mul_SigFlt, Handle, [Handle, c_double])
lib_func(_tact.Mul_FltSig, Handle, [c_double, Handle])

lib_func(_tact.Add_SigFlt, Handle, [Handle, c_double])
lib_func(_tact.Add_FltSig, Handle, [c_double, Handle])

lib_func(_tact.Sub_SigFlt, Handle, [Handle, c_double])
lib_func(_tact.Sub_FltSig, Handle, [c_double, Handle])

lib_func(_tact.Neg_Sig, Handle, [Handle])

# Sequence

lib_func(_tact.Sequence_create, Handle, None)
lib_func(_tact.Sequence_getHead, c_double, [Handle])
lib_func(_tact.Sequence_setHead, None, [Handle, c_double])
lib_func(_tact.Sequence_pushFlt, None, [Handle, c_double])
lib_func(_tact.Sequence_pushSig, None, [Handle, Handle])
lib_func(_tact.Sequence_pushSeq, None, [Handle, Handle])
lib_func(_tact.Sequence_insertSig, None, [Handle, Handle, c_double])
lib_func(_tact.Sequence_insertSeq, None, [Handle, Handle, c_double])
lib_func(_tact.Sequence_clear, None, [Handle])

lib_func(_tact.Sequence_SigSig, Handle, [Handle, Handle])
lib_func(_tact.Sequence_SigFlt, Handle, [Handle, c_double])
lib_func(_tact.Sequence_FltSig, Handle, [c_double, Handle])
lib_func(_tact.Sequence_SeqFlt, None, [Handle, c_double])
lib_func(_tact.Sequence_SeqSig, None, [Handle, Handle])
lib_func(_tact.Sequence_SeqSeq, None, [Handle, Handle])

# General

lib_func(_tact.Time_create, Handle, None)
lib_func(_tact.Scalar_create, Handle, [c_double])
lib_func(_tact.Ramp_create1, Handle, [c_double, c_double])
lib_func(_tact.Ramp_create2, Handle, [c_double, c_double, c_double])
lib_func(_tact.Noise_create, Handle, None)
lib_func(_tact.Expression_create, Handle, [c_char_p])
lib_func(_tact.Samples_create, Handle, [POINTER(c_float), c_int, c_double])

# Process

lib_func(_tact.Repeater_create, Handle, [Handle, c_int, c_double])
lib_func(_tact.Stretcher_create, Handle, [Handle, c_double])
lib_func(_tact.Reverser_create, Handle, [Handle])

# Envelope

lib_func(_tact.Envelope_create, Handle, [c_double, c_double])
lib_func(_tact.ASR_create, Handle, [c_double, c_double, c_double, c_double])
lib_func(_tact.ADSR_create, Handle, [c_double, c_double, c_double, c_double, c_double, c_double])
lib_func(_tact.ExponentialDecay_create, Handle, [c_double, c_double])
lib_func(_tact.SignalEnvelope_create, Handle, [Handle, c_double, c_double])

# Oscillator

lib_func(_tact.Sine_create1, Handle, [Handle])
lib_func(_tact.Sine_create2, Handle, [c_double])
lib_func(_tact.Sine_create3, Handle, [c_double, c_double])
lib_func(_tact.Sine_create4, Handle, [c_double, Handle, c_double])

lib_func(_tact.Square_create1, Handle, [Handle])
lib_func(_tact.Square_create2, Handle, [c_double])
lib_func(_tact.Square_create3, Handle, [c_double, c_double])
lib_func(_tact.Square_create4, Handle, [c_double, Handle, c_double])

lib_func(_tact.Saw_create1, Handle, [Handle])
lib_func(_tact.Saw_create2, Handle, [c_double])
lib_func(_tact.Saw_create3, Handle, [c_double, c_double])
lib_func(_tact.Saw_create4, Handle, [c_double, Handle, c_double])

lib_func(_tact.Triangle_create1, Handle, [Handle])
lib_func(_tact.Triangle_create2, Handle, [c_double])
lib_func(_tact.Triangle_create3, Handle, [c_double, c_double])
lib_func(_tact.Triangle_create4, Handle, [c_double, Handle, c_double])

lib_func(_tact.Pwm_create, Handle, [c_double, c_double])

# Library

lib_func(_tact.Library_saveSignal, c_bool, [Handle, c_char_p])
lib_func(_tact.Library_loadSignal, Handle, [c_char_p])
lib_func(_tact.Library_deleteSignal, c_bool, [c_char_p])
lib_func(_tact.Library_exportSignal, c_bool, [Handle, c_char_p, c_int, c_int, c_double])
lib_func(_tact.Library_importSignal, Handle, [c_char_p, c_int, c_int])

# Debug

lib_func(_tact.Debug_sigMapSize, int, None)

###############################################################################
