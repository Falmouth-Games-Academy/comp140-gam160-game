# Handzer
An experimental game where you can play as your hand using a custom Arduino controller. Keyboard support is available by using the scroll wheel during the calibration stage, and typing 'kb' during the game.

Guide: https://github.com/LXShades/comp140-gam160-game/docs/game_guide.md

# Compiling
Requires SDL 2.0.7 and the FMOD API (v1.10.04 recommended).  
Place FMOD libraries in Game/fmod_lib.  
Place FMOD includes in Game/fmod_include  
Place SDL in Game/SDL-2.0.7  

# Architecture overview
![UML diagram](https://raw.githubusercontent.com/LXShades/comp140-gam160-game/master/docs/architecture.png)

See the code, heavily commented where appropriate, for further details. The following classes are fundamental to the engine and gameplay. This list is non-exhaustive.  

## Game
Game is the single class that manages the game loop. The Run() function basically kick-starts the entire game.  

Game contains the renderer, camera, game object list, gamestate, active level, and similar vital components.  

The class is not a Singleton as it may be useful to have several game states in the future. Instead the global variable 'game' refers to the running instance of the game. Notably it is the only global variable in the source code.  

Possible todos: Split object list into separate clas

## GameState
Derivable class that manages the default game loop actions for different parts of the game. For example, GameStatePlay is the default GameState during play, which updates and renders all objects; while GameStateEditor is the level editor.

## RenderManager
Handles the rendering of text and rectangles. Due to limited code refactoring time, most gameplay rendering is also handled in the Camera class.

## InputManager
Handles the input from the keyboard and mouse.

## GestureManager
Handles the conversion of Arduino or keyboard inputs into player gesture data (e.g. bounce movement, jumps). (todo: full translation--some gestures are still managed by the Player class)

## Serial
Handles the input from the Arduino. Todo: Rename to ArduinoManager?

## Camera
Handles the movement, zooming, and transformation of the game's view. Includes ViewTarget feature which lets it watch multiple objects. Also contains functions for rendering in-game sprites in the game view.  

Possible todos: Move rendering functions into RenderManager or another game view-specific class

## Object
An interactive gameplay object. Derivable. Contains virtual functions for the creation, destruction, update and rendering of objects.  

Several default behaviours are supported by this class. See UpdateFlags, where you can determine what features you'd like the objects to use during the game loop, such as UpdateGravity and UpdateDeathTimer.  

## Player
The glorious hand himself. Derives from Object. 

## Image
An image, perhaps loaded from an image file.

## Sprite
An instance of an image with rotation and scaling features. Supports the ability to create images directly from files, while interacting with ImageCache to ensure images aren't re-loaded.

## ImageCache
Filename-associative cache of images that have been loaded.
