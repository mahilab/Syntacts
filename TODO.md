# C++ API
## Must Do
- profile memory usage and consider using unique_ptr in Signal with a clone method (see Sean's video)
- realtime channel pitch adjustment
- pop signals from channel when over
- polyphony 
- try using a block allocator

## Nice to Have
- real-time manipulation of *any* parameter (see WebAudio AudioParam for inspiration)
- move sum/prod machinery into signal itself (e.g. scale factor) or template specialize 
- add option to save Cues to chosen location
- Replace Tween functors with one Bezier class, create static const instances representing existing Tweens

## Completed
- ~~remove Cue in favor of pure signals and operations~~
- ~~add duration to Signal and operators~~
- ~~Sequence class~~
- ~~determine when cloning should happen -- when passed as a param, or only once passed to the session?~~
- ~~finish implementing clone facilities (envelope, deep clone pointers, etc)~~ 

# GUI
## Must Do
- Abstracted signal nodes in Designer tab
- Sequencer tab
- drag/drop help dialogs
- fix Info dialog

## Nice to Have
- CPU usage meter
- faster startup
- copy/rename library items
- drag/drop lib to channel
- select device from Details listing
- custom theme
- last use memory (e.g. boot with last used device)
- custom plot renderer without decimation

# Plugin DLL / C# API
## Must Do
- determine why no audio output when compiled with command line .NET (vs Unity .NET)
- expose all (or most important) C++ API functionality