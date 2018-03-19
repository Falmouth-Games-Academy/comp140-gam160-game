# Alternative Controller Game Concept

## Game:
The game I want to make will be a 2D puzzle/puzzle platformer that is created in a 3D world, taking insparation from games such as [Fez](http://fezgame.com/), the character will be controlled by the player in a familliar fashion to 'regular' games such as using either WASD, arrow keys, or even a joystick. However whilst doing this the player will also have to control the world in order to complete the puzzles, This will be done by rotating the alternative controller in a similar fashion to how a rubiks cube works (See gif below)
![alt text](https://media0.giphy.com/media/XhLfu6KgEyM5q/giphy.gif "How a rubiks cube works.")

When a player moves one part of the alternative controller the world in the game will rotate simultaniously and this will be used to locate the character through the level by moving platforms, that were just out of reach, close to the character so that they can progress through the level. The player will have to move the world carfully as they can quickly cause chaos in the world if they're not paying attention to where they are moving traps to and are making fall due to gravity, which could be turned on and off using a switch. The game will also feature a timer and a minimal moves scoring system that will allow player's to compete with one another to get the biggest score and fastest times whilst also causing minimal chaos in the world. The player should also be able to rotate the physical controller and the world in the game will rotate with it allowing the player to view all sides of the world before rotating so they know exactly how the world will change once they have rotated it.    


## Controller Design:

When coming up with an idea for a game that will use an alternate controller I first did some research on some games that have done this before. I used the Alt-Ctrl [Twitter](https://twitter.com/altctrlgamejam?lang=en) as well as their [website](http://altctrlgamejam.com/) to find these games and I then looked into how their controllers work with the game [here](https://docs.google.com/document/d/e/2PACX-1vScrenIGp2Ds6mMtKWdtPZEu1V-i9g4Sz4i8Cimpg2ha41mJ2hxjuk9bBVxqvJxw3RJBziIIOdFH4Lk/pub).

For the controller I am going to use 360° rotary encoders that will be attached to the middle block of each face of the cube, this will allow me to know when each face of the cube is rotated and can keep track of each block based on the rotation of each encoder. I will also try to use something that will keep the orientation of the cube so I can keep track of how the player is holding it and can change the games view so the player can see each side of the world.

## Revising My Design

Based on the difficulties and issues that I ran into when creating the controller for the pong game I might have to make my controller a lot simpler to be able to makie it work correctly. I first want to see if I can make my first design possible. However, if I do run into issues while doing this I will simplify it by having just two rotating sides that will change the rotation of a bit of the world and then have some buttons that will be used to select the part of the world that will be rotated based on these rotating sides that the play moves.

## Bill Of Materials

For my initial design of using a Rubik's cube as the controller I will need:

A minnimum of 6 360° rotary encoders that will be in controll of each face of the cube, these do not need any extra parts except the wires to connect it to the arduino. 

A toggle switch that will be used to turn the gravity on and off.

LED lights to cleary show the player the status of different things on the game nad controller, for example if the gravity is on/off.

A gyroscope or IMU will be needed to keep the orientation of the controller so the player can rotate the controller to see around the world, this could be changed to some buttons that will change the view if these don't work with the design. 

A bluetooth transceiver may be used if possible to allow the controller to be wireless allowing the player to freely move it without worrying about breaking or tangling the wires that are connected to it but this wouldn't be implemented until the rest of the parts are working correctly.

As a rubik's cube is compact it may be difficult to implement the arduino device into one, if it seems like I am unable to do this I will go to a back-up plan of a deconstructed cube design that will function in a similar way but will allow for the space needed to implement everything. 
