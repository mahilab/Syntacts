<img src="https://raw.githubusercontent.com/mahilab/Syntacts/master/logo/logo_text.png" width="500">

Syntacts is a haptic rendering framework for vibrotactile feedback. It eliminates the need for expensive haptic controllers or custom electronics by leveraging commercial-off-the-shelf audio interfaces. As a complete package, Syntacts provides the software and hardware needed to interface audio devices with low latency, synthesize complex waveforms, and amplify signals to appropriate levels. To learn more, please visit the official website:

## [www.syntacts.org](https://www.syntacts.org/)

# Quick Start

You can find in-depth [tutorials](https://www.syntacts.org/tutorials/) on the main website, but if you're in a hurry, follow these quick start guides:

## Syntacts GUI
- Get the latest [Release](https://github.com/mahilab/Syntacts/releases) and extract the files.
- Run the GUI executable `syntacts_gui` in the top level directory. 
- On Windows, you may receive a "Windows protected your PC" screen. Click "More info", then "Run anyway".
- On macOS, the executable `syntacts_gui` may not run. Open a Terminal and run the following command in the directory of `syntacts_gui`. After this, you should be able to successfully run `syntacts_gui`.

  ```shell
  > sudo chmod 777 syntacts_gui
  ```

## C/C++ API
- Get the source code by pulling the `master` branch on the GitHub repository.
- Build and install **Syntacts** for your system by following the [tutorial](https://www.syntacts.org/tutorials/).
- Use the [template](https://github.com/mahilab/Syntacts/tree/master/template) to make a new Syntacts project with CMake.

## C# API
- Get the latest [Release](https://github.com/mahilab/Syntacts/releases) and extract the files.
- Navigate to the `csharp` directory.
- From the command line, build the Syntacts library and then run each example you want to try out:
  ```shell
  > cd csharp/Syntacts
  > dotnet build
  > cd ../examples/example_basic
  > dotnet run
  ```
    
## Unity 
- Get the latest [Release](https://github.com/mahilab/Syntacts/releases) and extract the files.
- Import `unity/syntacts.unitypackage` to your project.
- Add the `SyntactsHub` component to a scene object.
- Use the `session` member variable of `SyntactsHub` to play Syntacts Signals.

OR

- Navigate to the `unity/SyntactsDemo/Assets/Demo` directory.
- Open `Demo.unity`.

## Python API
- Get the latest [Release](https://github.com/mahilab/Syntacts/releases) and extract the files.
- Navigate to the `python` directory.
- Run any of the `example.py` files:
  ```shell
  > python example_basic.py
  ```

# Having issues?
- Check the [Tutorials](https://www.syntacts.org/tutorials/) and [FAQ](https://www.syntacts.org/faq/) pages.
- Head over to the [Issues](https://github.com/mahilab/Syntacts/issues) page and tell us about your problem.
