#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"
#include "GameConstants.h"
#include <unordered_set>
#include <cmath>

class StudentWorld;

class Actor : public GraphObject
{
public:
	// Constructor. Sets the proper X and Y coordinates, direction, image ID, and depth by passing those parameter to the GraphObject constructor
	// Assigned the StudentWorld* pointing to the object in which this actor resides
	Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth);

	// Destructor. Doesn’t do anything
	virtual ~Actor();

	// Action to perform each tick. Purely virtual since every actor has a different set of actions to perform. 
	virtual void doSomething() = 0;

	// Returns true if an actor is dead. Returns false by default for this base class so energyless actors don’t get removed after every tick
	virtual bool isDead() const;

	// returns true if an actor blocks movement
	// Returns false by default in this base class since Only pebbles block movement
	virtual bool blocksMovement() const;

	// Cause this actor to be be bitten, suffering an amount of damage.
	virtual void getBitten(int amt);
	// default actor isnt bitable

	// In general, causes this actor to be poisoned 
	// Since most actors aren’t poisonable, this base function implementation does nothing.
	virtual void getPoisoned();

	// Cause this actor to be be stunned.
	// Since most actors aren’t stunnable, this base function implementation does nothing.
	virtual void getStunned();

	// returns true if Actor can be Eaten
	// since only food is edible, this base function implementation returns false
	virtual bool isEdible() const;

	// returns true if Actor is a pheromone belonging to the indicated colony 
	// since only pheromone objects can return a value of true, this base function implementation returns false
	virtual bool isPheromone(int colony) const;

	// returns true if this actor is an enemy of an ant of the indicated colony
	// returns false by default in this base class
	virtual bool isEnemy(int colony) const;

	// Is this actor detected as dangerous by an ant of the indicated colony? 
	// since only triggerable actor and insect objects can return a value of true, this base function implementation returns false
	virtual bool isDangerous(int colony) const;

	// Is this actor the anthill of the indicated colony? 
	// since only anthill can return a value of true, this base function implementation returns false
	virtual bool isAntHill(int colony) const;
	// Default false

	// Get this actor's world.
	StudentWorld* getWorld() const;

	// returns true if this actor is stunnable
	// this base function implementation returns false since most actors are not stunnable
	virtual bool isStunnable() const;

	// returns true if this actor is poisonable
	// this base function implementation returns false since most actors are not poisonable
	virtual bool isPoisonable() const;

	// returns true if this actor turns into food when it dies
	// this base function implementation returns false since most actors dont become food
	virtual bool becomesFoodUponDeath() const;

	// returns a random direction, all equally probable
	GraphObject::Direction genRandomDir();

private:
	StudentWorld* m_world;
};

class Pebble : public Actor
{
public:
	// constructor, sets indicated StudentWorld pointer and starting coordinates
	// also sets direction to right and assigns the proper image ID
	Pebble(StudentWorld* sw, int startX, int startY);

	// Destructor. Doesn't do too much
	virtual ~Pebble();

	// Does nothing. its a pebble
	virtual void doSomething();

	// Returns true since pebbles block movement
	virtual bool blocksMovement() const;
};

class EnergyHolder : public Actor
{
public:
	// constructor. Sets proper StudentWorld pointer, starting coordinates, direction, energy, imageID, depth, colonial affiliation and Compiler pointer
	EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth, int colony, Compiler* compiler);

	// Destructor. Does nothing;
	virtual ~EnergyHolder();

	// Supposedly does something but its purely virtual since every EnergyHolder has different actions
	virtual void doSomething() = 0;

	// Returns true when this EnergyHolders hitpoints or energy reaches 0 (or less)
	virtual bool isDead() const;

	// Get this actor's amount of energy (for a Pheromone, same as strength).
	int getEnergy() const;

	// Adjust this actor's amount of energy upward or downward.
	void updateEnergy(int amt);

	// Add an amount of food to this actor's location
	// either adds to an existing food object or makes a new one
	void addFood(int amt);

	// Have this insect pick up up to 400 units of food.
	// returns amount of energy successfully picked up
	int pickupFood(int amt);

	// Have this actor pick up an amount of food and eat it.
	// returns amount of energy successfully eaten
	int pickupAndEatFood(int amt);

	// returns the amount of food carried by this energy holder
	int getCarriedFood() const;

	// increases or decreases the amount of food carried by this holder 
	void updateCarriedFood(int amt);

	// Sets carry capacity
	void setCarryCapacity(int cap);

	// Does this actor become food when it dies?
	// returns false by default since only insects become food
	virtual bool becomesFoodUponDeath() const;

	// returns the colony to which this EnergyHolder belongs
	int getColony() const;

	// returns the compiler associated with this Energy Holder
	Compiler* getCompiler() const;

private:
	int m_energy;		// remaining hitpoints of this object
	int m_colony;		// colony of this object
	int m_carriedFood;   // amount of food being carried by this object
	int m_carryCapacity; // max amount of food that this energyholder can carry
						 // default set to 20000, only ants will have a custom value (1800)
	Compiler* m_compiler;	// compiler pointer of this object
};

