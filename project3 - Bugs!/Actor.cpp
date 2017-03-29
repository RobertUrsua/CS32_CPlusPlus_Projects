#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include <cmath>
#include <iostream>
using namespace std;

////////// ACTOR FUNCTIONS ///////////////

Actor::Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth)
	: GraphObject(imageID, startX, startY, startDir, depth), m_world(world)
{}

Actor::~Actor() {}

// Is this actor dead?
bool Actor::isDead() const { return false; }
// Default false for inanimate objects so
// they dont get removed after every tick

// Does this actor block movement?
bool Actor::blocksMovement() const { return false; }
// Default false. only insects and pebble block movement

// Cause this actor to be be bitten, suffering an amount of damage.
void Actor::getBitten(int amt) {};
// default actor isnt bitable

// Cause this actor to be be poisoned.
void Actor::getPoisoned() {};
// since default actor isnt poisonable, do nothing

// Cause this actor to be be stunned.
void Actor::getStunned() {};
// defautl actor isnt stunnable

// Can this actor be picked up to be eaten?
bool Actor::isEdible() const { return false; }
// Default false as only one object, food is true

// Is this actor detected by an ant as a pheromone?
bool Actor::isPheromone(int colony) const { return false; }
// Default false as only one object is a pheromone

// Is this actor an enemy of an ant of the indicated colony?
bool Actor::isEnemy(int colony) const { return false; }

// Is this actor detected as dangerous by an ant of the indicated colony?
bool Actor::isDangerous(int colony) const { return false; }
// Default false as only poison, water and enemy ants are dangerous

// Is this actor the anthill of the indicated colony?
bool Actor::isAntHill(int colony) const { return false; }
// Default false

// Get this actor's world.
StudentWorld* Actor::getWorld() const { return m_world; }

// Is it stunnable
bool Actor::isStunnable() const { return false; }

// Is it poisonable
bool Actor::isPoisonable() const { return false; }

bool Actor::becomesFoodUponDeath() const { return false; }


GraphObject::Direction Actor::genRandomDir() {
	int randNum = randInt(0, 3);
	switch (randNum) {
	case 0:
		return GraphObject::right;
		break;
	case 1:
		return GraphObject::left;
		break;
	case 2:
		return GraphObject::up;
		break;
	case 3:
		return GraphObject::down;
		break;
	}
	return GraphObject::right;
}




///////////// PEBBLE FUNCTIONS ////////////////

Pebble::Pebble(StudentWorld* sw, int startX, int startY)
	: Actor(sw, startX, startY, right, IID_ROCK, 1) {}

Pebble::~Pebble() {}

void Pebble::doSomething() {}

bool Pebble::blocksMovement() const { return true; }



////////////////// ENERGY HOLDER FUNCTIONS /////////////////

EnergyHolder::EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth, int colony, Compiler* compiler)
	: Actor(sw, startX, startY, startDir, imageID, depth), m_energy(energy), m_colony(colony), m_compiler(compiler), m_carriedFood(0), m_carryCapacity(20000) {}

EnergyHolder::~EnergyHolder() {}

bool EnergyHolder::isDead() const { return m_energy <= 0; }

// Get this actor's amount of energy (for a Pheromone, same as strength).
int EnergyHolder::getEnergy() const { return m_energy; }

// Adjust this actor's amount of energy upward or downward.
void EnergyHolder::updateEnergy(int amt) { m_energy += amt; }

// Add an amount of food to this actor's location.
void EnergyHolder::addFood(int amt) {
	// if food already exists here add 100 units
	Actor* foodHere = getWorld()->getEdibleAt(getX(), getY());
	if (foodHere != nullptr) {
		dynamic_cast<EnergyHolder*>(foodHere)->updateEnergy(amt);
		return;
	}

	// or else just create new food
	Actor* newFood = new Food(getWorld(), getX(), getY(), amt);
	getWorld()->addActor(newFood);
	return;
}

