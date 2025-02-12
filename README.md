# JoePhys

This is a simple physics engine which I decided to make for my TY physics project. I hope that my teacher will like it :)

## How to Run
There are no releases to JoePhys yet (I havent even got a window to work yet, i dont know why i'm making this README file right now). You must build from the source code

## Building

### Building on Windows
Requirements:
  You must have CMake installed (https://cmake.org/download/)
  You must have Git installed (https://git-scm.com/downloads)

Open powershell and navigate to where you want the JoePhys main folder to be downloaded.
You can clone the JoePhys repository using Git:
```sh
git clone https://github.com/Sonrile/JoePhys
cd JoePhys
```
Then build the project:
```sh
mkdir build
cd build
cmake ..
```
You will be met with a Visual Studio project file (JoePhys.sln)
Now just open Visual Studio and build the project (dont forget to set the config to Release!)

#### If you don't want to install Visual Studio
if you simply want to enjoy JoePhys without having to install visual studio, the process would look something like this:
```sh
git clone https://github.com/Sonrile/JoePhys
cd JoePhys
mkdir build
cd build
cmake ..
cmake --build . --config Release
```
