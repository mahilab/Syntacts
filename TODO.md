# C++ API
## Must Do
- ~~remove Cue in favor of pure signals and operations~~
- ~~add duration to Signal and operators~~
- ~~Sequence class~~
- ~~determine when cloning should happen -- when passed as a param, or only once passed to the session?~~
- ~~finish implementing clone facilities (envelope, deep clone pointers, etc)~~ 
- ~~Signal pool allocator~~ 
- ~~add option to save Cues to chosen location~~
- ~~move sum/prod machinery into signal itself (e.g. offset and scale factor) or template do template specializations~~ 
- ~~pop signals from channel when over~~
- ~~change oscillator ctors to do 2*pi*f scaling in Scalar~~
- ~~realtime channel pitch adjustment~~
- import audio/csv files
- spatializer class
- change to bias/gain


## Optimization
- ~~consider using unique_ptr in Signal with a clone method~~
- multi-sample functions (all the way down)
- eliminate Tweens in favor of static bezier objects
- use of std::map for KeyedEnvelope complicates GUI, consider vectors

## Nice to Have
- real-time manipulation of *any* parameter (see WebAudio AudioParam for inspiration)
- channel polyphony 
- look into cereal's minima load/save capabilities
- additional variables in expression
- Repeater, Stretcher signal

# GUI
## Must Do
- ~~Abstracted signal nodes in Designer tab~~
- Sequencer tab
- drag/drop help dialogs
- ~~fix Info dialog~~
- ~~dual sliders for pitch/vol~~
- improved slot header
- ~~render selected library item~~
- ~~fix library delete~~
- don't load all lib items automatically
- pop-up visualizer
- refresh library

## Nice to Have
- ~~CPU usage meter~~
- faster startup
- copy/rename library items
- drag/drop lib to channel
- ~~select device from Details listing~~
- custom theme
- last use memory (e.g. boot with last used device)
- ~~custom plot renderer without decimation~~

# Plugin DLL / C# API
## Must Do
- determine why no ASIO audio output when compiled with command line .NET (vs Unity .NET)
- expose all (or most important) C++ API functionality