#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <string>
#include <unordered_set>
#include <iostream>
#include <list>

class Actor;
class Compiler;

class StudentWorld : public GameWorld
{
public:
		// constructor, sets game tick count to 2000,
		StudentWorld(std::string assetDir);

		// Destructor. calls cleanUp() to delete all Actors
		virtual ~StudentWorld();

		// loads field files and bug files 
		// and adds actors to the filed
		virtual int init();
	
		// Updates tick count, makes all actors do something,
		// removes dead actors, updates the game text,
		// and decides if an ant has won or not when the game end
		virtual int move();

		// deletes all actors
		virtual void cleanUp();

		// calls doSomething() for all actors
		// moves actors in the unordered_map if they moved in the field
		void makeActorsDoSomething();

		// Removes dead actors, readjusting the unordered_map & erasing empty lists
		void removeDeadActors();

		// Loads all actors into the unordered_map as specified in the field file
		int loadFieldFile();

		// Loads bugs files and populates the m_compilers vector
		int loadBugsFiles();

		// Updates the score and tick count text at the top of the screen
		void updateGameText();

		// Can an insect move to x,y?
		bool canMoveTo(int x, int y) const;

		// Add an actor to the world
		void addActor(Actor* a);

		// If an item that can be picked up to be eaten is at x,y, return a
		// pointer to it; otherwise, return a null pointer.  (Edible items are
		// only ever going be food.)
		Actor* getEdibleAt(int x, int y) const;

		// If a pheromone of the indicated colony is at x,y, return a pointer
		// to it; otherwise, return a null pointer.
		Actor* getPheromoneAt(int x, int y, int colony) const;

		// Is an enemy of an ant of the indicated colony at x,y?
		bool isEnemyAt(int x, int y, int colony) const;

		// Is something dangerous to an ant of the indicated colony at x,y?
		bool isDangerAt(int x, int y, int colony) const;

		// Is the anthill of the indicated colony at x,y?
		bool isAntHillAt(int x, int y, int colony) const;

		// Bite an enemy of an ant of the indicated colony at me's location
		// (other than me; insects don't bite themselves).  Return true if an
		// enemy was bitten.
		bool biteEnemyAt(Actor* me, int colony, int biteDamage);

		// Poison all poisonable actors at x,y.
		bool poisonAllPoisonableAt(int x, int y);

		// Stun all stunnable actors at x,y.
		bool stunAllStunnableAt(int x, int y);

		// Record another ant birth for the indicated colony.
		void increaseScore(int colony);


private:
	// Ticks remaining in the game, initialized to 2000;
	int ticksRemain;

	// array holding scores of ant colonies
	int m_colonyScores[4];
	
	// records tick count during which the score of an ant colony was last change
	int m_lastIncrement[4];

	// vector holding filenames of bug files
	std::vector<std::string> m_fileNames;

	// pointers to compilers for every ant colony
	std::vector<Compiler*> m_compilers;

	// 2D array or lists holding pointers to actors;
	std::unordered_map<int, std::list<Actor*>> m_fieldActors;
};

#endif // STUDENTWORLD_H_