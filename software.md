---
layout: page
title: Software
permalink: /software/
---

![Syntacts Software](https://raw.githubusercontent.com/wiki/mahilab/Syntacts/images/software_no_text.png)

Syntacts provides you with low level access to audio hardware and a variety of mechanisms to generate tactile cues. Softwre APIs and examples for C/C++, C#, Python, and Unity are provided. In addition, Syntacts contains a cross-platform GUI for prototyping and playing back cues. Follow the steps below to get started in your preferred environment.

## GUI
- Get the latest [Release](https://github.com/mahilab/Syntacts/releases) and extract the files.
- Run the GUI executable `syntacts-gui` in the top level directory. On Windows, you may receive a "Windows protected your PC" screen. Click "More info", then "Run anyway".

## C/C++ API
- Get the source code by pulling the `master` branch on the GitHub repository or from the latest [Release](https://github.com/mahilab/Syntacts/releases). 
- Build and install **Syntacts** for your system my following the [tutorial](build.md).
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
    
## Python API
- Get the latest [Release](https://github.com/mahilab/Syntacts/releases) and extract the files.
- Navigate to the `python` directory.
- Run any of the `example.py` files.

## Unity Example
- Get the latest [Release](https://github.com/mahilab/Syntacts/releases) and extract the files.
- Import `unity/syntacts.unitypackage` to your project.
- Add the `SyntactsHub` script to a scene object.

OR

- Navigate to the `unity/SyntactsDemo/Assets` directory.
- Open `Demo.unity` in Unity.

## Having issues?
- Check the [Tutorials](tutorials.md) and [FAQ](faq.md) pages.
- Head over to the [Issues](https://github.com/mahilab/Syntacts/issues) page and tell us your problem.

