# Poolio

Poolio will literally put the pool cue in your hands. Using a cradle to rest your actual physical cue into, the arduino will track it's movements to reflect your shots direction and power. 

## Mechanics

An inbuilt joystick will allow aiming your shot without having to leave the controller but it is in the way that you perform a shot that the game comes into it's own. 
The arduino will track the movement of the cue to trigger a shot and reflect the appropriate amount of force applied.  By angling the cue up and down, screw or top spin may be applied to the object ball, whilst left and right adjustments may be used to replicate adding swerve or to fine tune aiming.

The game itself will be relatively simple and well established; A physics based ball rolling game leaving most of the mechanics to Unreal Engines own physics module. The rules will initially be English pool rules but the amount of variations available leave much scope for games such as snooker, 8 ball, or killer.

## Research

Currently only one other motion based snooker controller exists; the Wii snooker cue add on, for use in the different pool and snooker games available for the Nintendo Wii.  Reviews are mixed from [Amazon](https://www.amazon.co.uk/WSC-Real-09-Snooker-Championship/dp/B0029LHMZO) customers and [criticisms](https://www.amazon.co.uk/d/PC-Video-Games/Logic-3-NW847-Wii-Snooker-Cue/B0016ZEX4W/ref=sr_1_6?s=videogames&ie=UTF8&qid=1490018062&sr=1-6&keywords=wii+snooker) are of the build quality, not the concept of a held cue controller.

There is also a game called [SportsBarVR](http://www.poolnationvrgame.com/) which despite not using a physical controller, can certainly provide inspiration as a pool game that does not rely on a traditional controller.

![alt text](https://github.com/Stompyy/comp140-gam160-game/blob/master/Wii_snooker.jpg)
![alt text](https://github.com/Stompyy/comp140-gam160-game/blob/master/VRPOOL.jpg)

## Hardware

Relying on a real pool cue that can either sit in a cradle or have the arduino attached to the back section, aiming will be controlled with a joystick and a button to toggle fine tuning.  

There are many different options available to measure the forward and backwards movement of the cue to take a shot, and the favourite currently is to translate that movement onto a motor to generate a voltage. This will give both a magnitude and a duration which can be used to return a shot velocity. All controls have been set out in the input settings of unreal to allow easy mapping to any arduino inputs.
