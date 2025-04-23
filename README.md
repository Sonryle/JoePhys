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

Pre-built binaries for the library and testing environment are available here: https://binaries-get-your-binaries.com

If you are trying to compile the library, I will assume you have enough programming knowledge to do so by yourself, however I will supply step by step instructions on how to build the *testing environment* yourself.

### Building the testing environment on Windows
Requirements:
* ![Git](https://git-scm.com/downloads)
* ![CMake](https://cmake.org/download)  
* ![Visual Studio](https://visualstudio.microsoft.com/) or a C++ compiler supported by CMake

Open powershell and navigate to where you want the JoePhys main folder to be downloaded. You can clone the JoePhys repository using Git:
```sh
git clone https://github.com/Sonryle/JoePhys
```
Then build the project:
```sh
cd JoePhys/testbed
mkdir build
cd build
cmake ..
```
In `JoePhys/testbed/build` you will be met with a Visual Studio project file (`testbed.sln`). Open this file with Visual Studio and build the project, making sure to switch the project configuration from "Debug" to "Release".

#### If you don't want to use Visual Studio
If you would prefer not to install Visual Studio, you can build the testing environment using the following steps:
```sh
git clone https://github.com/Sonryle/JoePhys
cd JoePhys/testbed
mkdir build
cd build
cmake ..
cmake --build . --config Release
```
The resulting `testbed.exe` file will be in JoePhys/testbed/build/Release/testbed.exe

### Building the testing environment on Linux
Requirements:  
* You must have CMake installed
* You must have Git installed
* You must have Make installed

In your terminal simply clone the repo, generate the make files with CMake and then build the project
```sh
git clone https://github.com/Sonryle/JoePhys
cd JoePhys/testbed
mkdir build
cd build
cmake ..
make
```
The resulting `testbed` executable will be in JoePhys/testbed/build/JoePhys

## Usage

The JoePhys library is simple and straightforward to use. After including "JoePhys/JoePhys.hpp", all you need to do is create a new World.

```c++
#include <JoePhys/JoePhys.hpp>

int main()
{
 // World settings
 int simulation_hertz = 120; // number of simulation "steps" per simulated second (must match framerate for real time physics)
 int sub_steps = 8;          // number of smaller "sub" steps per simulation step
 vec2 gravity(0.0f, -9.8f)   // vec2 is included in JoePhys.hpp

 // Create world
 World myWorld();
 myWorld.Create(simulation_hertz, sub_steps, gravity);
}
```

World settings can be changed at any point

```c++
 myWorld.simulation_hertz = 200;
 myWorld.sub_steps = 16;
 myWorld.gravity.Set(0.0f, 0.0f);
 myWorld.chunk_scale = 1.0f; // Scale of each chunk in the world grid in meters
```

Creating a particle goes like this

```c++
 // Particle data
 vec2 position(0.0f, 0.0f);  // in meters
 vec2 velocity(0.0f, 0.0f);  // initial velocity of our particle (in meters per second)
 float elasticity = 0.75f;   // 0 = no velocity retained after collision, 1 = 100% velocity retained
 float radius = 0.5f;        // in meters
 float mass = 1;             // in grams
 bool is_static = 0;         // static particles are immovable and treated as if they had an infinite mass

 // Create particle
 Particle* myParticle = new Particle(position, velocity, elasticity, radius, mass, is_static);
```

Adding particles to the world is straightforward aswell, but it needs a tiny bit of explanation. 

Particles and springs are stored in "Clusters", which are then stored in the world. This is done in order to create a more organised world, as every group of particles and springs can be organised into a "Cluster".

```c++
 // Create a cluster to contain our particle
 Cluster* myCluster = new Cluster;
 myCluster->particles.push_back(myParticle);

 // Add the clutser to the world
 World.clusters.push_back(myCluster);
```

A pointer to the particle is now stored in the world and will be used in all future physics updates.

You can move the world forwards by one *simulation step* by calling the `Step()` function. Again, the number of simulation steps per *simulated second* is controlled by the *simulation_hertz* setting in `myWorld.simulation_hertz`)

```c++
 myWorld.Step(0);
```

Now the positions and velocities of every particle in our world will have updated.

There are certain flags that you can pass into the step function which can disable certain features of the world. These flags are:

* NO_GRAVITY
* NO_DRAG
* NO_PARTICLE_MOVEMENT
* NO_PARTICLE_COLLISION
* NO_SPRING_FORCES

these can be passed into the Step function like so:

```c++
 myWorld.Step(NO_GRAVITY);
 myWorld.Step(NO_PARTICLE_COLLISION);

 // Flags can be chained together
 myWorld.Step(NO_GRAVITY || NO_DRAG || NO_SPRING_FORCES);
```

Creating springs to join particles is an easy task too, simply create a spring and add it to a cluster

```c++
 // Spring data
 float length = 2;      // in meters
 float stiffness = 100; // in newtons per meter

 // Create our spring
 Spring* mySpring = new Spring(pointerToParticleOne, pointerToParticleTwo, length, stiffness);
 myCluster->springs.push_back(mySpring);
```

And that's it!! That is all you need to know to get started with JoePhys!

To wrap things up, here is an example of what a program using JoePhys could look like
```c++
#include <JoePhys/JoePhys.hpp>

#define PI 3.141592653589
#define simulation_hertz 120
#define sub_steps 16

World myWorld;

// Sets up a Newton's Cradle scene in the world
void SetUpWorld()
{
 myWorld.Create(simulation_hertz, sub_steps, vec2(0.0f, -9.8f));

 // Add anchor points for steel balls
 Cluster* anchors = new Cluster;
 for (int x = 0; x < 5; x++)
  anchors->particles.push_back(new Particle(vec2((x * 1.5f) - 3.0f, 4.0f), vec2(0, 0), 1.0f, 0.25f, 1, 1));

 // Add steel balls
 Cluster* balls = new Cluster;
 for (int x = 0; x < 5; x++)
  if (x == 0) // if its the left-most steel ball, it must be raised to the left so that it will fall
   balls->particles.push_back(new Particle(vec2((x * 1.5f) - 9.0f, 4.0f), vec2(0, 0), 1.0f, 0.75f, 5, 0));
  else
   balls->particles.push_back(new Particle(vec2((x * 1.5f) - 3.0f, -2.0f), vec2(0, 0), 1.0f, 0.75f, 5, 0));

 // Add springs connecting the balls to their anchor points
 for (int n = 0; n < 5; n++)
  anchors->springs.push_back(new Spring(balls->particles[n], anchors->particles[n], 6.0f, 10000.0f));
	
 // Add clusters to world
 myWorld->clusters.push_back(anchors);
 myWorld->clusters.push_back(balls);
}

void RenderWorld()
{
 // loop over every cluster in the world
 for (Cluster* c : myWorld.clusters))
 {
  for (Particle* p : c->particles)
   if (p->is_static)
    yourOwnRenderer.RenderCircle(p->pos, p->radius, DARK_BLUE);
   else
    yourOwnRenderer.RenderCircle(p->pos, p->radius, BLUE);

  for (Spring* s : world->clusters[c]->springs)
   yourOwnRenderer.RenderLine(s->particleA->pos, s->particleB->pos, BLACK);
  }
}

int main()
{
 SetUpWorld();

 while(1)
 {
  if(FrameLimitFunction(myWorld.simulation_hertz))
  {
   myWorld.Step(0);
   RenderWorld();
  }
 }
}
```

## Contributing

Forks of JoePhys are welcome! You are also welcome to submit pull requests, which I will look through and make a decision on, but I must warn you that (while I am not expecting any forks or pull requests at all hahaha) I will be fairly strict on what I allow to be added to JoePhys (don't feel discouraged if you want to add cool things, if I agree with them and think they are cool too, then I will 100% merge your pull request or create a separate branch for you)

## Credit

### Third Party Libraries Used

The JoePhys physics engine uses no libraries outside of the C++ Standard Library, however the testbed environment uses third party libraries help with creating windows, communicating with the GPU and displaying GUI.

I am extremely grateful to all of the developers who spent great time and effort creating wonderful open source libraries for the benefit of everyone, and I could not have created JoePhys anywhere near as well (or even at all) without the use of these libraries to help me visualise and control JoePhys in a testing environment.

With that said, here is the list of all third party libraries used in the testbed environment:

* glad - (https://glad.dav1d.de)
* GLFW - (https://glfw.org)
* imgui - (https://github.com/ocornut/imgui)

### Resources Used

There is no way that I am smart enough to invent every formula and function for a realistic physics engine. This project is not an invention, it is a conglomeration of things which I have learned and researched from OTHER people and put together into a project. No code is copy pasted or mindlessly stolen, but I have taken a generous amount of inspiration from other projects and papers in order to create it. JoePhys is built on existing research and prior knowledge. This project was simply created as a learning opportunity.

Again, with that being said, here is a list of all of the resources I learned from to create JoePhys:

* CPhysics by Hayden Marshalla (https://github.com/HaydenMarshalla/CPhysics)

  * Much inspiration was taken from this project's code structure and rendering system. No code was explicitly copy pasted, but some of the functions in the testing environment are word-for-word exactly the same. (These functions are in Renderer.cpp and they are simply used for initiating and handling the renderer)

* Pezzza's Work (https://youtube.com/@PezzzasWork)

  * Pointed me in the right direction on getting started with particle physics and taught me about the optimisation where you break the world up into chunks

* Khan Academy (https://khanacademy.org)

  * Helped me to learn about derivatives, differential equations and the beginnings of particle collision resolution (along with many other math concepts along the way)

* Gonkee (https://youtube.com/@Gonkee)
 
  * Taught me about Runge-Kutta methods and the steps required to create soft body physics using springs
 
* Wikipedia (https://en.wikipedia.org)

  * Gave me insight on some topics such as Runge-Kutta and particle collision resolution

## License

JoePhys' MIT license can be read [here](LICENSE).
