# Game Engine and Xenon 2000 Clone

A small 2D game engine written from scratch in C++, paired with a playable clone of *Xenon 2000*, the vertically scrolling shoot 'em up. The project was built as coursework for the Undergraduate in Games and Multimedia programme at Politecnico de Leiria, and later revisited to track down and fix a set of physics and collision bugs that were still open at submission time.

## Overview

The solution is split into two Visual Studio projects. `Engine` is a static library that owns the game loop, resource management, rendering, input, and physics, it has no dependency on anything outside itself. `Xenon2022` is the executable that consumes the engine's public API to build an actual game, and never touches SDL2 or Box2D directly. The idea behind the split was to keep the engine generic enough to support other 2D games built the same way.

The game itself puts the player in control of a spaceship at the bottom of the screen, fighting off waves of enemies that descend from the top while dodging asteroids and collecting power ups. The graphical assets are the original freeware set released by The Bitmap Brothers together with PC Format magazine for *Xenon 2000*.

## Features

**Engine**

- Component style game objects built around a shared `Object` base class, with `GameObject`, `LevelBackground`, and `UIText` as its main children.
- A `GameLevel` class that stores every object and background in a level and lets the game swap levels through `Engine::setLevel()`.
- Sprite tilemap rendering with both automatic and manual frame animation.
- Tiled and non tiled backgrounds, used for parallax scrolling as well as UI elements like the health bar and life counter.
- A sorting layer system so draw order can be controlled per object, at setup time or at runtime.
- Bitmap font based UI text rendering.
- Gamepad input handling through SDL2, including controller detection and button mapping.
- Physics and collision handling through Box2D.

**Xenon 2000 clone**

- A player spaceship with movement, three tiers of missile firepower, health, and lives.
- Two enemy types, rushers and loners, each spawned by their own timer based spawner.
- Drone packs that move together in a sinusoidal pattern.
- Metal and stone asteroids of varying sizes.
- Power ups for healing, upgrading firepower, and recruiting a companion ship.
- Score tracking and a scrolling parallax background.

## Tech stack

- C++ with Visual Studio 2022
- SDL2 for window creation, input, and rendering context
- Box2D 3.1 for collision detection
- GLAD and OpenGL for rendering
- GLM for math
- stb_image for texture loading

## Getting started

The project targets Windows and Visual Studio 2022.

1. Open `Xenon2022Engine.sln`.
2. Build the solution (this builds `Engine` as a static library first, then links it into `Xenon2022`).
3. Run `Xenon2022.exe`.

A gamepad is required to play, the game reads movement and firing through SDL's controller API and does not currently have a keyboard fallback. Move with the D-pad and fire missiles with the A button.

## Physics and collision fixes

The original submission shipped with a note in its own report flagging that objects were sometimes not detected by collisions, and that a fixed timestep was likely the missing piece. That turned out to be one of several issues found while going back through the physics code, all now fixed:

- **Physics stepping.** `b2World_Step` used to run once per object per frame instead of once per frame, so the simulated time drifted from real elapsed time depending on how many objects were alive. It now runs through a fixed timestep accumulator built from the real frame delta, so the simulation speed stays consistent regardless of object count or framerate.
- **Teleporting bodies.** Every object's Box2D body was moved with `b2Body_SetTransform`, which Box2D's own docs describe as an instant teleport with no notion of the path taken. That meant continuous collision could never catch a fast object sweeping past a thin collider between frames. Bodies are now driven with linear velocity computed from the object's own movement each frame, letting Box2D's continuous collision actually see the motion, then snapped back to the object's authoritative position after the physics step so the existing manual movement code still owns where everything ends up.
- **Off center hitboxes.** Collision shapes were built with an offset equal to half the object's own height, shifting every hitbox away from where its sprite was actually drawn, by different amounts depending on each object's size. Shapes are now centered on the body origin, matching the sprite.
- **One sided contact notification.** The contact listener only called `OnCollideEnter` on whichever object Box2D happened to place in the "A" slot of a contact, and Box2D does not guarantee that ordering. A missile landing in slot A meant nothing happened at all, since missiles have no reaction of their own, only the enemy they hit does. Both objects in a contact are now notified, matching how engines like Unity handle collision callbacks.
- **Uninitialized pointers.** A couple of object types disable Box2D entirely (`hasBox2d = false`) but their body pointers were never given a default value, leaving the engine reading uninitialized memory for them every frame. They are now initialized to `nullptr`.
- **Health bar not refreshing.** Taking damage correctly reduced the ship's health value, but nothing told the health bar UI to redraw, so it only ever changed on healing or respawning. `spaceship` now refreshes the bar right after applying damage.

## Assets and credits

The *Xenon 2000* graphics are freeware, originally released by The Bitmap Brothers in association with PC Format magazine, archived at [archive.org](https://archive.org/details/Xenon_2000_Project_PCF). Box2D, SDL2, GLM, GLAD, and stb_image are used under their respective licenses, see `Engine/Dependencies` for each library's own terms.

## Authors

Xavier Martin Lopes and Hugo Ferreira, Undergraduate in Games and Multimedia, Politecnico de Leiria.

## License

This project is licensed under the Apache License 2.0, see `LICENSE` for details.
