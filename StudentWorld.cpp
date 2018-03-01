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
	if (!nachenblaster->isAlive()) {
		cout << "decLives" << endl;
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
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

Alien* StudentWorld::createNewAlien() {
	int s1 = 60;
	int s2 = 20 + getLevel() * 5;
	int s3 = 5 + getLevel() * 10;
	int s = s1 + s2 + s3;
	int value = randInt(1, s);
	if (value <= s1)
		return new Smallgon(5 * (1 + (getLevel() - 1) * .1), this);
	else if (value > s1 && value <= (s1 + s2))
		return new Smoregon(5 * (1 + (getLevel() - 1) * .1), this);
	else
		return new Snagglegon(10 * (1 + (getLevel() - 1) * .1), this);
}

bool StudentWorld::playerInLineOfFire(const Actor* a) {
	if ((nachenblaster->getX() < a->getX()) && abs(a->getY() - nachenblaster->getY()) <= 4) {
		cout << "PLF True" << endl;
		return true;
	}
	cout << "PLF False" << endl;
	return false;
}

void StudentWorld::checkForCollisions(Alien* a) {
	if (!a->isAlive() || !nachenblaster->isAlive())
		return;
	double distancex, distancey;
	double distance;
	distancex = ((nachenblaster)->getX() - a->getX()) * ((nachenblaster)->getX() - a->getX());
	distancey = ((nachenblaster)->getY() - a->getY()) * ((nachenblaster)->getY() - a->getY());
	distance = sqrt(distancex + distancey);
	cout << "player: (" << nachenblaster->getX() << ", " << nachenblaster->getY() << ")" << endl;
	cout << "Alien: (" << a->getX() << ", " << a->getY() << ")" << endl;
	cout << "distance: " << distance << endl;
	if (distance < .75 * (a->getRadius() + (nachenblaster)->getRadius())) {
		cout << "Called sufferDamage on alien due to collision with player" << endl;
		a->sufferDamage(COLLISION_WITH_PLAYER, nachenblaster);
	}
	for (list<Actor*>::iterator it = m_actorList.begin(); it != m_actorList.end(); it++) {
		if((*it)->isAlive() && (*it) != a && !(*it)->isStar()){
				distancex = ((*it)->getX() - a->getX()) * ((*it)->getX() - a->getX());
				distancey = ((*it)->getY() - a->getY()) * ((*it)->getY() - a->getY());
				distance = sqrt(distancex + distancey);
				if (distance < .75 * (a->getRadius() + (*it)->getRadius()))
					//Did it collide with a nachenblaster-fired cabbage/torpedo?
						if((*it)->isProjectile() && !static_cast<Projectile*>(*it)->shotByAlien())
							if (static_cast<Projectile*>(*it)->isTorpedo()) {
								a->sufferDamage(COLLISION_WITH_TORPEDO, *it);
							}
							else {
								a->sufferDamage(COLLISION_WITH_PROJECTILE, *it);
							}
		}
	}
}

//Called by Alien-fired projectiles only
int StudentWorld::checkForCollisions(Projectile* p) {
	if (!p->isAlive() || !nachenblaster->isAlive())
		return NO_COLLISION;
	double distancex, distancey;
	double distance;
	distancex = ((nachenblaster)->getX() - p->getX()) * ((nachenblaster)->getX() - p->getX());
	distancey = ((nachenblaster)->getY() - p->getY()) * ((nachenblaster)->getY() - p->getY());
	distance = sqrt(distancex + distancey);
	if (distance < .75 * (p->getRadius() + (nachenblaster)->getRadius()))
		if (p->isTorpedo()) {
			p->sufferDamage(COLLISION_WITH_TORPEDO, nachenblaster);
			return COLLISION_WITH_TORPEDO;
		}
		else {
			p->sufferDamage(COLLISION_WITH_PROJECTILE, nachenblaster);
			return COLLISION_WITH_PROJECTILE;
		}
	return NO_COLLISION;
}

/*int StudentWorld::checkForCollisions(Projectile* p) {
	int distancex, distancey;
	double distance;
	distancex = ((nachenblaster)->getX() - a->getX()) * ((nachenblaster)->getX() - a->getX());
	distancey = ((nachenblaster)->getY() - a->getY()) * ((nachenblaster)->getY() - a->getY());
	distance = sqrt(distancex - distancey);
	if (distance < .75 * (a->getRadius() + (nachenblaster)->getRadius())) {
		if (a->isAlien())
			static_cast<Alien*>(a)->sufferDamage(COLLISION_WITH_PLAYER, nachenblaster);
		if (a->isProjectile() && static_cast<Projectile*>(a)->shotByAlien())
			if (static_cast)
				static_cast<Projectile*>(a)->sufferDamage(COLLISION_WITH_PLAYER, nachenblaster);
	}
	for (list<Actor*>::iterator it = m_actorList.begin(); it != m_actorList.end(); it++) {
		if ((*it)->isAlive() && (*it) != a && !(*it)->isStar()) {
			distancex = ((*it)->getX() - a->getX()) * ((*it)->getX() - a->getX());
			distancey = ((*it)->getY() - a->getY()) * ((*it)->getY() - a->getY());
			distance = sqrt(distancex - distancey);
			if (distance < .75 * (a->getRadius() + (*it)->getRadius()))
				if (a->isAlien())
					if ((*it)->isProjectile() && !static_cast<Projectile*>(*it)->shotByAlien())
						static_cast<Projectile*>(*it)->sufferDamage(COLLISION_WITH_ALIEN, *it);
		}
	}
	return NO_COLLISION;
}*/
