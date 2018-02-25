#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <list>
#include <string>

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

private:
	NachenBlaster* nachenblaster;
	std::list<Actor*> m_actorList;
};

#endif // STUDENTWORLD_H_