int EnergyHolder::pickupFood(int amt) {
	// get pointer to a potential food
	Actor* foodHere = getWorld()->getEdibleAt(getX(), getY());
	if (foodHere != nullptr) {
		// if it is actually food do these things:
		Food* actualFood = dynamic_cast<Food*>(foodHere);
		int foodPickedUp = actualFood->getEnergy();
		// cap food pickup at amt units
		foodPickedUp = foodPickedUp > amt ? amt : foodPickedUp;
		// cap food pickup so that the carried food is always less than or equal to m_carryCapacity
		foodPickedUp = foodPickedUp <= m_carryCapacity - m_carriedFood ? foodPickedUp : m_carryCapacity - m_carriedFood;
		// decrase the food objects energy
		actualFood->updateEnergy(-foodPickedUp);
		// increase carried food
		m_carriedFood += foodPickedUp;
		return foodPickedUp;
	}
	return 0;
}
// Have this actor pick up an amount of food and eat it.
int EnergyHolder::pickupAndEatFood(int amt) {
	int energyToActuallyEat = pickupFood(amt);
	updateEnergy(energyToActuallyEat);
	return energyToActuallyEat;
}

// returns the amount of food carried by this energy holder
int EnergyHolder::getCarriedFood() const { 
	return m_carriedFood; 
}

// increases or decreases the amount of food carried by this holder 
void EnergyHolder::updateCarriedFood(int amt) {
	m_carriedFood += amt;
	return;
}

// Sets carry capacity
void EnergyHolder::setCarryCapacity(int cap) {
	m_carryCapacity = cap;
	return;
}


// Does this actor become food wdhen it dies?
bool EnergyHolder::becomesFoodUponDeath() const { return false; }
// Only insects become food upon death

// returns the colony to which this EnergyHolder belongs
int EnergyHolder::getColony() const { return m_colony; }
// Food is assigned a colony of -10
// Grasshoppers are assign a colony of -1. They will always be an enemy

// returns the compiler associated with this Energy Holder
Compiler* EnergyHolder::getCompiler() const { return m_compiler; }






// FOOD FUNCTIONS
Food::Food(StudentWorld* sw, int startX, int startY, int energy)
	: EnergyHolder(sw, startX, startY, right, energy, IID_FOOD, 2, -10, nullptr) {}

Food::~Food() {}

void Food::doSomething() {}

bool Food::isEdible() const { return true; }


// ANTHILL FUNCTIONS


AntHill::AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program)
	: EnergyHolder(sw, startX, startY, right, 8999, IID_ANT_HILL, 2, colony, program) {}

AntHill::~AntHill() {}

void AntHill::doSomething() {
	// decrease hp by 1
	updateEnergy(-1);

	// check if dead, return if dead
	if (isDead())
		return;
	
	// attempt to eat 10000 units of food
	pickupAndEatFood(10000);

	// checkif i have atleast 2000 hp
	// if yes, add ant in same square
	// return hp by 1500
	// Ask student world to increase count of total number of ants
	if (getEnergy() >= 2000) {
		// create ant and adjust hp
		Ant* newAnt = new Ant(getWorld(), getX(), getY(), getColony(), getCompiler(), getProperAntID(getColony()));
		getWorld()->addActor(newAnt);
		updateEnergy(-1500);
		// adjust score in studentworld
		getWorld()->increaseScore(getColony());
	}	
}

bool AntHill::isAntHill(int colony) const { return colony == getColony(); }



int AntHill::getProperAntID(int colony) {
	switch (colony) {
	case 0:
		return IID_ANT_TYPE0;
		break;
	case 1:
		return IID_ANT_TYPE2;
		break;
	case 2:
		return IID_ANT_TYPE2;
		break;
	case 3:
		return IID_ANT_TYPE3;
		break;
	}
	return -1; // error
}
// PHEROMONE FUNCTIONS

Pheromone::Pheromone(StudentWorld* sw, int startX, int startY, int colony)
	: EnergyHolder(sw, startX, startY, right, 256, getProperIID(colony), 2, colony, nullptr) {}

Pheromone::~Pheromone() {}

void Pheromone::doSomething() { updateEnergy(-1); }

int Pheromone::getProperIID(int colony) {
	switch (colony) {
	case 0:
		return IID_PHEROMONE_TYPE0;
		break;
	case 1:
		return IID_PHEROMONE_TYPE1;
		break;
	case 2:
		return IID_PHEROMONE_TYPE2;
		break;
	case 3:
		return IID_PHEROMONE_TYPE3;
		break;
	}
	return -1; // error
}

