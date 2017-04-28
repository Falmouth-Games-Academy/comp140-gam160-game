# comp140-gam160-game
Repository for Assignment 1 of COMP140-GAM160
Link to Github Repository: https://github.com/JamesCollins1/comp140-gam160-game

In my game, you take up the role of a submarine commander. There is a continual list of commands for the player to fulfil, to gain points the player must complete the tasks correctly. However, if the player presses the wrong button of list of buttons, they will lose a life. When they lose all their lives the game ends as the submarine would have sunk. The game increases in difficulty after each successful completion of a command. As the difficulty increases the time between each command decreases. This aims to increase the stress level of the player, making them more likely to make mistakes and lose lives.

The controller I have been making in tandem links directly to my game. I have included a set of 3 toggle switches, 1 Momentary On-On, Toggle Switch, 1 Key Switch, 1, potentiometer, 2 Tilt Switches, 5 LEDs, 1 Tricolour LED and a buzzer in my controller.  I have chosen toggle switches as I feel they make the controller feel more like it is part of a submarine command panel. The Key switch helps to add a certain amount of novelty to the controller and makes the controller feel more like a submarine panel. The large momentary toggle switch also has a red flick-Up cover on it, which makes it seem like a missile switch, further adding to the submarine feel.

Planned FFeatures for the Future:
1. Getting unreal to reliably read several analog inputs as some of my controller can not be used currently.
2. Getting Unreal to communicate back to the controller, allowing LEDs to light up when their respective button needs activating.
3. Add extra events to the game, such as a 'roulette' mode where the leds will flash in a ring and the Led that is left on is the button that needs pressing.
4. Add an event where the LEDs begin to light up irrespective of which button needs pressing, adding an element of difficulty and disassociating the led from the buttons.

