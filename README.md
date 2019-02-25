# comp140-gam160-game
Repository for Assignment 1 of COMP140-GAM160

## Part A: The Game

Through the limitless potential of Arduino, I'm planning on encouranging people to drink more water by designing a game based around drinking from a drinking glass (or at the least tilting the glass further and further upwards as the game progresses).

Mechanics will include firing projectiles at enemies with a button, refilling ammunition by lifting the glass upwards (encouraging the player to drink water) and moving left and right by rotating the bottle anti-clockwise and clockwise respectively.

The player will be at the bottom of a screen, moving upwards towards enemies and enemy fire at a consistent rate. Their interaction will consist solely of moving along the X-axis to dodge enemy fire and shooting at enemies to destroy them. Ammunition is limited and can be generously refilled by lifting the glass upwards. 

The refilling of ammunition is not instant, the glass must remain raised for a short period of time while ammunition slowly refills. Windows of relative calmness will be displayed in advance to the player, giving them a brief moment of respite with which to take a drink and refill their ammunition.

The game itself will aim to last around two minutes and will feature at least two different difficulty modes, as I feel providing an 'easy mode' is beneficial to the general accessibility of the game. It will be heavily influenced by games of the 'bullet hell' genre like Touhou and Nuclear Throne, forcing the player to precisely dodge endless streams of attacks, although the restriction to one dimension of movement will provide a challenge in balancing the difficulty.

Being exclusively two dimensional in design will play to SDL's strengths, but design of the sprites and sound effects will also be a challenge. Another challenge, though not explicitly related to the game itself, will be preventing the possibility of water damage to the electrical components attached to the drinking glass. Both my artistic design and DIY craftsmanship skills will be tested heavily in this project, although I'm confident I can at least provide a well-functioning 2D video game experience.

## Part B: The Controller

My controller will consist of a steel 500 millilitre beaker and a simple gyroscope and button combo. I've chosen to use a beaker over a bottle to prevent cheating when it comes to refilling ammunition via drinking. With a bottle (or any drink container with a narrow neck), one can simply hold the water in place with their tongue and lips instead of actually drinking the water. With a beaker (or any similar container with a wide opening), one risks spilling the water all over themselves if they attempt this, looking rather foolish in the process. I know this from experience.

The actual wiring and complexity of the controller itself is very barebones and novice. There are a multitude of tutorials available online when it comes to appropriately connecting a gyroscope to Arduino.

As mentioned above, an obvious challenge with this design is the danger of water spilling onto the components. There are methods of waterproofing available to trial, including but not limited to; cling film, waterproof containers and resins like Epoxy.

Another less obvious challenge will be centering both the button and the gyroscope. Were the button off-center, it may be difficult to find during intense moments where the bottle is repeatedly twisted/rolled to move the character side-to-side. Were the gyroscope off-center, the input may be inconsistent and inaccurate, although this is of lesser priority than the button in my opinion.

When electrical components and liquids have the potential to interact, I understand that health and safety is of paramount importance. I also understand that external videogame testers (including younger children) may have the opportunity to test our games and controllers, further emphasizing the importance of a fool-proof design. It may be worth looking further into wireless options for Arduino, but connecting Arduino wirelessly appears to be somewhat complicated from first glance.
