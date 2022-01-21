# Yape

Yape (Yet another physics engine) is a rigid body impulse based physics engine written in C++. This was written to gain better understanding into rigid body physics engines as well as my personal enjoyment. It is riddled with tonnes of issues that I hope to fix as the time goes by. Features include:

- Random polygon generation
- Collision detection (Separated axis theorem)
- Impulse based collision response
- Broadphase
- Frictional impulse (Coulomb friction)
- Joints (Modelled with constraints)
- Physics interpolation (For lower physics framerates)
- Collisions (mid-phase and narrow phase)
- Restitution (elastic and inelastic collisions)
- Physical properties (mass, area, density etc.)

<p align="center">
  <img src="https://raw.githubusercontent.com/zzef/yape/master/demos/final_demo1.gif" width="48%" />
  <img src="https://raw.githubusercontent.com/zzef/yape/master/demos/final_demo2.gif" width="48%" /> 
</p>

## Useful resources for this project

Here are some of the resources I used for this project. I know everyone loves Erin Catto but I found the blog posts by dyn4j (specifically on constraints) and Randy Gaul to be the most helpful.

- http://www.dyn4j.org/2011/11/contact-points-using-clipping/
- https://dyn4j.org/2010/09/distance-constraint/
- https://www.gafferongames.com/post/fix_your_timestep/
- https://www.youtube.com/watch?v=7Ik2vowGcU0 javidx9 on convex polygon collision detection using Seperated Axis theorem
- Randy Gauls impulse engine https://github.com/RandyGaul/ImpulseEngine
- https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331
- https://www.randygaul.net/2013/03/28/custom-physics-engine-part-2-manifold-generation/
- https://www.gafferongames.com/post/collision_response_and_coulomb_friction/
- https://www.gafferongames.com/post/integration_basics/
- https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/previousinformation/physics3constraints/2017%20Tutorial%203%20-%20Constraints.pdf
- Erin Catto's Fast and simple physics using sequential impulse https://vdocument.in/fast-and-simple-physics-using-sequential-impulses-erin-catto-crystal-dynamics.html
- https://github.com/erincatto/box2d-lite

## Todos and Further Improvements

- Spacial Hashing
- Implement warm starting (contact coherence) and impulse accumulation for better stability (especially at lower frames, sub 60fps).
- Fix polygons disappearing bug.
- Fix random jitter.
- Allow for user to exert force via click and drag (perhaps model with a point constraint).
- Remove unneccesary recalculations during manifold generation.
- Finish Circle to circle (left the easiest till last) and circle to polygon collision.
- Lots and lots of code refactoring.

## Dependencies
- gcc

## How to run
You will need linux to run. Please do the following:

`git clone https://github.com/zzef/yape.git`

`make clean`

`make`

`make install`

`./yape`

## How to use

- Click screen to generate polygons and add them to the scene. 
- press 'b' to switch to box generation mode.
- press 'i' to toggle interactive mode (in interactive mode you can drag polygons around).
- press 'r' to reset current demo
- press 'a' to toggle view constraints
- press 'x' to clear all polygons
- press 't' to toggle view contact points
