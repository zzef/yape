# zef-physics-engine

A Rigid body impulse based physics engine written in C++. This was written to gain better understanding into rigid body physics engines. 

## Useful resources for this project

- Randy Gauls impulse engine https://www.randygaul.net/projects-open-sources/impulse-engine/
- https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331
- https://www.gafferongames.com/post/collision_response_and_coulomb_friction/
- https://www.gafferongames.com/post/integration_basics/

## Main improvements

- Finish Circle to circle and circle to polygon collision
- Inertia tensor for polygons could be based on area giving better realistic movement
- Generate neon colors for each polygon
- Fix timestep (Currently directly tied to framerate so not very portable)
- Rotation and position vector applied at every vertex. This is unnecessary and slow.

## Dependencies
- libSDL2
- gcc

## How to run
You will need linux to run. Please do the following to run:

`git clone https://github.com/zzef/zef-physics-engine.git`

`make`

`./engine`

## How to use

- Click screen to generate polygons and add them to the scene. 
- press 'i' to toggle interactive mode. 
- In interactive mode you can drag polygons around.
