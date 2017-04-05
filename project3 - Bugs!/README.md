# Bugs!

Bugs is a simulation framework/game for up to four colonies of ants in a field with a bunch of other characters/objects (grasshoppers, food, etc). Each colony will be controlled by a unique set of instructions in the form of a program written in the custom-made BUGS! language. The idea is for us CS32 students to essentially create a programming contest platform for high/middle school students who would work towards writing the best BUGS! program for their colonies.

# Files I worked on:
* StudentWorld.cpp
* StudentWorld.h
* Actor.cpp
* Actor.h

# Work I've done
* Utilize good object oriented design and memory allocation to efficiently implement the Ant, Baby Grasshopper, Adult Grasshopper, Food, Pebble, Water Pool, Poison, Pheromone and Ant Hill classes and all the interactions between them
  * Setup a class heirarchy so that shared traits (in the form of variables and functions) are defined in the base class that could encompass the most number of related classes.
  * Examples of features that I implemented: hit points, movement, dealing damage, decoding of Bugs! language into C++ code for Ants, etc...
 * Test each class as thoroughly as possible to prevent unintended interactions between characters.
 * Wrote the functions that allowed the characters to interact with each other in an efficient manner within the playing field (e.g. setting up a data structure for keeping track of characters so that they could easily check if there's another actor that's adjacent to them)
 * 