bool Pheromone::isPheromone(int colony) const { return colony == getColony(); }

// Increase the strength (i.e., energy) of this pheromone.
void Pheromone::increaseStrength() { 

	int pherStrength = getEnergy();

	// use pherStrength to find out how much strength to add to pheromone
	// limit increment so that pherStrength is always less than or equal to 768
	int pherStrengthIncrease = pherStrength >= 512 ? 768 - pherStrength : 256;
	updateEnergy(pherStrengthIncrease);
	return;
}


///////// TRIGGERABLE ACTOR CLASS FUNCTIONS

TriggerableActor::TriggerableActor(StudentWorld* sw, int x, int y, int imageID)
	: Actor(sw, x, y, right, imageID, 2) {}

TriggerableActor::~TriggerableActor() {}

bool TriggerableActor::isDangerous(int colony) const { return true; }



///////// WATERPOOL ACTOR CLASS FUNCTIONS

WaterPool::WaterPool(StudentWorld* sw, int x, int y)
	: TriggerableActor(sw, x, y, IID_WATER_POOL) {}

WaterPool::~WaterPool() {}

void WaterPool::doSomething() {
	getWorld()->stunAllStunnableAt(getX(), getY());
}


///////// POISON ACTOR CLASS FUNCTIONS

Poison::Poison(StudentWorld* sw, int x, int y)
	: TriggerableActor(sw, x, y, IID_POISON) {}

Poison::~Poison() {}

void Poison::doSomething() {
	getWorld()->poisonAllPoisonableAt(getX(), getY());
}

///////// INSECT ACTOR CLASS FUNCTIONS


Insect::Insect(StudentWorld* world, int startX, int startY, int energy, int imageID, int colony, Compiler* comp)
	: EnergyHolder(world, startX, startY, right, energy, imageID, 1, colony, comp),
	m_sleepTicks(0), m_lastStunY(0), m_lastStunX(0)
{
	setDirection(genRandomDir());
};

Insect::~Insect() {}

void Insect::getBitten(int amt) {
	updateEnergy(-amt);
	return;
}

void Insect::getPoisoned() {
	// checks if the Poison tile is the same pool that poisoned it last tick
	if (getX() != m_lastStunX &&  getY() != m_lastStunY) {
		updateEnergy(-150);
		m_lastStunX = getX();
		m_lastStunY = getY();
	}
	return;
}

void Insect::resetLastStunned() {
	m_lastStunY = -1;
	m_lastStunX = -1;
}

void Insect::getStunned() {
	// checks if its current stunner waterPool is the same pool that stunned it last tick
	if (getX() != m_lastStunX &&  getY() != m_lastStunY) {
		m_sleepTicks += 2;
		m_lastStunX = getX();
		m_lastStunY = getY();
	}

	return;
}

bool Insect::isEnemy(int colony) const { return true; }
// all insects are enemies besides allied ants

bool Insect::isDangerous(int colony) const { return true; }
// all insects are dangerous except allied ants 

bool Insect::becomesFoodUponDeath() const { return true; }

// Set x,y to the coordinates of the spot one step in front of this insect.
void Insect::getXYInFrontOfMe(int& x, int& y) const {
	Direction curDir = getDirection();
	x = getX();
	y = getY();
	switch (curDir) {
	case right:
		x++;
		break;
	case left:
		x--;
		break;
	case up:
		y++;
		break;
	case down:
		y--;
		break;
	}
	return;
}

// Move this insect one step forward if possible, and return true;
// otherwise, return false without moving.
bool Insect::moveForwardIfPossible() {

	int x, y;
	getXYInFrontOfMe(x, y);

	if (getWorld()->canMoveTo(x, y)) {
		moveTo(x, y);

		// If an insect has moved, it means it can be stunned again
		resetLastStunned();

		return true;
	}
	return false;
}

// Increase the number of ticks this insect will sleep by the indicated amount.
void Insect::increaseSleepTicks(int amt) {
	m_sleepTicks += amt;
	return;
}

int Insect::getRemainingSleepTicks() const { return m_sleepTicks; }

