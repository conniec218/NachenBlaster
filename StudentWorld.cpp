#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
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
	setInitAlienStats();
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
	nachenblaster->doSomething();
	for (list<Actor*>::iterator it = m_actorList.begin(); it != m_actorList.end();) {
		if ((*(*it)).isAlive()) {
			(*(*it)).doSomething();
			it++;
			if (!nachenblaster->isAlive()) {
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
			if (aliensKilled() == nAliensToAdvance()) {
				playSound(SOUND_FINISHED_LEVEL);
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
		else {
			Actor* st = *it;
			it = m_actorList.erase(it);
			delete st;
		}
	}
	setGameStatText(updateStatusBar());
	if (randInt(1, 15) == 8)
		addActorToList(new Star(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
	if (createAlien())
		addActorToList(createNewAlien());
	cout << "Aliens to advance: " << m_nAliensToAdvance << endl;
	cout << "m_maxAliensOnScreen: " << m_maxAliensOnScreen << endl;
	cout << "m_aliensOnScreen: " << m_aliensOnScreen << endl;
	cout << "aliens killed: " << m_aliensKilled << endl;
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

bool StudentWorld::createAlien() {
	int r = nAliensToAdvance() - aliensKilled();
	int m = maxAliensOnScreen();
	bool a = aliensOnScreen() < min(m, r);
	return a;
}

Alien* StudentWorld::createNewAlien() {
	addNewAlienOnScreen();
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
		return true;
	}
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
	if (distance < .75 * (a->getRadius() + (nachenblaster)->getRadius())) {
		a->sufferDamage(COLLISION_WITH_PLAYER, nachenblaster);
	}
	for (list<Actor*>::iterator it = m_actorList.begin(); it != m_actorList.end(); it++) {
		if((*it)->isAlive() && (*it) != a && !(*it)->isStar()){
			distancex = ((*it)->getX() - a->getX()) * ((*it)->getX() - a->getX());
			distancey = ((*it)->getY() - a->getY()) * ((*it)->getY() - a->getY());
			distance = sqrt(distancex + distancey);
			if (distance < .75 * (a->getRadius() + (*it)->getRadius()))
				//Did it collide with a nachenblaster-fired cabbage/torpedo?
				if ((*it)->isProjectile() && !static_cast<Projectile*>(*it)->shotByAlien())
					if (static_cast<Projectile*>(*it)->isTorpedo())
						a->sufferDamage(COLLISION_WITH_TORPEDO, *it);
					else
						a->sufferDamage(COLLISION_WITH_PROJECTILE, *it);
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
			playSound(SOUND_BLAST);
			p->sufferDamage(COLLISION_WITH_TORPEDO, nachenblaster);
			return COLLISION_WITH_TORPEDO;
		}
		else {
			playSound(SOUND_BLAST);
			p->sufferDamage(COLLISION_WITH_PROJECTILE, nachenblaster);
			return COLLISION_WITH_PROJECTILE;
		}
	return NO_COLLISION;
}

//check for collisions with nachenblaster
NachenBlaster* StudentWorld::checkForCollisions(Goodie * g) {
	if (!g->isAlive() || !nachenblaster->isAlive())
		return nullptr;
	double distancex, distancey;
	double distance;
	distancex = ((nachenblaster)->getX() - g->getX()) * ((nachenblaster)->getX() - g->getX());
	distancey = ((nachenblaster)->getY() - g->getY()) * ((nachenblaster)->getY() - g->getY());
	distance = sqrt(distancex + distancey);
	if (distance < .75 * (g->getRadius() + (nachenblaster)->getRadius()))
		return nachenblaster;
	return nullptr;
}

int StudentWorld::aliensKilled() const {
	return m_aliensKilled;
}

int StudentWorld::nAliensToAdvance() const {
	return m_nAliensToAdvance;
}

void StudentWorld::killedAnAlien() {
	playSound(SOUND_DEATH);
	m_aliensKilled++;
	m_aliensOnScreen--;
}

void StudentWorld::setInitAlienStats() {
	m_nAliensToAdvance = 6 + (4 * getLevel());
	m_maxAliensOnScreen = 4 + (.5 * getLevel());
	m_aliensOnScreen = 0;
	m_aliensKilled = 0;
}

int StudentWorld::maxAliensOnScreen() const {
	return m_maxAliensOnScreen;
}

int StudentWorld::aliensOnScreen() const
{
	return m_aliensOnScreen;
}

void StudentWorld::addNewAlienOnScreen()
{
	m_aliensOnScreen++;
}

void StudentWorld::decAliensOnScreen() {
	m_aliensOnScreen--;
}

string StudentWorld::updateStatusBar()
{ 
	std::ostringstream status_bar;
	status_bar << "Lives: " << getLives() << "  " << 
		"Health: " << int((nachenblaster->hitPoints()/50.0) * 100) << "%  " << 
		"Score: " << getScore() << "  " << 
		"Level: " << getLevel() << "  " << 
		"Cabbages: " << int((nachenblaster->cabbagePoints()/30.0) * 100)<< "%  " <<
		"Torpedoes: " << nachenblaster->torpedoInventory();
	string s = status_bar.str();
	return s;
}
