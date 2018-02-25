#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
}

StudentWorld::~StudentWorld() {
	cleanUp();
}

int StudentWorld::init()
{
	nachenblaster = new NachenBlaster(this);
	for (int i = 0; i < 30; i++) {
		addActorToList(new Star(randInt(0, VIEW_WIDTH-1), randInt(0, VIEW_HEIGHT-1), this));
	}

	//create a new alien! dummy function
	if (createAlien())
		addActorToList(createNewAlien());

    return GWSTATUS_CONTINUE_GAME;
}

//call doSomething on everything
int StudentWorld::move()
{


	for (list<Actor*>::iterator it = m_actorList.begin(); it != m_actorList.end();) {
		if ((*(*it)).isAlive()) {
			(*(*it)).doSomething();
			it++;
		}
		else {
			Actor* st = *it;
			it = m_actorList.erase(it);
			delete st;
		}
		/*if ((*(*it)).getX() < 0) {  //isAlive
			Actor* st = *it;
			it = m_actorList.erase(it);
			delete st;
			//cerr << "erase" << endl;
		}
		else
			it++;*/
	}
	if (randInt(1, 15) == 8)
		addActorToList(new Star(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));

	nachenblaster->doSomething();
	//Determines if the object is dead
	
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //decLives();
    //return GWSTATUS_PLAYER_DIED;
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete nachenblaster;
	nachenblaster = nullptr;
	for (list<Actor*>::iterator it = m_actorList.begin(); it != m_actorList.end();) {
		Actor* actor = *it;
		it = m_actorList.erase(it);
		delete actor;
	}
}

void StudentWorld::addActorToList(Actor* a) {
	m_actorList.push_back(a);
}
//Dummy function
bool StudentWorld::createAlien() {
	return true;
}

//Dummy function
Alien* StudentWorld::createNewAlien() {
	int i = randInt(1, 3);
	switch (i) {
	case 1: 
		return new Smallgon(this);
		break;
	case 2: 
		return new Smoregon(this);
		break;
	case 3: 
		return new Snagglegon(this);
		break;
	}
	return new Smallgon(this);
}

