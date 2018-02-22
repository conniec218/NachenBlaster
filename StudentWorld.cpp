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
		Star* s = new Star(randInt(0, VIEW_WIDTH-1), randInt(0, VIEW_HEIGHT-1), this);
		m_actorList.push_back(s);
	}
    return GWSTATUS_CONTINUE_GAME;
}

//call doSomething on everything
int StudentWorld::move()
{
	for (list<Actor*>::iterator it = m_actorList.begin(); it != m_actorList.end();) {
		(*(*it)).doSomething();
		if ((*(*it)).getX() < 0) {  //isAlive
			Actor* st = *it;
			it = m_actorList.erase(it);
			delete st;
			//cerr << "erase" << endl;
		}
		else
			it++;
	}
	if (randInt(1, 15) == 8)
		m_actorList.push_back(new Star(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
	//Is this right? (above)
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
		actor = nullptr;
	}
}
