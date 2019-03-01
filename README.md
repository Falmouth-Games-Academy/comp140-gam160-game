# comp140-worksheetB

## Part A - Arduino Game Controller Project

### Overview

For my game controller project I intend to create a game in the visual style of the original Elite games, or as close as possible, in 3D space.
As for the game controller itself, I have been thinking about ways to implement a 2-part controller:
* One part that would control the Player Ships's rotation on the 3 axis (and perhaps even movement speed) and
* Another part that would control the rest of the ships components, such as weapons, and a single stick that controls movement.

### The game controller

The layout will consist of 2 parts: the right side will contain the rotation controls, while the left side will control the ships' components and thrust.

For the Rotation and/or movement part, I intend to use a Gyroscope (for rotations) attached to a potentiometer (for acceleration).

For the ship components controls' I intend to use, for starters, a single button layout for firing the weapons.

As for the exterior design of the controller, I was thinking about having either a button panel (for controlling the weapons) and some sort of outer casing (or fighter-style handle) for the gyro and acceleration potentiometer. Of course, that would mean that it would be able to rotate along all 3 axis as well as move forwards or backwards to determine the acceleration speed. As this may be cumbersome, I have also prepared an alternative layout.

Finally, I intend to attach several LED's to the second part to display warnings such as fuel remaining, proximity warnings, damage wearnings etc. (Mostly for immersion)

![Layout idea](https://github.com/Powll/comp140-worksheetB/blob/master/Images/Controller_Layout.png "Layout idea")

The image above represents one of the layout ideas I had in mind, with separate controls for rotation and movement, while the image below illustrates a 3D prototype I put together just to illustrate the style I was looking to achieve. Then again, the style is subject to change, as I will need to research the 3D printing solutions I can make use of.

It should be noted that the 3D stick model contains the LED's at the top for different warnings and a big red button that could be used for firing the ship's weapons.

![Stick idea](https://github.com/Powll/comp140-worksheetB/blob/master/Images/Controller.png "Stick idea")

### The game itself

For the game itself I intend to create a simple 3D space exploration game, with some basic interactivity (such as landing on certain stellar objects or crashing into planets) and -possibly- some basic combat. As for the type of game I will be aiming for, I would most likely go for sanbox, as it presents little restrictions to the player, and much creative freedom for me to add and remove ideas as I continue the development process. 

### Expected difficulties

As for the biggest setbacks I expect to encounter, I could name:
* My lack of knowledge in SDL will likely slow the initial development
* Since I will be using OpenGL I will need to research it intensely
* AI for enemies will certainly prove difficult, but I am confident that I can work it out
* Given the overall complexity of the game, I will need to work with the overall performance in mind
* I lacke xperience in working with electronics, so I will have to do some research on how to actually construct the controller
* I am still unsure whether to go for a cockpit-view or third-person view for the ship. 

### External references

[HOTAS controllers](https://www.windowscentral.com/best-hotas-controllers-vr): This controller is the go-to for the space exploration-combat-trading game Elite: Dangerous, which makes use of 2 main stick that control ship movement, rotation and weapon aiming. They also have several buttons to facilitate controlling other ship systems with ease. While I could not possibly hope to achieve a design as ergonomic or aesthetically pleasing as that, I have taken some inspiration from it mainly due to the ease of usage and immersion factor it provides to the player (The controller is visually identical to the in-game controls of the player).

[Logitech Flight Simulator Throttle](https://www.logitechg.com/en-gb/products/flight/flight-simulator-throttle-quadrant.html): This controller is similar to what I hope to use for the acceleration controller of the ship, using a potentiometer to determine the thrust given to the ship. One possible usage is to have 0 thrust at the middle position of the potentiometer and positive values when turning it one way and negative thrust when turning it the other way.

[Logitech Flight Simulator Switch Controller](https://www.logitechg.com/en-gb/products/flight/flight-simulator-switch-panel.html): The switch panel here presents ample control options for various in-game settings. While I will certainly not make use of as many features, I hope to have some immersive features on my panel, such as LED's, buttons or even switches that control different aspects of the ship.

[Original Elite ships](https://www.c64-wiki.com/images/thumb/0/0b/EliteShipIdentificationChart.jpg/800px-EliteShipIdentificationChart.jpg): These are the ships present in the original Elite game. I have placed these here just to illustrate the sort of visual style I will be aiming for. Minimalist, clean and hopefully performance-friendly.

### Parts expected to use

* Gyroscope x 1
* Potentiometer x 1
* LED's x 4-8
* Button x 2-4
* Switches x 2 - OPTIONAL

### Ending notes

My intention as of now is to use the 2 stick layout, mainly due to the fact that I am not yet confident enough to attempt to incorporate all the LED's, potentiometers, gyros and buttons on a single stick. However, as work progresses, I would like to keep it as an open possibility, a personal "stretch goal", so to speak. I hope it will not represent an issue, given the materials required are mostly the same and the underlying idea is the same - controlling a ship.
