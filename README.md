## MyEngine
This is  a first attempt to create a physics engine in c++ !

# Intro
This project is a small game where you have to survive destroying incoming asteroids to your position.
There are 5 plantes where gravity force will make it harder for you to know they trajectory! Defend yourself!
Also works as a particle collision simulator.

## Controls

# Player Movement
-> AWSD to move.
-> Left click to shoot.
-> 1,2,3 to siwtch between weapons.
-> 5,6 swap player movement type; 5->velocity, 6->forces.

#  Weapons
-> 1 Blaster: Shoots a laser bullet that is not afected by gravity, destroyed on impact.
-> 2 Cannon: Shoots heavy bombs, the bomb explodes when it stops. vxy = 0;
-> 3 Bouncer: Shoots medium weight bullet, this bullet will bounce 3 time agains any wall, and then explode. Destroyed on impact with enemy.

# Game Loop Controls
-> G: God mode.
-> R: Restart game.
-> P: Stop/Start game.

# Other Controls
-> I: activate particle system, recomended to see with debug mode.
-> F1: Normal gravity, downwards.
-> F2: Satelite gravity, there is a big mass orbiting the screen, its a sun? its a starship? we dont know.
-> F5: Interplanetary gravity, it activates the gravity fields of the planets.
   ¡There is feed back on what is the current active gravities on top left corner, F1->yellow,F2->green,F3->blue.!
-> F4: Debug mode.
-> F5: Switch between 30/60 fps.
