#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <list>
#include <string>

const int COLLISION_WITH_ALIEN = 1;
const int COLLISION_WITH_PROJECTILE = 2;
const int COLLISION_WITH_PLAYER = 3;
const int COLLISION_WITH_TORPEDO = 4;
const int NO_COLLISION = -1;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Star;
class NachenBlaster;
class Actor;
class Alien;
class Projectile;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
	~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	void addActorToList(Actor* a);
	bool createAlien();
	Alien* createNewAlien();
	bool playerInLineOfFire(const Actor* a);
	int checkForCollisions(Alien* a);
	int checkForCollisions(Projectile* p);

private:
	NachenBlaster* nachenblaster;
	std::list<Actor*> m_actorList;
};

#endif // STUDENTWORLD_H_
