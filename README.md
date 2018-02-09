# comp140-gam160-game
Repository for Assignment 1 of COMP140-GAM160

# Shoes

Shoes will be a unity based experience based around the movement of a pair of shoes worn on the hands. These shoes function as 
alt-controllers complete with accelerometers and that will monitor the movement of said shoes.

 ### Gameplay
 
 The player will use these shoes to accomplish a number of small tasks including:
 
 ..* Kicking a ball toward a goal
 ..* Fending off attackers in a reflex kick off
 ..* Stomping on buildings as a giant
 ..* Opening in game menus using a kicking motion
 ..* Playing drums by stomping both shoes in a rythm
 ..* Driving a plane using a shoe
 ..* Plus any more I can make using my inputs within the time limit
 
 These tasks should be within scope as they will require minimal effort to create as the simple tasks will only require few scripts to work. The art requirements will be low as I plan to use a lot of 2d sprites that I'll only create if needed.
 
 If the player fails or succeeds at one of these tasks, the tasks is over and the next task appears on screen. This next task will be randomly selected from existing tasks, even if the player has already completed them. To compensate for this repetition, the speed of the tasks themselves as well as their time limits will be accelerated.
 
 Occasionally, the player will be given a spare lace (Extra Life) for beating so many tasks in a row flawlessly.
 
 If the player fails enough of these tasks and thus loses all of their spare laces (Extra lives), they recieve a game over. The goal is to reach 30 tasks completed in order to fight the final boss: The winged shoes of Hermes. The player must untie Hermes' shoes by beating the same tasks, but this time, one shoe will be used for one task. Both tasks will take place simultaneously, splitting the screen in half.These screens could also be reversed so that the left and right shoe control the right and left tasks respectively, in order to increase difficulty.
 
 Each time one of these dual tasks is accomplished, one of Hermes' shoes' laces will come undone. When all of the laces are off, the shoes fall away so the player can deliver a final blow.
 
 ### Technical Bits
 
 This game will be created within the Unity engine in the 2d mode, with the shoe controllers using an arduino and several sensor components in order to create an "alt-controller". The shoes themselves will be bought and imbedded with the needed electronics.
