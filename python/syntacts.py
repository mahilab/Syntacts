from ctypes import *

###############################################################################

_syntacts = cdll.LoadLibrary("./syntacts-c.dll")

###############################################################################

_syntacts.Session_create.restype  = c_void_p
_syntacts.Session_create.argtypes = None

_syntacts.Session_delete.restype  = None
_syntacts.Session_delete.argtypes = [c_void_p]

_syntacts.Session_open.restype  = c_int
_syntacts.Session_open.argtypes = [c_void_p, c_int, c_int, c_double]

_syntacts.Session_close.restype  = c_int
_syntacts.Session_close.argtypes = [c_void_p]

_syntacts.Session_play.restype  = c_int
_syntacts.Session_play.argtypes = [c_void_p, c_int, c_void_p]

_syntacts.Session_stop.restype  = c_int
_syntacts.Session_stop.argtypes = [c_void_p, c_int]

_syntacts.Session_pause.restype  = c_int
_syntacts.Session_pause.argtypes = [c_void_p, c_int]

_syntacts.Session_resume.restype  = c_int
_syntacts.Session_resume.argtypes = [c_void_p, c_int]

_syntacts.Session_setVolume.restype  = c_int
_syntacts.Session_setVolume.argtypes = [c_void_p, c_int, c_float]

_syntacts.Session_setPitch.restype  = c_int
_syntacts.Session_setPitch.argtypes = [c_void_p, c_int, c_float]

_syntacts.Session_isOpen.restype  = c_bool
_syntacts.Session_isOpen.argtypes = [c_void_p]

_syntacts.Session_count.restype = c_int
_syntacts.Session_count.argtypes = None

###############################################################################

_syntacts.Signal_valid.restype  = c_bool
_syntacts.Signal_valid.argtypes = [c_void_p]

_syntacts.Signal_delete.restype  = None
_syntacts.Signal_delete.argtypes = [c_void_p]

_syntacts.Signal_sample.restype  = c_float
_syntacts.Signal_sample.argtypes = [c_void_p, c_float]

_syntacts.Signal_length.restype  = c_float
_syntacts.Signal_length.argtypes = [c_void_p]

_syntacts.Signal_count.restype = c_int
_syntacts.Signal_count.argtypes = None

###############################################################################

_syntacts.Envelope_create.restype  = c_void_p
_syntacts.Envelope_create.argtypes = [c_float]

_syntacts.ASR_create.restype  = c_void_p
_syntacts.ASR_create.argtypes = [c_float, c_float, c_float]

_syntacts.ADSR_create.restype  = c_void_p
_syntacts.ADSR_create.argtypes = [c_float, c_float, c_float, c_float]

###############################################################################

_syntacts.Sine_create.restype  = c_void_p
_syntacts.Sine_create.argtypes = [c_float]

_syntacts.Square_create.restype  = c_void_p
_syntacts.Square_create.argtypes = [c_float]

_syntacts.Saw_create.restype  = c_void_p
_syntacts.Saw_create.argtypes = [c_float]

_syntacts.Triangle_create.restype  = c_void_p
_syntacts.Triangle_create.argtypes = [c_float]

###############################################################################

_syntacts.Product_create.restype = c_void_p
_syntacts.Product_create.argtypes = [c_void_p, c_void_p]

_syntacts.Sum_create.restype = c_void_p
_syntacts.Sum_create.argtypes = [c_void_p, c_void_p]

###############################################################################

_syntacts.Library_saveSignal.restype  = c_bool
_syntacts.Library_saveSignal.argtypes = [c_void_p, c_char_p]

_syntacts.Library_loadSignal.restype  = c_void_p
_syntacts.Library_loadSignal.argtypes = [c_char_p]

class Session:
    def __init__(self):
        self._handle = _syntacts.Session_create()
    
    def __del__(self):
        _syntacts.Session_delete(self._handle)

    def open(self, index, channelCount, sampleRate):
        return _syntacts.Session_open(self._handle, index, channelCount, sampleRate)

    def close(self):
        return _syntacts.Session_close(self._handle)

    def play(self, channel, signal):
        return _syntacts.Session_play(self._handle, channel, signal._handle)

    def stop(self, channel):
        return _syntacts.Session_stop(self._handle, channel)

    def pause(self, channel):
        return _syntacts.Session_pause(self._handle, channel)

    def resume(self, channel):
        return _syntacts.Session_resume(self._handle, channel)

    def set_volume(self, channel, volume):
        return _syntacts.Session_setVolume(self._handle, channel, volume)

    def set_pitch(self, channel, pitch):
        return _syntacts.Session_setPitch(self._handle, channel, pitch)

    def is_open(self):
        return _syntacts.Session_isOpen(self._handle)

###############################################################################

class Signal:    
    def __init__(self):
        self._handle = 0

    def __del__(self):
        _syntacts.Signal_delete(self._handle)

    def sample(self, t):
        return _syntacts.Signal_sample(self._handle, t)

    def length(self):
        return _syntacts.Signal_length(self._handle)

    def __mul__(self, other):
        return Product(self, other)

    def __add__(self, other):
        return Sum(self, other)

    @staticmethod
    def count():
        return _syntacts.Signal_count()

###############################################################################

class Envelope(Signal):
    def __init__(self, duration):
        self._handle = _syntacts.Envelope_create(duration)

class ASR(Signal):
    def __init__(self, a, s, r):
        self._handle = _syntacts.ASR_create(a,s,r)

class ADSR(Signal):
    def __init__(self, a, d, s, r):
        self._handle = _syntacts.ADSR_create(a,d,s,r)

###############################################################################

class Sine(Signal):
    def __init__(self, frequency):
        self._handle = _syntacts.Sine_create(frequency)

class Square(Signal):
    def __init__(self, frequency):
        self._handle = _syntacts.Square_create(frequency)

class Saw(Signal):
    def __init__(self, frequency):
        self._handle = _syntacts.Saw_create(frequency)

class Triangle(Signal):
    def __init__(self, frequency):
        self._handle = _syntacts.Triangle_create(frequency)

###############################################################################

class Product(Signal):
    def __init__(self, lhs, rhs):
        self._handle = _syntacts.Product_create(lhs._handle, rhs._handle)

class Sum(Signal):
    def __init__(self, lhs, rhs):
        self._handle = _syntacts.Sum_create(lhs._handle, rhs._handle)

###############################################################################

class Library:
    @staticmethod
    def save_signal(signal, name):
        return _syntacts.Library_saveSignal(signal._handle, c_char_p(name.encode()))

    @staticmethod
    def load_signal(name):
        sig = Signal()
        sig._handle = _syntacts.Library_loadSignal(c_char_p(name.encode()))
        return sig

###############################################################################
