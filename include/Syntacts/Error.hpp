#pragma once

/// Return errors
enum SyntactsError : int {
    SyntactsError_NoError             =  0,
    SyntactsError_InvalidDevice       = -1,
    SyntactsError_NotInitialized      = -2,
    SyntactsError_AlreadyIntialized   = -3,
    SyntactsError_InvalidChannel      = -4,
    SyntactsError_InvalidChannelCount = -5,
    SyntactsError_NoWaveform          = -6,
    SyntactsError_AudioFileBufferFail = -7,
    SyntactsError_AudioFileSaveFail   = -8,
    SyntactsError_ControlPanelFail    = -9
};