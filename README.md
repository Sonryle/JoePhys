# JoePhys

JoePhys is a 2D physics simulation library written in C++ which is designed to accurately and efficiently model particle and spring interactions.

A testbed application is included to visualize and demonstrate the library’s capabilities.
![testbed](https://github.com/Sonryle/JoePhys/blob/main/testbed/example_media/JoePhys.gif)

## Features

### Particles
Supports unlimited particles of any radius, mass, and elasticity. Particles may also exist as 'static', meaning they are immovable and possess infinite mass (as shown below for the darker blue particles).
![particles](https://github.com/Sonryle/JoePhys/blob/main/testbed/example_media/Particles.gif)

### Springs
Particles can be connected using springs, each with a stiffness constant of any value.
![springs](https://github.com/Sonryle/JoePhys/blob/main/testbed/example_media/Springs.gif)

### Chunking
When collisions are enabled, each particle is sorted into a world grid, and collision resolution is computed only within the same chunks. This optimization eliminates unnecessary checks between distant particles and greatly improves performance in large, scattered worlds.
![chunking](https://github.com/Sonryle/JoePhys/blob/main/testbed/example_media/Chunking.gif)

### Deterministic
JoePhys is deterministic, meaning that replaying a scene with the same settings will always reward you with the exact same result.
![deterministic](https://github.com/Sonryle/JoePhys/blob/main/testbed/example_media/Deterministic.gif)

### Air Resistance
Particles traveling through the air will experience drag, depending on their mass and velocity. You can see in the GIF below that smaller particles with less mass fall at a slower rate than larger particles with greater mass.
![air_resistance](https://github.com/Sonryle/JoePhys/blob/main/testbed/example_media/Air%20Resistance.gif)

### Runge-Kutta 2nd Order
Particle movement & velocity is approximated using Runge-Kutta 2nd order, a numerical method for solving ODEs (Ordinary Differential Equations). This results in an accuracy far above Euler's integration. Implementing this was tricky, as spring forces are dependant on particles relative positions, and so finding the acceleration of each particle at any given step forward in time proved difficult.
![runge_kutta](https://github.com/Sonryle/JoePhys/blob/main/testbed/example_media/Runge%20Kutta.gif)

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

## Usage

## Contributing

## Giving Credit Where Credit's Due

### Libraries Used

The JoePhys physics engine uses no libraries outside of the C++ Standard Library, however the testbed environment uses third party libraries help with creating windows, communicating with the GPU and displaying GUI. I am extremely grateful to all of the developers who spent great time and effort creating wonderful open source libraries for the benefit of everyone, and I could not have created JoePhys anywhere near as well (or even at all) without the use of these libraries to help me visualise and control JoePhys in a testing environment.

With that said, here is the list of all third party libraries used in the testbed environment:

* glad - (https://glad.dav1d.de)
* GLFW - (https://glfw.org)
* imgui - (https://github.com/ocornut/imgui)

### Resources Used

There is no way that I am smart enough to invent every formula and function for a realistic physics engine. This project is not an invention, it is a conglomeration of things which I have learned and researched from OTHER people and put together into a project. No code is copy pasted or mindlessly stolen, but I have taken a generous amount of inspiration from other projects and papers in order to create it. JoePhys is built on existing research and prior knowledge. This project was simply created as a learning opportunity.

Again, with that being said, here is a list of all of the resources I learned from to create JoePhys:

* CPhysics by Hayden Marshalla (https://github.com/HaydenMarshalla/CPhysics)

  * Much inspiration was taken from this project's code structure and rendering system. No code was explicitly
    copy pasted, but some of the functions are word-for-word exactly the same. (These functions are in Renderer.cpp
    and they are simply used for initiating and handling the renderer)

* Pezzza's Work (https://youtube.com/@PezzzasWork)

  * Pointed me in the right direction on getting started with particle physics and taught me about the optimisation where you break the world up into chunks

* Khan Academy (https://khanacademy.org)

  * Helped me to learn about derivatives, differential equations and the beginnings of particle collision resolution (along with many other math concepts along the way)

* Gonkee (https://youtube.com/@Gonkee)
 
  * Taught me about Runge-Kutta methods and the steps required to create soft body physics using springs

## License


