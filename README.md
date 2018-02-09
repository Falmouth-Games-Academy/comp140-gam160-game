# comp140-gam160-game
Repository for Assignment 1 of COMP140-GAM160

I propose to develop a 2d platformer using a very specific setup of the Arduino alternative controller.
The physical piece of hardware the player will be interacting with will be a small cube-like cushion sat
on their lap. This will be connected to the Arduino via 3 soft touch sensitive buttons on the cushion,
1. One on top of the cusion
2. One on the right side 
3. One on the left side

The game has the player control a Pufferfish, attempting to navigate the fish through a labyrinthine
underwater structure. The levels will be filled with tight corridors and hazzards (from stationary
spikes to mines that float up and down in a simple pattern) that will instakill the player.
The controls will be as follows,
+ Pressing the right side button moves the fish left
+ Pressing the left side button moves the fish right
+ Pressing the top button deflates the fish causing it to sink down
+ Pressing both side buttons inflates the fish causing it to float up

It is worth noting that both deflating and inflating are gradual changes in both elavation and size
that allow for holding of the input. Due to this mechanic I will be able to design levels with multiple
routes for different playstyles. For example, a level may split into both an upper path and a lower one.
The upper path will cause the player to have to control a large fish, so in turn the level design will
be more open and less complex, but as a con the controls may feel more floaty.
On the other hand, lower paths will feel twitchier, requiring smaller more precice movement to navigate.

Another mechanic I was considering were airbubbles found within the space. These would force the player
to inflate, essentially increasing the minimum size cap. These would be optional hazzards to avoid, where 
collecting too many could close off certain special paths or even cause the game to become impossible to
complete due to you being too large.
