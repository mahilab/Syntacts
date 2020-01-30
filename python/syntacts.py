from ctypes import *

# DLL Import
_tact = cdll.LoadLibrary("./syntacts-c.dll")

###############################################################################
## SESSION
###############################################################################

class Session:
    def __init__(self):
        self._handle = _tact.Session_create()
    
    def __del__(self):
        _tact.Session_delete(self._handle)

    def open(self, index, channelCount, sampleRate):
        return _tact.Session_open3(self._handle, index, channelCount, sampleRate)

    def close(self):
        return _tact.Session_close(self._handle)
    
    def is_open(self):
        return _tact.Session_isOpen(self._handle)

    def play(self, channel, signal):
        return _tact.Session_play(self._handle, channel, signal._handle)

    def playAll(self, signal):
        return _tact.Session_playAll(self._handle, signal._handle)

    def stop(self, channel):
        return _tact.Session_stop(self._handle, channel)

    def stopAll(self, signal):
        return _tact.Session_stopAll(self._handle)

    def pause(self, channel):
        return _tact.Session_pause(self._handle, channel)

    def pauseAll(self, signal):
        return _tact.Session_pauseAll(self._handle)   

    def resume(self, channel):
        return _tact.Session_resume(self._handle, channel)

    def resumeAll(self, signal):
        return _tact.Session_resumeAll(self._handle)

    def isPlaying(self, channel):
        return _tact.Session_isPlaying(self._handle, channel)

    def isPause(self, channel):
        return _tact.Session_isPaused(self._handle, channel)

    def setVolume(self, channel, volume):
        return _tact.Session_setVolume(self._handle, channel, volume)

    def getVolume(self, channel):
        return _tact.Session_getVolume(self._handle, channel)

    def setPitch(self, channel, pitch):
        return _tact.Session_setPitch(self._handle, channel, pitch)

    def getPitch(self, channel):
        return _tact.Session_getPitch(self._handle, channel)

    def getChannelCount(self):
        return _tact.Session_getChannelCount(self._handle)

    def getSampleRate(self):
        return _tact.Session_getSampleRate(self._handle)

    def getCpuLoad(self):
        return _tact.Session_getCpuLoad(self._handle)

    @staticmethod
    def count():
        return _tact.Session_count()

###############################################################################
## SIGNAL
###############################################################################

class Signal:    
    def __init__(self, handle):
        self._handle = handle

    def __del__(self):
        _tact.Signal_delete(self._handle)

    def sample(self, t):
        return _tact.Signal_sample(self._handle, t)

    def length(self):
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
        return _tact.Signal_count()

###############################################################################
# ENVELOPE
###############################################################################

class Envelope(Signal):
    def __init__(self, duration):
        self._handle = _tact.Envelope_create(duration)

class ASR(Signal):
    def __init__(self, a, s, r):
        self._handle = _tact.ASR_create(a,s,r)

class ADSR(Signal):
    def __init__(self, a, d, s, r):
        self._handle = _tact.ADSR_create(a,d,s,r)

###############################################################################
# OSCILLATOR
###############################################################################
class Sine(Signal):
    def __init__(self, frequency):
        self._handle = _tact.Sine_create2(frequency)

class Square(Signal):
    def __init__(self, frequency):
        self._handle = _tact.Square_create2(frequency)

class Saw(Signal):
    def __init__(self, frequency):
        self._handle = _tact.Saw_create2(frequency)

class Triangle(Signal):
    def __init__(self, frequency):
        self._handle = _tact.Triangle_create2(frequency)

###############################################################################

class Product(Signal):
    def __init__(self, lhs, rhs):
        self._handle = _tact.Product_create(lhs._handle, rhs._handle)

class Sum(Signal):
    def __init__(self, lhs, rhs):
        self._handle = _tact.Sum_create(lhs._handle, rhs._handle)

###############################################################################

class Sequence(Signal):
    def __init__(self, handle = None):
        if handle:
            self._handle = handle
        else:
            self._handle =  _tact.Sequence_create()
    
    def __lshift__(self, other):
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

class Library:
    @staticmethod
    def save_signal(signal, name):
        return _tact.Library_saveSignal(signal._handle, c_char_p(name.encode()))

    @staticmethod
    def load_signal(name):
        return Signal(_tact.Library_loadSignal(c_char_p(name.encode())))

###############################################################################

class Debug:
    @staticmethod
    def sigMapSize():
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
lib_func(_tact.Session_getChannelCount, c_int, [Handle])
lib_func(_tact.Session_getSampleRate, c_double, [Handle])
lib_func(_tact.Session_getCpuLoad, c_double, [Handle])

lib_func(_tact.Session_count, c_int, None)

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

_tact.Sequence_create.restype = Handle
_tact.Sequence_create.argtypes = None

_tact.Sequence_SigSig.restype = Handle
_tact.Sequence_SigSig.argtypes = [Handle, Handle]

_tact.Sequence_SigFlt.restype = Handle
_tact.Sequence_SigFlt.argtypes = [Handle, c_double]

_tact.Sequence_FltSig.restype = Handle
_tact.Sequence_FltSig.argtypes = [c_double, Handle]

_tact.Sequence_SeqFlt.restype = None
_tact.Sequence_SeqFlt.argtypes = [Handle, c_double]

_tact.Sequence_SeqSig.restype = None
_tact.Sequence_SeqSig.argtypes = [Handle, Handle]

_tact.Sequence_SeqSeq.restype = None
_tact.Sequence_SeqSeq.argtypes = [Handle, Handle]

# General

# TODO

# PROCESS

# TODO

# Envelope

_tact.Envelope_create.restype  = Handle
_tact.Envelope_create.argtypes = [c_double]

_tact.ASR_create.restype  = Handle
_tact.ASR_create.argtypes = [c_double, c_double, c_double]

_tact.ADSR_create.restype  = Handle
_tact.ADSR_create.argtypes = [c_double, c_double, c_double, c_double]

# Oscillator

_tact.Sine_create2.restype  = Handle
_tact.Sine_create2.argtypes = [c_double]

_tact.Square_create2.restype  = Handle
_tact.Square_create2.argtypes = [c_double]

_tact.Saw_create2.restype  = Handle
_tact.Saw_create2.argtypes = [c_double]

_tact.Triangle_create2.restype  = Handle
_tact.Triangle_create2.argtypes = [c_double]

# Library

_tact.Library_saveSignal.restype  = c_bool
_tact.Library_saveSignal.argtypes = [Handle, c_char_p]

_tact.Library_loadSignal.restype  = Handle
_tact.Library_loadSignal.argtypes = [c_char_p]

# Debug

_tact.Debug_sigMapSize.restype = c_int
_tact.Debug_sigMapSize.argtypes = None

###############################################################################