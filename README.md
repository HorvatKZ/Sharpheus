# Sharpheus Game Engine

Easy-to-use 2D C++ game engine created as a university project

![Sharpheus](/Assets/Branding/sharpheus_promo.png?raw=true "Sharpheus")

*Under Apache License 2.0*

### Main features:
+ 2D Rendering
+ Collision system
+ Event system
+ Editor

### Supported platforms:
+ Windows
+ *Linux (in future)*

### Supported rendering APIs:
+ OpenGL
+ *More in future*

### How to use it
+ Clone this repo
+ Generate solution with premake
    + Creating a premake support for wxWidgets is complicated and not stable. So compiling wxWidgets is not part of the premake files
    + However theoritically every library is platform-independent, the solution currently tested only on Windows
    + On Windows you can easly generate VS2019 solution using the `GenVSSolution.bat`
    + Supporting other plantforms is in the plan
+ Compile wxWidgets with any of the provided tools in `external/wxWidgets`
    + For Windows using VS is recomended. You can find the prepared VS solution in `external/wxWidgets/build/msw/wx_vc16.sln`
    + Run a batch build for the projects in x64 DebugDLL and x64 Release DLL configurations
+ Copy wxWidgets x64 DLLs to `bin/Debug` or `bin/Release` (depending on the configuration)
    + In case of Windows you find these DLLs in `external/wxWidgets/lib/vc_x64_dll`
    + You only need `wxbase31ud_...dll`-s and `wxmsw31ud_...dll`-s for Debug and `wxbase314u_...dll`-s and `wxmsw314u_...dll`-s for Release configuration
+ Have fun using Sharpheus

### Using versions:
+ C++ 17
+ wxWidgets 3.1.4
+ OpenGL 4.6
+ GLFW 3.3.2
+ GLEW 2.1.0
+ glm 0.9.9.8
+ spdlog 1.8.2
+ Premake 5.0 alpha 15

### Development:
[Trello board](https://trello.com/b/oWa4OvIU/sharpheus)