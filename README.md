# Sharpheus Game Engine

![Sharpheus](/Assets/Branding/sharpheus_long_logo.png?raw=true "Sharpheus")

**Sharpheus** is an easy-to-use 2D only C++ game engine, with an editor supporting it. The core principles behind are **simplycity and efficiency**. The project started as a university thesis in the beginning of 2021, and has continued since. The entire codebase is free to use till the origin is indicated.

> Under MIT License

## Features

- An intuitive Editor

![Editor](/Assets/Branding/editor.png?raw=true "Editor")

- Custom code can be written in both Python and C++

![Behaviors](/Assets/Branding/behaviors.png?raw=true "Behaviors")

- 2D batch rendering using the GPU

![Batch Rendering](/Assets/Branding/batch.png?raw=true "Batch Rendering")

- Exporting the game without external dependencies

![Exported](/Assets/Branding/exported.png?raw=true "Exported")

- Multimedia support

![Multimedia](/Assets/Branding/sounds.png?raw=true "Multimedia")

- Custom physics system

![Physics](/Assets/Branding/physics.png?raw=true "Physics")

- Global and local event handling system

![Events](/Assets/Branding/events.png?raw=true "Events")

- Logging support

![Logging](/Assets/Branding/logging.png?raw=true "Logging")

- Animations, tilemaps, particle systems and more...

![Extras](/Assets/Branding/extras.png?raw=true "Extras")

## Supported

### Operating System

- **Windows**
- *Linux (in future)*

### Graphics APIs

- **OpenGL**
- *Vulkan in future*

### Architectures

- **64-bit (x64) only**
- **Sharpheus will NOT support 32-bit (x86) architecture**

### File formats

- **Own formats:**

  - proj.sharpheus - project file
  - lvl.sharpheus - level file
  - scene.sharpheus - scene file
  - anim.sharpheus - animation file
  - tile.sharpheus - tileset file

- **Images:**

  - png (recommended)
  - jpg & jpeg
  - bmp
  - gif (only first frame is displayed)

- **Audio:**

  - mp3 (recommended)
  - wav
  - flac
  - ogg

### How to use it
+ Clone this repo
+ Compile wxWidgets with any of the provided tools in `external/wxWidgets`
    + Creating a premake support for wxWidgets is complicated and not stable. So compiling wxWidgets is not part of the premake files
    + For Windows using VS is recomended. You can find the prepared VS solution in `external/wxWidgets/build/msw/wx_vc16.sln`
    + Run a batch build for the projects in x64 DebugDLL and x64 Release DLL configurations
+ Copy wxWidgets x64 DLLs to `bin/Debug`, `bin/Release`, `Template/bin/Debug` and `Template/bin/Release`
    + In case of Windows you find these DLLs in `external/wxWidgets/lib/vc_x64_dll`
    + You only need `wxbase31ud_...dll`-s and `wxmsw31ud_...dll`-s for Debug and `wxbase314u_...dll`-s and `wxmsw314u_...dll`-s for Release configuration
+ Compile SharpheusStarter
    + However theoritically every library is platform-independent, the solution currently tested only on Windows
    + Supporting other plantforms is in the plan
    + On Windows you can easly generate VS2019 solution for SharpheusStarter using the `SharpheusStarter/GenVSSolution.bat`
    + Now you can build SharpheusStarter using this solution
+ Start SharpheusStarter and have fun using Sharpheus

### Using versions:
+ C++ 17
+ wxWidgets 3.1.4
+ OpenGL 4.6
+ GLFW 3.3.2
+ GLEW 2.1.0
+ glm 0.9.9.8
+ spdlog 1.8.2
+ SoLoud 2020.02.07
+ pybind11 2.9.0
+ Premake 5.0 alpha 15
