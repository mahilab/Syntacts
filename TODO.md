# API
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
- ~~spatializer class~~
- ~~change to bias/gain~~
- ~~import WAV files~~
- import CSV
- import Macaron JSON
- save/load spatializers
- grid mechanism in spatializers

## Optimization
- ~~consider using unique_ptr in Signal with a clone method~~
- ~~eliminate Tweens in favor of static bezier objects~~
- multi-time sample functions (all the way down)
- use of std::map for KeyedEnvelope complicates GUI, consider vectors

## Nice to Have
- ~~Repeater, Stretcher, Reverse signals~~
- real-time manipulation of *any* parameter (see WebAudio AudioParam for inspiration)
- channel polyphony 
- look into cereal's minimal load/save capabilities
- additional variables in expression

# GUI
## Must Do
- ~~Abstracted signal nodes in Designer tab~~
- ~~fix Info dialog~~
- ~~dual sliders for pitch/vol~~
- ~~render selected library item~~
- ~~fix library delete~~
- ~~don't load all lib items automatically~~
- ~~pop-up visualizer~~
- ~~dark/light themes~~
- ~~refresh library~~
- ~~Sequencer tab~~
- ~~remove carnot dep~~
- improved slot header
- drag/drop help dialogs
- PolyLine / KeyEnvelope
- improve play/pause/stop interface
- library signal references
- lirbary signal busting
- improve dnd interface

## Nice to Have
- ~~CPU usage meter~~
- ~~drag/drop lib to channel~~
- ~~select device from Details listing~~
- ~~custom plot renderer without decimation~~
- faster startup
- ~~last use memory (e.g. boot with last used device)~~
- copy/rename library items
- custom themes
- ASIO control panel

# Plugin DLL / External API
## Must Do
- determine why no ASIO audio output when compiled with command line .NET (vs Unity .NET)
- expose all (or most important) C++ API functionality

# Distribution / Documentation
- embed PortAudio / ASIO / gui
- upload compiled binairies
- create installer with NSIS
- document all classes/functions
- update examples
- 
