# JoePhys

JoePhys is a 2D physics simulation library designed to accurately and efficiently model particle and spring interactions.

A testbed application is included to visualize and demonstrate the library’s capabilities.

## Features

### Particles
![particles](https://raw.githubusercontent.com/Sonryle/JoePhys/refs/heads/main/testbed/example_media/particles.gif)

### Chunking
![chunks](https://raw.githubusercontent.com/Sonryle/JoePhys/refs/heads/main/testbed/example_media/chunks.gif)

## Installation

Pre-build binaries are available here: https://binaries-get-your-binaries.com

### Building on Windows
Requirements:  
* You must have CMake installed (https://cmake.org/download/)  
* You must have Git installed (https://git-scm.com/downloads)

Open powershell and navigate to where you want the JoePhys main folder to be downloaded. You can clone the JoePhys repository using Git:
```sh
git clone https://github.com/Sonryle/JoePhys
```
Then build the project:
```sh
cd JoePhys
mkdir build
cd build
cmake ..
```
In `JoePhys/build` you will be met with a Visual Studio project file (`JoePhys.sln`). Use this to compile the project.

#### If you don't want to install Visual Studio
If you prefer not to install Visual Studio, you can build JoePhys using the following steps:
```sh
git clone https://github.com/Sonryle/JoePhys
cd JoePhys
mkdir build
cd build
cmake ..
cmake --build . --config Release
```
The resulting JoePhys.exe file will be in JoePhys/build/Release/JoePhys.exe
### Building on Linux
Requirements:  
* You must have CMake installed
* You must have Git installed

In your terminal simply clone the repo, generate the make files with CMake and then build the project
```sh
git clone https://github.com/Sonryle/JoePhys
cd JoePhys
mkdir build
cd build
cmake ..
make
```
The resulting JoePhys executable will be in JoePhys/build/JoePhys

## Giving Credit Where Credit's Due

### Libraries Used

The JoePhys physics engine uses no libraries outside of the C++ Standard Library, however the testing environment uses third party libraries help with creating windows, communicating with the GPU and displaying GUI.
I am extremely grateful to all of the developers who spent great time and effort creating
wonderful open source libraries for the benefit of everyone, and I could not have created JoePhys
anywhere near as well (or even at all) if it weren't for the hard work and diligence of some of
these people.

With that said, here is the list of all third party libraries used in JoePhys:

* glad - (https://glad.dav1d.de/)
* GLFW - (https://www.glfw.org/)
* imgui - (https://github.com/ocornut/imgui)

### Resources Used

There is no way that I am smart enough to invent every formula and function for a realistic physics engine.
This project is not an invention, it is a conglomeration of things which I have learned and researched 
from OTHER people and put together into a project. No code is copy pasted or mindlessly stolen, but I have
taken a generous amount of inspiration from other projects and papers in order to create it. JoePhys is built
on existing research and prior knowledge. This project was simply created as a learning opportunity.

Again, with that being said, here is a list of all of the resources I learned from to create JoePhys:

* CPhysics by Hayden Marshalla (github.com/HaydenMarshalla/CPhysics)

  * Much inspiration was taken from this project's code structure and rendering system. No code was explicitly
    copy pasted, but a select few of the functions are word-for-word exactly the same. (These functions are in Renderer.cpp
    and they are simply used for initiating and handling the renderer)

* Im going to add the rest later