class Food : public EnergyHolder
{
public:
	// sets StudentWorld pointer, and starting coordinates and energy
	Food(StudentWorld* sw, int startX, int startY, int energy);

	// Destructor. Does nothing;
	virtual ~Food();

	// Does nothing. It's food
	virtual void doSomething();

	// Returns true since food is edible
	virtual bool isEdible() const;
};

class AntHill : public EnergyHolder
{
public:
	// constructor. Sets proper StudentWorld pointer, starting coordinates, direction(right), energy(8999), imageID, depth, colonial affiliation and Compiler pointer
	AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program);

	// Destructor. Does nothing;
	virtual ~AntHill();

	// Decreases queens hp by 1
	// checks if it's dead, if so return immediately
	// attempts to eat up to 10000 units of food on its tile
	// attempts to create ants in its tile with 2000 hp (only if it has 1500 or more hp)
	virtual void doSomething();

	// true if colony parameter matches anthill colony
	virtual bool isAntHill(int colony) const;

private:

	// returns appropriate ant image id belonging to the indicated colony
	int getProperAntID(int colony);
};

class Pheromone : public EnergyHolder
{
public:
	// constructor
	// sets indicated StudentWorld, starting coordinates and colony
	// sets direction to right and sets proper imageID
	Pheromone(StudentWorld* sw, int startX, int startY, int colony);

	// Destructor. Does nothing;
	virtual ~Pheromone();

	// does nothing
	virtual void doSomething();

	// returns true
	virtual bool isPheromone(int colony) const;

	// Increase the strength (i.e., energy) of this pheromone.
	// caps total strength to 768
	void increaseStrength();

private:
	// returns the corresponding IID for this pheromone type
	int getProperIID(int colony);
};

class TriggerableActor : public Actor
{
public:
	// constructor
	// sets indicated StudentWorld and starting coordinates
	// sets direction to right
	// sets imageID to the passed imageID parameter
	TriggerableActor(StudentWorld* sw, int x, int y, int imageID);

	// Destructor. Does nothing;
	virtual ~TriggerableActor();

	// returns true. poison and pools of water are dangerous
	virtual bool isDangerous(int colony) const;

	// purely virtual because we dont want to create objects of this class
	virtual void doSomething() = 0;

};

class WaterPool : public TriggerableActor
{
public:
	// sets indicated StudentWorld and starting coordinates
	// passes WaterPool image ID to TriggerableActor constructor
	WaterPool(StudentWorld* sw, int x, int y);

	// Destructor. Does nothing;
	virtual ~WaterPool();

	// attempts to stun all actors in its current position
	virtual void doSomething();
};

class Poison : public TriggerableActor
{
public:
	// sets indicated StudentWorld and starting coordinates
	// passes Poison image ID to TriggerableActor constructor
	Poison(StudentWorld* sw, int x, int y);

	// Destructor. Does nothing;
	virtual ~Poison();

	// attemps to poison all actor in its current tile
	virtual void doSomething();
};

class Insect : public EnergyHolder
{
public:
	// sets indicated StudentWorld, starting coordinates, energy, imageID, colony and compiler 
	// sets a random starting direction for every insect
	Insect(StudentWorld* world, int startX, int startY, int energy, int imageID, int colony, Compiler* comp);

	// Destructor. Does nothing;
	virtual ~Insect();

	// purely virtual since all insects have different actions
	virtual void doSomething() = 0;

	// Makes this insect's energy decrease by amt
	virtual void getBitten(int amt);

	// Poisons this insect
	virtual void getPoisoned();

	// Stuns this insect, but checks if it wasn't already previously
	// standing on the same WaterPool object
	virtual void getStunned();

	// true if this insect's colony doesnt match caller's colony
	virtual bool isEnemy(int colony) const;

	// same function as isEnemy. Needed to be overloaded for proper behavious
	virtual bool isDangerous(int colony) const;

	// Returns true since all insects turn into food when they die
	virtual bool becomesFoodUponDeath() const;

	// Set x,y to the coordinates of the spot one step in front of this insect.
	void getXYInFrontOfMe(int& x, int& y) const;

	// Move this insect one step forward if possible, and return true;
	// otherwise, return false without moving.
	virtual bool moveForwardIfPossible();

	// Increase the number of ticks this insect will sleep by the indicated amount.
	void increaseSleepTicks(int amt);

	// Returns number of turns left to sleep
	int getRemainingSleepTicks() const;

	// resets the recorded coordinates of the waterPool object that last stunned this insect
	void resetLastStunned();

	// makes the insect get hungrier
	// checks if insect is dead. If it is, return false
	// cehcks if insect is sleeping or stunned
		// if it is, decrement sleep counter, return false
	// return true if insect isnt dead or sleeping
	bool doIDoSomethingThisTick();

private:
	int m_sleepTicks;
	int m_distanceLeft; // distance Insect still has to walk towards a direction
	int m_lastStunX; // x coordinate of waterpool that last stunned this insect
	int m_lastStunY; // y coordinate of waterpool that last stunned this insect
							  
};

