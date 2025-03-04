# JoePhys

This is a simple physics engine which I decided to make for my TY physics project. I hope that my teacher will like it :)

## Building

### Building on Windows
Requirements:  
* You must have CMake installed (https://cmake.org/download/)  
* You must have Git installed (https://git-scm.com/downloads)

Open powershell and navigate to where you want the JoePhys main folder to be downloaded. You can clone the JoePhys repository using Git:
```sh
git clone https://github.com/Sonryle/JoePhys
cd JoePhys
```
Then build the project:
```sh
mkdir build
cd build
cmake ..
```
In JoePhys/build you will be met with a Visual Studio project file (JoePhys.sln) use this to compile the project.

#### If you don't want to install Visual Studio
If you simply want to enjoy JoePhys without having to install visual studio, the process would look something like this:
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
```bash
git clone https://github.com/Sonryle/JoePhys
cd JoePhys
mkdir build
cd build
cmake ..
make
```
The resulting JoePhys executable will be in JoePhys/build/JoePhys
