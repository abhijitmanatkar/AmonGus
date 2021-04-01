# AmonGus

An impostor of a game.

## Installation and Running
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./Amon-Gus
```

## Guide
You play as red. Blue is the impostor and he is out to get you. You have to finish two tasks and get to the exit before time runs out.

Your first task : Kill Blue. 

There is a red button present at a random location in the maze. Get to it before you die, and you will successfully have vaporized Blue. 


Your second task: Collect some points

There is a yellow triangle present at a random location in the maze. Walk over it to release powerups. But be warned as this will also release obstacle boxes. Coming in contact with these boxes will reduce your health. Your task is to collect all the powerups for points.

Remember, Blue is constantly after you. He will deal you damage anytime he is near you.

If your health falls below zero or if you run out of time, the game is over.


## Controls
- Arrow keys to navigate.
- Q to quit.

## Features
- The maze is procedurally generated. There is a new maze at the start of every game.
- Shortest paths between all points are calculated using th Floyd-Warshall algorithm. The enemy uses these to pick the quickest route to get to the player.
- Cool text rendering using [gltext](https://github.com/vallentin/glText).
- No lighting effects :(
- Made with OpenGL and frustration.

<i>-Abhijit Manatkar (2019101108)</i>