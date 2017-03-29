#include "StudentWorld.h"
#include "Field.h" 
#include "GraphObject.h"
#include "Actor.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir), ticksRemain(2000)
{
	for (int i = 0; i < 4; i++) {
		m_colonyScores[i] = 0;
		m_lastIncrement[i] = 2000;
	}
}

StudentWorld::~StudentWorld() {
	cleanUp();
}

int StudentWorld::init() {
	if (loadBugsFiles() == GWSTATUS_LEVEL_ERROR)
		return GWSTATUS_LEVEL_ERROR;
	if (!loadFieldFile())
		return GWSTATUS_LEVEL_ERROR;
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {	
	// decrease tick count
	ticksRemain--;

	makeActorsDoSomething();

	removeDeadActors();

	// Set the game text above the screen
	updateGameText();

	if (ticksRemain)
		return GWSTATUS_CONTINUE_GAME;
	else {
		// check for a winner
		int maxAnts = 5;
		int maxTicks = -1;
		int winningAnt = -1;
		for (int i = 0; i < 4; i++) {
			if (m_colonyScores[i] > maxAnts && m_lastIncrement[i] > maxTicks) {
				winningAnt = i;
				maxAnts = m_colonyScores[i];
				maxTicks = m_lastIncrement[i];
			}

		}
		if (winningAnt != -1) {
			setWinner(m_compilers[winningAnt]->getColonyName());
			return GWSTATUS_PLAYER_WON;
		}

		return GWSTATUS_NO_WINNER;
	}
}

void StudentWorld::cleanUp() {
	std::unordered_map<int, std::list<Actor*>>::iterator mapIt = m_fieldActors.begin();

	// navigates through unordered map and deletes all actors
	while (mapIt != m_fieldActors.end()) {
		std::list<Actor*>* actorList = &(mapIt->second);
		std::list<Actor*>::iterator currentActor = actorList->begin();
		while (currentActor != actorList->end()) {
			if (*currentActor != nullptr) {
				delete *currentActor;
				*currentActor = nullptr;
			}
			currentActor++;
		}
		mapIt++;
	}

	// delet compilers

	std::vector<Compiler*>::iterator compIt = m_compilers.begin();
	while (compIt != m_compilers.end()) {
		if (*compIt != nullptr) {
			delete *compIt;
			*compIt = nullptr;
		}
		compIt++;
	}

}



void StudentWorld::updateGameText() {

	std::string statText = "Ticks: "; 
	int spacesToAdd = ticksRemain;
	while (spacesToAdd < 1000 && spacesToAdd) {
		statText += " ";
		spacesToAdd = spacesToAdd * 10;
	}
	if (!spacesToAdd)
		statText += "   ";

	statText += std::to_string(ticksRemain) + " -";

	// add score information
	for (unsigned int i = 0; i < m_fileNames.size(); i++) {
		statText = statText + "  " + m_compilers[i]->getColonyName() + ": ";
		if (m_colonyScores[i] < 10)
			statText += "0" + std::to_string(m_colonyScores[i]);
		else
			statText += std::to_string(m_colonyScores[i]);

	}

	setGameStatText(statText);
}

int StudentWorld::loadBugsFiles() {

	// pre load compilers for ant hill

	// get the names of all of the ant program source files using provided function
	m_fileNames = getFilenamesOfAntPrograms();
	std::string errorc = "Error compiling bugs source code";

	for (unsigned int i = 0; i < m_fileNames.size(); i++) {
		m_compilers.push_back(new Compiler);
		if (!m_compilers[i]->compile(m_fileNames[i], errorc)) {
			// error!
			setError(m_fileNames[i] + " " + errorc);
			return GWSTATUS_LEVEL_ERROR;
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::loadFieldFile()
{
	Field f;
	string fieldFile = getFieldFilename();
	string error;
	if (f.loadField(fieldFile, error) != Field::LoadResult::load_success)
	{
		setError(fieldFile + " " + error);
		return false; // something bad happened!
	}
	// otherwise the load was successful and you can access the
	// contents of the field 

	// Load actors from Field file and add them to the simulation world
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 64; y++) {
			Field::FieldItem item = f.getContentsOf(x, y);
			Actor* newActor = nullptr;

			switch (item) {
			case Field::FieldItem::rock:
				newActor = new Pebble(this, x, y);
				break;
			case Field::FieldItem::food:
				newActor = new Food(this, x, y, 6000);
				break;
			case Field::FieldItem::anthill0:
				if (m_fileNames.size() > 0)
					newActor = new AntHill(this, x, y, 0, m_compilers[0]);
				break;
			case Field::FieldItem::anthill1:
				if (m_fileNames.size() > 1)
					newActor = new AntHill(this, x, y, 1, m_compilers[1]);
				break;
			case Field::FieldItem::anthill2:
				if (m_fileNames.size() > 2)
					newActor = new AntHill(this, x, y, 2, m_compilers[2]);
				break;
			case Field::FieldItem::anthill3:
				if (m_fileNames.size() > 3)
					newActor = new AntHill(this, x, y, 3, m_compilers[3]);
				break;
			case Field::FieldItem::poison:
				newActor = new Poison(this, x, y);
				break;
			case Field::FieldItem::water:
				newActor = new WaterPool(this, x, y);
				break; 
			case Field::FieldItem::grasshopper:
				newActor = new BabyGrasshopper(this, x, y);
				break;
			}
	
			if (newActor != nullptr) {
				m_fieldActors[64 * x + y].push_back(newActor);
				newActor->setVisible(true);
			}
		}
	}

	
	return true;
}

void StudentWorld::makeActorsDoSomething() {

	unordered_map<int, list<Actor*>>::iterator mapIt = m_fieldActors.begin();
	list<Actor*>* actorList;
	list<Actor*>::iterator currentActor;
	int prevx, prevy, newx, newy;

	while (mapIt != m_fieldActors.end()) {
		actorList = &(mapIt->second);
		currentActor = actorList->begin();
		while (currentActor != actorList->end()) {
			prevx = (*currentActor)->getX();
			prevy = (*currentActor)->getY();

			(*currentActor)->doSomething();

			newx = (*currentActor)->getX();
			newy = (*currentActor)->getY();

			//check if actor moved
			//and move it to a the new cell if it did
			if (prevx != newx || prevy != newy) {
				m_fieldActors[newx * 64 + newy].push_back(*currentActor);
				currentActor = m_fieldActors[prevx * 64 + prevy].erase(currentActor);
			}
			if (currentActor != actorList->end())
				currentActor++;
		}
		if (mapIt->second.empty())
			mapIt = m_fieldActors.erase(mapIt);
		if (mapIt != m_fieldActors.end())
			mapIt++;
	}
}

void StudentWorld::removeDeadActors() {
	unordered_map<int, list<Actor*>>::iterator mapIt = m_fieldActors.begin();

	while (mapIt != m_fieldActors.end()) {
		list<Actor*>* actorList = &(mapIt->second);
		list<Actor*>::iterator currentActor = actorList->begin();
		while (currentActor != actorList->end()) {
			if ((*currentActor)->isDead()) {
				(*currentActor)->setVisible(false);

				if ((*currentActor)->becomesFoodUponDeath())
					dynamic_cast<EnergyHolder*>(*currentActor)->addFood(100);

				delete *currentActor;
				*currentActor = nullptr;
				currentActor = actorList->erase(currentActor);
			}
			if (currentActor != actorList->end())
				currentActor++;
		}
		mapIt++;
	}
}

// Can an insect move to x,y?
bool StudentWorld::canMoveTo(int x, int y) const {
	
	unordered_map<int, list<Actor*>>::const_iterator mapIt = m_fieldActors.find(x * 64 + y);
	if (mapIt == m_fieldActors.end())
		return true;

	list<Actor*>::const_iterator actorHere = mapIt->second.begin();

	// check if any actor if this tile blocks movement
	while (actorHere != mapIt->second.end()) {
		if ((*actorHere) != nullptr)
			if ((*actorHere)->blocksMovement())
				return false;
		actorHere++;
	}

	return true;
}


// Add an actor to the world
void StudentWorld::addActor(Actor* a) {
	unordered_map<int, list<Actor*>>::iterator mapIt = m_fieldActors.find(a->getX() * 64 + a->getY());
	mapIt->second.push_back(a);
	a->setVisible(true);
	return;
}

// If an item that can be picked up to be eaten is at x,y, return a
// pointer to it; otherwise, return a null pointer.  (Edible items are
// only ever going be food.)
Actor* StudentWorld::getEdibleAt(int x, int y) const {

	unordered_map<int, list<Actor*>>::const_iterator mapIt = m_fieldActors.find(x * 64 + y);
	if (mapIt == m_fieldActors.end())
		return nullptr;

	list<Actor*>::const_iterator actorHere = mapIt->second.begin();

	// check if any actor in this tile is Edible
	while (actorHere != mapIt->second.end()) {
		if ((*actorHere) != nullptr)
			if ((*actorHere)->isEdible())
				return (*actorHere);
		actorHere++;
	}

	return nullptr;
}


// If a pheromone of the indicated colony is at x,y, return a pointer
// to it; otherwise, return a null pointer.
Actor* StudentWorld::getPheromoneAt(int x, int y, int colony) const {
	unordered_map<int, list<Actor*>>::const_iterator mapIt = m_fieldActors.find(x * 64 + y);
	if (mapIt == m_fieldActors.end())
		return nullptr;

	list<Actor*>::const_iterator actorHere = mapIt->second.begin();

	// check if any actor in this is tile is a pheromone belonging to my colony
	while (actorHere != mapIt->second.end()) {
		if ((*actorHere) != nullptr)
			if ((*actorHere)->isPheromone(colony))
				return (*actorHere);
		actorHere++;
	}

	return nullptr;
}


// Is an enemy of an ant of the indicated colony at x,y?
bool StudentWorld::isEnemyAt(int x, int y, int colony) const {
	unordered_map<int, list<Actor*>>::const_iterator mapIt = m_fieldActors.find(x * 64 + y);
	if (mapIt == m_fieldActors.end())
		return false;

	list<Actor*>::const_iterator actorHere = mapIt->second.begin();

	// check if any actor in this tile is an enemy
	while (actorHere != mapIt->second.end()) {
		if ((*actorHere) != nullptr)
			if ((*actorHere)->isEnemy(colony))
				return true;
		actorHere++;
	}

	return false;
}


// Is something dangerous to an ant of the indicated colony at x,y?
bool StudentWorld::isDangerAt(int x, int y, int colony) const {
	unordered_map<int, list<Actor*>>::const_iterator mapIt = m_fieldActors.find(x * 64 + y);
	if (mapIt == m_fieldActors.end())
		return false;

	list<Actor*>::const_iterator actorHere = mapIt->second.begin();

	// check if any actor in this is Dangerous
	while (actorHere != mapIt->second.end()) {
		if ((*actorHere) != nullptr)
			if ((*actorHere)->isDangerous(colony))
				return true;
		actorHere++;
	}

	return false;
}

// Is the anthill of the indicated colony at x,y?
bool StudentWorld::isAntHillAt(int x, int y, int colony) const {
	unordered_map<int, list<Actor*>>::const_iterator mapIt = m_fieldActors.find(x * 64 + y);
	if (mapIt == m_fieldActors.end())
		return false;

	list<Actor*>::const_iterator actorHere = mapIt->second.begin();

	// check if any actor in this is Edible
	while (actorHere != mapIt->second.end()) {
		if ((*actorHere) != nullptr)
			if ((*actorHere)->isAntHill(colony))
				return true;
		actorHere++;
	}

	return false;
}



// Bite an enemy of an ant of the indicated colony at me's location
// (other than me; insects don't bite themselves).  Return true if an
// enemy was bitten.
bool StudentWorld::biteEnemyAt(Actor* me, int colony, int biteDamage) {
	int x = me->getX();
	int y = me->getY();

	unordered_map<int, list<Actor*>>::const_iterator mapIt = m_fieldActors.find(x * 64 + y);
	if (mapIt == m_fieldActors.end())
		return false;

	list<Actor*>::const_iterator actorHere = mapIt->second.begin();

	// count number of bitable enemies
	// choose random enemy to bite
	int numOfBitables = 0;
	while (actorHere != mapIt->second.end()) {
		if ((*actorHere) != nullptr)
			if ((*actorHere)->isEnemy(colony) && *actorHere != me)
				numOfBitables++;
		actorHere++;
	}

	// generate random number to determine which actor to bite
	int actorToBite = randInt(0, numOfBitables);
	
	actorHere = mapIt->second.begin();

	// bite chosen enemy
	numOfBitables = 0;
	while (actorHere != mapIt->second.end()) {
		if ((*actorHere) != nullptr)
			if ((*actorHere)->isEnemy(colony) && *actorHere != me) {
				if (numOfBitables == actorToBite) {
					//cout << " A BITINg OCCURED at " << x << " " << y << endl;
					//cout << " startHP: " << dynamic_cast<EnergyHolder*>((*actorHere))->getEnergy() << endl;
									
					(*actorHere)->getBitten(biteDamage);
					
					//cout << " endHP: " << dynamic_cast<EnergyHolder*>((*actorHere))->getEnergy() << endl;
					
					return true;
				}
				numOfBitables++;
			}
		actorHere++;
	}

	return false;
}

// Poison all poisonable actors at x,y.
bool StudentWorld::poisonAllPoisonableAt(int x, int y) {
	unordered_map<int, list<Actor*>>::const_iterator mapIt = m_fieldActors.find(x * 64 + y);
	if (mapIt == m_fieldActors.end())
		return false;

	list<Actor*>::const_iterator actorHere = mapIt->second.begin();

	// check if any actor in this tile is poisonable
	while (actorHere != mapIt->second.end()) {
		if ((*actorHere) != nullptr)
			if ((*actorHere)->isPoisonable()) 
				(*actorHere)->getPoisoned();
		actorHere++;
	}

	return false;
}

// Stun all stunnable actors at x,y.
bool StudentWorld::stunAllStunnableAt(int x, int y) {
	unordered_map<int, list<Actor*>>::const_iterator mapIt = m_fieldActors.find(x * 64 + y);
	if (mapIt == m_fieldActors.end())
		return false;

	list<Actor*>::const_iterator actorHere = mapIt->second.begin();

	// check if any actor in this tile is stunnable
	while (actorHere != mapIt->second.end()) {
		if ((*actorHere) != nullptr)
			if ((*actorHere)->isStunnable()) {
				(*actorHere)->getStunned();
				//cout << "RUH ROH someone got stunned at " << (*actorHere)->getX() << " " << (*actorHere)->getY() << endl;
			}
		actorHere++;
	}

	return false;
}

// Record another ant birth for the indicated colony.
void StudentWorld::increaseScore(int colony) {
	m_colonyScores[colony]++;
	m_lastIncrement[colony] = ticksRemain;
}