bool Insect::doIDoSomethingThisTick() {
	// get hungrier
	updateEnergy(-1);

	// check if hp <= 0 and if so make it dead
	if (isDead()) {
		//addDeathFoodHere();
		// more convenient to let student world add food
		return false;
	}

	// If sleeping or stunned, decrement ticks
	if (getRemainingSleepTicks() > 0) {
		m_sleepTicks--;
		return false;
	}

	return true;
}

////// ANT FUNCTIONS

Ant::Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID)
	: Insect(sw, startX, startY, 1500, imageID, colony, program), m_ic(0), m_blocked(false), m_bitten(false)
{
	setCarryCapacity(1800);
}

Ant::~Ant() {}

void Ant::doSomething() {

	// return if dead or sleeping
	if (!doIDoSomethingThisTick())
		return;
	if (!commandInterpreter())
		updateEnergy(-10000000);//kill ant;
}

void Ant::getBitten(int amt){
	updateEnergy(-amt);
	m_bitten = true;
}

bool Ant::isEnemy(int colony) const { return colony != getColony(); }

bool Ant::moveForwardIfPossible() {

	int x, y;
	getXYInFrontOfMe(x, y);

	if (getWorld()->canMoveTo(x, y)) {
		moveTo(x, y);

		// If an insect has moved, it means it can be stunned again
		resetLastStunned();
		m_bitten = false;
		m_blocked = false;
		return true;
	}
	m_blocked = true;
	return false;
}


bool Ant::isStunnable() const { return true; }

bool Ant::isPoisonable() const { return true; }

void Ant::emitPheromone() {
	// if pheromone already exists here add 256 units // capped at 768
	Actor* pheromoneHere = getWorld()->getPheromoneAt(getX(), getY(), getColony());
	if (pheromoneHere != nullptr) {
		dynamic_cast<Pheromone*>(pheromoneHere)->increaseStrength();
	}

	// or else just create new pheromone
	Actor* newPheromone = new Pheromone(getWorld(), getX(), getY(), getColony());
	getWorld()->addActor(newPheromone);
	return;
}

bool Ant::commandInterpreter() {

	Compiler::Command cmd;

	// limit to 10 non simulation modifying commands
	for (int ithCommand = 0; ithCommand < 10; ithCommand++) {

		// get the command from element ic of the vector
		if (!getCompiler()->getCommand(m_ic, cmd)) {
			updateEnergy(-1000000); // KILL THIS ANT
			return false; // error - no such instruction!
		}

		switch (cmd.opcode)
		{
		case Compiler::moveForward: {
			// cause the ant to move forward by
			// updating its x,y coordinates
			++m_ic; // advance to next instruction
			moveForwardIfPossible();
			return true;	// return since simulation state  could be altered
			
			break;
		}
		case Compiler::eatFood:
		{

			int foodConsumed = getCarriedFood();
			foodConsumed = foodConsumed > 100 ? 100 : foodConsumed; // cap food consumption at 100
			updateEnergy(foodConsumed);	// increase ants hp
			updateCarriedFood(-foodConsumed);	 // decrease current food carried
			++m_ic;

			return true;	// return since simulation state is altered
			break;
		}
		case Compiler::dropFood: {
			++m_ic;
			// drops all carried food onto the tile on which this ant is standing
			if (getCarriedFood() >0) {
				addFood(getCarriedFood());
				updateCarriedFood(-getCarriedFood());
			}
			return true;	// return since simulation state is altered
			break;
		}
		case Compiler::bite: {
			++m_ic;
			// if enemy is present in my tile, attmpt to bite it
			if (getWorld()->isEnemyAt(getX(), getY(), getColony())) {
				getWorld()->biteEnemyAt(this, getColony(), 15); // bite for 15 damage
				return true; 	// return since simulation state is altered
			}
			break;
		}
		case Compiler::pickupFood: {
			++m_ic;
			// attempt to pick up a food item at current tile
			pickupFood(400);
			return true; 	// return since simulation state could be altered
			break;
		}
		case Compiler::emitPheromone: {
			++m_ic;
			emitPheromone();
			return true;
			break;
		}
		case Compiler::faceRandomDirection: {
			++m_ic;
			setDirection(genRandomDir());
			return true;
			break;
		}
		case Compiler::generateRandomNumber: {
			++m_ic;
			m_lastRandNum = randInt(0, stoi(cmd.operand1));
			break;
		}
		case Compiler::goto_command: {
			m_ic = stoi(cmd.operand1);
			break;
		}
		case Compiler::rotateClockwise: {
			++m_ic;
			Direction curDir = getDirection();
			switch (curDir) {
			case right:
				curDir = down; break;
			case down:
				curDir = left; break;
			case left:
				curDir = up; break;
			case up:
				curDir = right; break;
			}
			setDirection(curDir);
			return true;
			break;
		}
		case Compiler::rotateCounterClockwise: {
			++m_ic;
			Direction curDir = getDirection();
			switch (curDir) {
			case right:
				curDir = up; break;
			case down:
				curDir = right; break;
			case left:
				curDir = down; break;
			case up:
				curDir = left; break;
			}
			setDirection(curDir);
			return true;
			break;
		}
		case Compiler::if_command:
		{
			// if condition is false, increment instruction counter
			if(!ifCommandHandler(cmd))
				++m_ic;
			break;
		}
		}
	}
	return true;

}

