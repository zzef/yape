# Yape

Yape (Yet another physics engine) is a rigid body impulse based physics engine written in C++. This was written to gain better understanding into rigid body physics engines as well as my personal enjoyment. It is riddled with tonnes of issues that I hope to fix as the time goes by. Features include:

- Random polygon generation
- Collision detection (Separated axis theorem)
- Impulse based collision response
- Frictional impulse (Coulomb friction)
- Distance joints (Modelled with constraints)

<p align="center">
  <img src="https://raw.githubusercontent.com/zzef/zef-physics-engine/master/demos/demo2.gif">
</p>

<p align="center">
  <img src="https://raw.githubusercontent.com/zzef/zef-physics-engine/master/demos/demo1.gif">
</p>


## Useful resources for this project

- https://www.gafferongames.com/post/fix_your_timestep/
- https://www.youtube.com/watch?v=7Ik2vowGcU0 javidx9 on convex polygon collision detection using Seperated Axis theorem
- Randy Gauls impulse engine https://www.randygaul.net/projects-open-sources/impulse-engine/
- https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331
- https://www.randygaul.net/2013/03/28/custom-physics-engine-part-2-manifold-generation/
- http://www.dyn4j.org/2011/11/contact-points-using-clipping/
- https://www.gafferongames.com/post/collision_response_and_coulomb_friction/
- https://www.gafferongames.com/post/integration_basics/
- https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/previousinformation/physics3constraints/2017%20Tutorial%203%20-%20Constraints.pdf
- https://dyn4j.org/2010/09/distance-constraint/


## Todos and Further Improvements

- Implement stable stacking.
- Fix polygons disappearing bug.
- Fix random jitter.
- Implement some broadphase.
- Allow for user to exert force via click and drag.
- Remove unneccesary recalculations during manifold generation.
- Implement warm starting and impulse accumulation for better stability (especially at lower frames, sub 60fps).
- Finish Circle to circle and circle to polygon collision.

## Dependencies
- libsfml
- gcc

To install libsfml on linux, run the following:

`sudo apt-get install libsfml-dev`

## How to run
You will need linux to run. Please do the following to run:

`git clone https://github.com/zzef/zef-physics-engine.git`

`make`

`./engine`

## How to use

- Click screen to generate polygons and add them to the scene. 
- press 'i' to toggle interactive mode. 
- In interactive mode you can drag polygons around.
