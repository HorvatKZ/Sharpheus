# Sharpheus Game Engine

![Sharpheus](/Assets/Branding/sharpheus_long_logo.png?raw=true "Sharpheus")

**Sharpheus** is an easy-to-use 2D only C++ game engine, with an editor supporting it. The core principles behind are **simplycity and efficiency**. The project started as a university thesis in the begining of 2021, and has continued since. The entire codebase is free to use till the origin is indicated.

*Under Apache License 2.0*

### Main features of the Engine:
+ 2D Batch Rendering using the GPU
+ Mutlimedia support
+ Collision system
+ Event system for handing inputs (window, keyboard, mouse, controller)
+ Efficient reource handling
+ Logging

### Main features of the Editor:
+ Useing the engine's capabilities with an intuitive UI
+ Exporting the finished game for distribution
+ Creating and editing levels
+ Creating, organising and editing GameObjects
+ Creating and using frame based animations
+ Creating and using tilesets

### Supported platforms:
+ **Windows**
+ *Linux (in future)*

### Supported rendering APIs:
+ **OpenGL**
+ *More in future*

### Supported architectures:
+ **64-bit (x64) only**
+ **Sharpheus will NOT support 32-bit (x86) architecture**

### Supported file formats:
+ **Own formats:**
    + proj.sharpheus - project file
    + lvl.sharpheus - level file
    + scene.sharpheus - scene file
    + anim.sharpheus - animation file
    + tile.sharpheus - tileset file
+ **Images:**
    + png (recommended)
    + jpg & jpeg
    + bmp
    + gif (only first frame is displayed)
+ **Audio:**
    + mp3 (recommended)
    + wav
    + flac
    + ogg

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
+ Premake 5.0 alpha 15

### Development:
[Trello board](https://trello.com/b/oWa4OvIU/sharpheus)