bool Ant::ifCommandHandler(const Compiler::Command& cmd) {
	// m_ic still refers to if command
	switch (stoi(cmd.operand1))
	{
	case 0: {
		//0: i_smell_danger_in_front_of_me
		int xfront, yfront;
		getXYInFrontOfMe(xfront, yfront);
		if (getWorld()->isDangerAt(xfront, yfront, getColony())) {
			m_ic = stoi(cmd.operand2);
			return true;
		}
		break;
	}
	case 1: {
		//1 : i_smell_pheromone_in_front_of_me
		int xfront, yfront;
		getXYInFrontOfMe(xfront, yfront);

		Actor* potentialPher = getWorld()->getPheromoneAt(xfront, yfront, getColony());
		if (potentialPher != nullptr)
			if (potentialPher->isPheromone(getColony()))
				if (dynamic_cast<Pheromone*>(potentialPher)->getEnergy() > 0) {
					m_ic = stoi(cmd.operand2);
					return true;
				}
		break;

	}
	case 2: {
		// if i was bit
		if(m_bitten) {
			m_ic = stoi(cmd.operand2);
			return true;
		}
		break;
	}
	case 3: {
		// if i am carrying food
		if(getCarriedFood()>0) {
			m_ic = stoi(cmd.operand2);
			return true;
		}
		break;
	}
	case 4: {
		// if hungry
		if (getEnergy() <= 25) {
			m_ic = stoi(cmd.operand2);
			return true;
		}
		break;
	}
	case 5: {
		// standing on anthill
		if(getWorld()->isAntHillAt(getX(),getY(),getColony())) {
			m_ic = stoi(cmd.operand2);
			return true;
		}
		break;
	}
	case 6: {
		// if standing on food
		Actor* potentialFood = getWorld()->getEdibleAt(getX(), getY());
		if (potentialFood != nullptr) {
			m_ic = stoi(cmd.operand2);
			return true;
		}
		break;

	}
	case 7: {
		// if standing with enemy
		if (getWorld()->isEnemyAt(getX(), getY(), getColony())) {
			m_ic = stoi(cmd.operand2);
			return true;
		}
		break;
	}
	case 8: {
		// if blocked from moving
		if(m_blocked) {
			m_ic = stoi(cmd.operand2);
			return true;
		}
		break;
	}
	case 9:
		//9 : last_random_number_was_zero
		if (m_lastRandNum == 0) {
			m_ic = stoi(cmd.operand2);
			return true;
		}
		break;
	}

	return false;
}

///////// GRASSHOPPER ACTOR FUNCTIONS


Grasshopper::Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID)
	: Insect(sw, startX, startY, energy, imageID, -1, nullptr), m_distanceLeft(randInt(2, 10))
{
	genNewDistanceGoal();
}

Grasshopper::~Grasshopper() {}


