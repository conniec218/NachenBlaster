#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <iostream>
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
	case 2: 
		return new Smoregon(this);
	case 3: 
		return new Snagglegon(this);
	}
	return new Smallgon(this);
}

bool StudentWorld::playerInLineOfFire(const Actor* a) {
	if ((nachenblaster->getX() < a->getX()) && abs(a->getY() - nachenblaster->getY()) <= 4) {
		cout << "PLF True" << endl;
		return true;
	}
	cout << "PLF False" << endl;
	return false;
}

int StudentWorld::checkForCollisions(Actor* a) {
	int distancex, distancey;
	double distance;
	distancex = ((nachenblaster)->getX() - a->getX()) * ((nachenblaster)->getX() - a->getX());
	distancey = ((nachenblaster)->getY() - a->getY()) * ((nachenblaster)->getY() - a->getY());
	distance = sqrt(distancex - distancey);

	if (distance < .75 * (a->getRadius() + (nachenblaster)->getRadius())) {
		if(a->isAlien())
			static_cast<Alien*>(a)->sufferDamage(COLLISION_WITH_PLAYER, nachenblaster);
	}
	for (list<Actor*>::iterator it = m_actorList.begin(); it != m_actorList.end(); it++) {
		//cout << !(*it)->isStar() << endl;
		if((*it)->isAlive() && (*it) != a && !(*it)->isStar()){
				cout << "Valid actor" << endl;
				distancex = ((*it)->getX() - a->getX()) * ((*it)->getX() - a->getX());
				distancey = ((*it)->getY() - a->getY()) * ((*it)->getY() - a->getY());
				distance = sqrt(distancex - distancey);
				
				if (distance < .75 * (a->getRadius() + (*it)->getRadius())) {
					cout << "Collision occurred" << endl;
					if (a->isAlien()) {
						;
					}
					//static_cast<Alien*>(*it)->sufferDamage();  //call suffer damage for both!
					else if ((*it) == nachenblaster)
						;//call suffer damage for both
					else if ((*it)->isProjectile())
						;//call suffer damage for both! may have to differentiate between shot by alien and shot by nachenblaster
			}
		}
	}
	return NO_COLLISION;
}
