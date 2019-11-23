#pragma once

/// Return errors
enum SyntactsError : int {
    SyntactsError_NoError             =  0,
    SyntactsError_InvalidDevice       = -1,
    SyntactsError_NotOpen             = -2,
    SyntactsError_AlreadyOpen         = -3,
    SyntactsError_InvalidChannel      = -4,
    SyntactsError_InvalidChannelCount = -5,
    SyntactsError_InvalidSampleRate   = -6,
    SyntactsError_NoWaveform          = -7,
    SyntactsError_ControlPanelFail    = -8
};