class Ant : public Insect
{
public:
	// Constructor: sets the indicated student world, starting coordinates, colony, compiler program,
	// imageID and reset memory (remembering whether it was bitten or blocked in the tile)
	// Also sets the ants carry capacity to 1800
	Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID);

	// Destructor. Does nothing;
	virtual ~Ant();

	// In order:
	// decreases ant's hp by 1
	// Check is the ant is stunned or sleeping 
		// if yes, decrease sleep count
	// if no, interpet a command by calling commandInterpreter()
	virtual void doSomething();

	// Damages this ant and makes it remember that it got bitten
	virtual void getBitten(int amt);

	// returns true if the indicated colony differs from this ants colony
	virtual bool isEnemy(int colony) const;

	// attempts to  move the ant forward and remembers if it gets blocked
	virtual bool moveForwardIfPossible();

	// make this ant emit a pheromone
	void emitPheromone();
	

	// the following return true since the base function returned false
	// but BabyGrasshoppers are both stunnable and poisonable
	virtual bool isStunnable() const;
	virtual bool isPoisonable() const;

private:
	// Interprets and executes commands from the Compiler object
	// Kills the ant if there's an error
	bool commandInterpreter();

	// manipulates the instruction counter as indicated
	// if the specified condition passed by the if_command is true
	// returns true if condition was true, false otherwise
	bool ifCommandHandler(const Compiler::Command& cmd);

	int m_ic;
	bool m_blocked;
	bool m_bitten;
	int m_lastRandNum;
};

class Grasshopper : public Insect
{
public:
	// constructor, sets indicated Student world, starting coordinates, energy and imageID
	// sets random distance for grasshopper to travel
	Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID);

	// Destructor. Does nothing;
	virtual ~Grasshopper();

	// Baby and adult grasshopper still have different actions so make this pure virtual
	virtual void doSomething() = 0;

	// Makes grasshopper attempt to eat 200 units of food
	// gives it 50% chance of sleeping if it does eat
	// returns true if grasshopper wants to sleep
	bool attemptToEatHere();

	// Makes grasshopper check its distance goal in the direction it wants to travel
	// is it still has to walk, make it walk one tile
	// otherwise, set a new movement goal
	void makeGrassHopperMove();

	// travel distance related functions
	// sets the distance that the insect has to travel to random number from 2 to 10
	void genNewDistanceGoal();

	// modifies distance goal by adding num
	void adjustDistanceGoal(int num);

	// returns distance goal
	int getDistanceGoal() const;

private:
	// Distance that this Grasshopper still wants to move in its current direction
	int m_distanceLeft;
};

class BabyGrasshopper : public Grasshopper
{
public:
	// constructor: sets indicated StudentWorld and starting coordinates
	// sets appropraite imageID and sets energy to 500
	BabyGrasshopper(StudentWorld* sw, int startX, int startY);

	// Destructor. Does nothing;
	virtual ~BabyGrasshopper();

	// In order:
	// Decreases hp by 1
	// Checks if dead and returns immediately if so (StudentWorld will convert it to food)
	// Checks if stunned/sleeping. 
		// Decrements sleep counter if so
	// Checks if hp >= 1600
		// Creates an Adult Grasshopper in its tile
		// kills itself then returns immediately
	// Attempts to eat up to 200 units of food in its square (
		// Increases sleep duration with a probability of 0.5 if it eats and returns immediately
	// Check if finished walking in desired direction
		// if yes, pick new direction and distance 
	// Attempt to move one tile in current distance and direction
		// if blocked from moving, set desired distance to zero and increase sleep duration, then return immediately
		// if successful, decrement distance counter
	// Increase Sleep duration
	virtual void doSomething();

	// the following return true since the base function returned false
	// but BabyGrasshoppers are both stunnable and poisonable
	virtual bool isStunnable() const;
	virtual bool isPoisonable() const;
};

class AdultGrasshopper : public Grasshopper
{
public:
	// constructor: sets indicated StudentWorld and starting coordinates
	// sets appropraite imageID and sets energy to 1600
	AdultGrasshopper(StudentWorld* sw, int startX, int startY);

	// Destructor. Does nothing;
	virtual ~AdultGrasshopper();

	// In order:
	// Decreases hp by 1
	// Checks if dead and returns immediately if so (StudentWorld will convert it to food)
	// Checks if stunned/sleeping. 
		// Decrements sleep counter if 
	// With a 1/3 chance, attempt to bite a random enemy in this tile
	// With a 1/10 chance, attempt to skip to an open tile within a radius of 10 units
	// Attempts to eat up to 200 units of food in its square
		// Increases sleep duration with a probability of 0.5 if it eats and returns immediately
	// Check if finished walking in desired direction
		// if yes, pick new direction and distance 
	// Attempt to move one tile in current distance and direction
	// if blocked from moving, set desired distance to zero and increase sleep duration, then return immediately
	// if successful, decrement distance counter
	// Increase Sleep duration
	virtual void doSomething();
	
	// Reduces energy by amt then attempts to bite back (1/2 chance)
	virtual void getBitten(int amt);
};



#endif // ACTOR_H_