// Makes grasshopper attempt to eat 200 units of food
// gives it 50% chance of sleeping if it does eat
// returns true if grasshopper wants to sleep
bool Grasshopper::attemptToEatHere() {
	// attempt to eat food on current square
	if (pickupAndEatFood(200)) {
		//50% chance sleeping if succesful at eating
		if (randInt(0, 1)) {
			increaseSleepTicks(2);
			return true;
		}
	}
	return false;
}

// Makes grasshopper check its distance goal in the direction it wants to travel
// is it still has to walk, make it walk one tile
// otherwise, set a new movement goal
void Grasshopper::makeGrassHopperMove() {
	// if finished moving
	if (getDistanceGoal() <= 0) {
		// pick new random direction
		setDirection(genRandomDir());
		// make new distance goal
		genNewDistanceGoal();
	}

	if (moveForwardIfPossible())
		adjustDistanceGoal(-1);
	else
		adjustDistanceGoal(-getDistanceGoal());
}


void Grasshopper::genNewDistanceGoal() { m_distanceLeft = randInt(2, 10); } // sets distanceLeft to random number from 2 to 10

void Grasshopper::adjustDistanceGoal(int num) { m_distanceLeft += num; }

int Grasshopper::getDistanceGoal() const { return m_distanceLeft; }




//////// BABY GRASSHOPPER ACTOR FUNCTIONS


BabyGrasshopper::BabyGrasshopper(StudentWorld* sw, int startX, int startY)
	: Grasshopper(sw, startX, startY, 500, IID_BABY_GRASSHOPPER)
{}

BabyGrasshopper::~BabyGrasshopper() {}

void BabyGrasshopper::doSomething() {

	// return if dead or sleeping
	if (!doIDoSomethingThisTick())
		return;

	// If hp >= 1600 become adult
	if (getEnergy() >= 1600) {
		AdultGrasshopper* newAdultG = new AdultGrasshopper(getWorld(), getX(), getY());
		getWorld()->addActor(newAdultG);
		updateEnergy(-1000000); // same as killing the baby. rip baby
								// adding food is done in student world
								// addDeathFoodHere();
	}

	if (attemptToEatHere())
		return;		

	makeGrassHopperMove();
	increaseSleepTicks(2);
}

bool BabyGrasshopper::isStunnable() const { return true; }

bool BabyGrasshopper::isPoisonable() const { return true; }

//// ADULT GRASSHOPPER FUNCTIONS
AdultGrasshopper::AdultGrasshopper(StudentWorld* sw, int startX, int startY)
	: Grasshopper(sw, startX, startY, 1600, IID_ADULT_GRASSHOPPER) {}

AdultGrasshopper::~AdultGrasshopper() {}

void AdultGrasshopper::doSomething() {

	// return if dead or sleeping
	if (!doIDoSomethingThisTick())
		return;
	
	// attempt to bite enemies on current sqaure
	// 1/3 chance to bite a RANDOM bitable enemy, doing 50 hp of damage
	if (getWorld()->isEnemyAt(getX(), getY(), getColony())) {
		if (!randInt(0, 2))
			if (getWorld()->biteEnemyAt(this, getColony(), 50)) {
				increaseSleepTicks(2);
				// proceed to step 13 aka sleep after successful bite
				return;
			}
	}
	
	// One in ten chance to jump within 10 squares radius
	if (!randInt(0, 9)) {
		int newx = 100;
		int newy = 100;

		// generate random tile within 10 units
		while (sqrt(((newx - getX()) ^ 2) + ((newy - getY()) ^ 2)) > 10) {
			newx = randInt(getX() - 10, getX() + 10);
			newy = randInt(getY() - 10, getY() + 10);
		}

		// check if tile can be jumped to
		if (getWorld()->canMoveTo(newx, newy)
			&& newx >= 0 && newx <= 63 && newy >= 0 && newy <= 63) {
			moveTo(newx, newy);
			// jump!
		}
		// sleep
		increaseSleepTicks(2);
		return;
	}
	
	attemptToEatHere();
	makeGrassHopperMove();
	increaseSleepTicks(2);
}


void AdultGrasshopper::getBitten(int amt) {
	updateEnergy(-amt);
	
	// 50% chace to bite back if still alive
	if (!randInt(0, 1) && !isDead())
		getWorld()->biteEnemyAt(this, getColony(), 50);
}



//////// AUXILLIARY FUNCTIONS
