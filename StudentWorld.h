#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>
#include <sstream>

const int COLLISION_WITH_TORPEDO = 1;
const int COLLISION_WITH_PROJECTILE = 2;
const int COLLISION_WITH_PLAYER = 3;
const int NO_COLLISION = -1;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Star;
class NachenBlaster;
class Actor;
class Alien;
class Projectile;
class Goodie;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
	~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	void addActorToList(Actor* a);
	bool playerInLineOfFire(const Actor* a);
	void checkForCollisions(Alien* a);
	int checkForCollisions(Projectile* p);
	NachenBlaster* checkForCollisions(Goodie * g);
	void killedAnAlien();
	void decAliensOnScreen();
	
private:
	void setInitAlienStats();
	bool createAlien();
	Alien* createNewAlien();
	int aliensKilled() const;
	int maxAliensOnScreen() const;
	int nAliensToAdvance() const;
	int aliensOnScreen() const;
	void addNewAlienOnScreen();

	std::string updateStatusBar();
	NachenBlaster* nachenblaster;
	std::list<Actor*> m_actorList;
	int m_aliensKilled;
	int m_nAliensToAdvance;
	int m_maxAliensOnScreen;
	int m_aliensOnScreen;
};

#endif // STUDENTWORLD_H_
