#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
using Direction = int;

Actor::Actor(StudentWorld* s, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
	: GraphObject(imageID, startX, startY, dir, size, depth), m_studentWorld(s), m_isAlive(true) {}

StudentWorld* Actor::getWorld() {
	return m_studentWorld;
}

bool Actor::isAlive() const {
	return m_isAlive;
}

void Actor::setAlive(bool alive) {
	m_isAlive = alive;
}

bool Actor::isAlien() const {
	return false;
}

bool Actor::isProjectile() const {
	return false;
}

bool Actor::isStar() const {
	return false;
}

NachenBlaster::NachenBlaster(StudentWorld* s)
	: Actor(s,IID_NACHENBLASTER, 0, 128, 0, 1.0, 0), m_hitPoints(50), m_cabbagePoints(30), m_torpedoInventory(0) {}

void NachenBlaster::doSomething() {
	if(cabbagePoints() < 30)
		addCabbagePoint();
	if (m_hitPoints <= 0)
		return;
	int ch;
	if(getWorld()->getKey(ch)){
		switch (ch) {
		case KEY_PRESS_DOWN:
			if (getY() - 6 >= 0)
				moveTo(getX(), getY() - 6);
			break;
		case KEY_PRESS_LEFT:
			if (getX() - 6 >= 0)
				moveTo(getX() - 6, getY());
			break;
		case KEY_PRESS_RIGHT:
			if (getX() + 6 < (VIEW_WIDTH - 1))
				moveTo(getX() + 6, getY());
			break;
		case KEY_PRESS_UP:
			if (getY() + 6 < (VIEW_HEIGHT - 1))
				moveTo(getX(), getY() + 6);
			break;
		case KEY_PRESS_SPACE:
			if (cabbagePoints() >= 5) {
				shootCabbage();
			}
			break;
		case KEY_PRESS_TAB:
			if (torpedoInventory() > 0)
				shootTorpedo();
		}
	}
	return;
}

int NachenBlaster::hitPoints() const {
	return m_hitPoints;
}

void NachenBlaster::sufferDamage(int damage) {
	m_hitPoints -= damage;
	if (m_hitPoints <= 0)
		setAlive(false);
}

int NachenBlaster::cabbagePoints() const
{
	return m_cabbagePoints;
}

void NachenBlaster::addCabbagePoint()
{
	m_cabbagePoints++;
}

int NachenBlaster::torpedoInventory() const {
	return m_torpedoInventory;
}

void NachenBlaster::shootCabbage() {
	m_cabbagePoints -= 5;
	getWorld()->addActorToList(new Cabbage(getWorld(), getX() + 12, getY()));
}

void NachenBlaster::shootTorpedo() {
	m_torpedoInventory--;
	getWorld()->addActorToList(new Flatulence_Torpedo(false, getWorld(), getX() + 12, getY(), 0));
}

Star::Star(int startX, int startY, StudentWorld* s)
	: Actor(s, IID_STAR, startX, startY, 0, randInt(5, 50) * .01, 3) {}

void Star::doSomething() {
	moveTo(getX() - 1, getY());
}

bool Star::isStar() const {
	return true;
}

Alien::Alien(double hitPoints, StudentWorld* s, double travelSpeed, int flightLength, int IMAGE_ID, int xDirection, int yDirection)
	: Actor(s, IMAGE_ID, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), 0, 1.5, 1), m_flightLength(flightLength), m_travelSpeed(travelSpeed), 
	m_xDirection(xDirection), m_yDirection(yDirection), m_hitPoints(hitPoints) {}

void Alien::doSomething() {
	if (!isAlive())
		return;
	if (getX() < 0) {
		setAlive(false);
		getWorld()->decAliensOnScreen();
		cout << "Alien died 3" << endl;
		return;
	}
	getWorld()->checkForCollisions(this);
	if (needsNewFlightPlan())
		setNewFlightPlan();
	//#5
	if (!getWorld()->playerInLineOfFire(this) || !reactToPlayerInLineOfFire()) {
			moveTo(getX() + (m_xDirection * travelSpeed()), getY() + (m_yDirection * travelSpeed()));
			m_flightLength--;
		}

	getWorld()->checkForCollisions(this);
	//Simple movements
	//reactToPlayerInLineOfFire();
	//moveTo(getX() - 1, getY());

}

void Alien::flightPlan(int &x, int& y) {
	x = m_xDirection;
	y = m_yDirection;
}

double Alien::travelSpeed() const {
	return m_travelSpeed;
}

int Alien::flightLength() const {
	return m_flightLength;
}

bool Alien::needsNewFlightPlan() const {
	return  getY() <= 0 || getY() >= VIEW_HEIGHT - 1 || flightLength() == 0;
}

void Alien::setNewFlightPlan() {
	if (getY() >= VIEW_HEIGHT - 1) {
		m_xDirection = -1;
		m_yDirection = -1;
	}
	else if (getY() <= 0) {
		m_xDirection = -1;
		m_yDirection = 1;
	}
	else if (flightLength() == 0) {
		int i = randInt(0, 2);
		switch (i) {
		case 0: 
			//Due left
			m_xDirection = -1;
			m_yDirection = 0;
			break;
		case 1: 
			//Due up and left
			m_xDirection = -1;
			m_yDirection = 1;
			break;
		case 2:
			//Due down and left
			m_xDirection = -1;
			m_yDirection = -1;
			break;
		}
	}
	m_flightLength = randInt(1, 32);
}

bool Alien::isAlien() const {
	return true;
}

int Alien::hitPoints() const {
	return m_hitPoints;
}

void Alien::sufferDamage(int cause, Actor* a) {
	if (cause == COLLISION_WITH_PLAYER) {
		cout << "Collision with player" << endl;
		if(isSnagglegon())
			static_cast<NachenBlaster*>(a)->sufferDamage(5);   
		else
			static_cast<NachenBlaster*>(a)->sufferDamage(15);
		setAlive(false);
		getWorld()->killedAnAlien();
		cout << "Alien died 1" << endl;
		return;
		//Increase player's score
		//Introduce a new explosion object
	}
	if (cause == COLLISION_WITH_PROJECTILE) {
		cout << "Alien hit with cabbage. Hit points left: " << hitPoints() << endl;
		m_hitPoints -= 2;  
	}
	else if (cause == COLLISION_WITH_TORPEDO) {
		cout << "Alien hit with torpedo" << endl;
		m_hitPoints -= 8;
	}
	a->setAlive(false);
	if (m_hitPoints <= 0) {
		//Increase player's score depending on type of alien this is! virtual function here
		setAlive(false);
		getWorld()->killedAnAlien();
		cout << "Alien died 2" << endl;
	}
}

bool Alien::isSnagglegon() const {
	return false;
}
Smallgon::Smallgon(double hitPoints, StudentWorld *s) 
: Alien(hitPoints, s, 2.0, 0, IID_SMALLGON) {}

bool Smallgon::reactToPlayerInLineOfFire() {
	//Returns true if a projectile was shot
	if (randInt(1, 20 / (getWorld()->getLevel()) + 5) == 1) {
		getWorld()->addActorToList(new Turnip(getWorld(), getX() - 14, getY()));
		return true;
	}
	return false;
}

Smoregon::Smoregon(double hitPoints, StudentWorld *s)
	: Alien(hitPoints, s, 2.0, 0, IID_SMOREGON) {}

bool Smoregon::reactToPlayerInLineOfFire() {
	//Returns true if a projectile was shot
	if (randInt(1, 15 / (getWorld()->getLevel()) + 5) == 1) {
		getWorld()->addActorToList(new Turnip(getWorld(), getX() - 14, getY()));
		return true;
	}
	return false;
}

Snagglegon::Snagglegon(double hitPoints, StudentWorld *s)
	: Alien(hitPoints, s, 1.75, -1, IID_SNAGGLEGON, -1, -1) {}

bool Snagglegon::reactToPlayerInLineOfFire() {
	//Returns true if a projectile was shot
	if (randInt(1, 15 / (getWorld()->getLevel()) + 10) == 1) {
		getWorld()->addActorToList(new Flatulence_Torpedo(true, getWorld(), getX() - 14, getY(), 180));
		return true;
	}
	return false;
}

bool Snagglegon::isSnagglegon() const {
	return true;
}

Projectile::Projectile(StudentWorld* s, int startX, int startY, int IMAGE_ID, Direction dir)
	: Actor(s, IMAGE_ID, startX, startY, dir, .5, 1) {}

void Projectile::doSomething() {
	if (!isAlive())
		return;
	if (OutofBounds()) {
		setAlive(false);
		return;
	}
	if (shotByAlien())
		if (getWorld()->checkForCollisions(this) != NO_COLLISION)
			return;
	moveProjectile();
	rotateProjectile();
	if (shotByAlien())
		if (getWorld()->checkForCollisions(this) != NO_COLLISION)
			return;
}

void Projectile::rotateProjectile() {
	setDirection(getDirection() + 20);
}

bool Projectile::isProjectile() const {
	return true;
}

void Projectile::sufferDamage(int cause, Actor* a) {
	if (cause == COLLISION_WITH_PROJECTILE) {
		cout << "player hit turnip" << endl;
		static_cast<NachenBlaster*>(a)->sufferDamage(2); 	
	}
	else if (cause == COLLISION_WITH_TORPEDO) {
		cout << "player hit torpedo" << endl;
		static_cast<NachenBlaster*>(a)->sufferDamage(8);
	}
	setAlive(false);
}

bool Projectile::isTorpedo() const {
	return false;
}

Cabbage::Cabbage(StudentWorld* s, int startX, int startY)
	: Projectile(s, startX, startY, IID_CABBAGE, 0) {}

bool Cabbage::OutofBounds() const {
	return getX() >= VIEW_WIDTH;
}

void Cabbage::moveProjectile() {
	moveTo(getX() + 8, getY());
}

bool Cabbage::shotByAlien() const {
	return false;
}

Turnip::Turnip(StudentWorld * s, int startX, int startY)
	: Projectile(s, startX, startY, IID_TURNIP, 0) {}

bool Turnip::OutofBounds() const {
	return getX() < 0;
}

void Turnip::moveProjectile() {
	moveTo(getX() - 6, getY());
}

bool Turnip::shotByAlien() const {
	return true;
}

Flatulence_Torpedo::Flatulence_Torpedo(bool shotByAlien, StudentWorld * s, int startX, int startY, Direction dir)
	: Projectile(s, startX, startY, IID_TORPEDO, dir), m_shotByAlien(shotByAlien) {}

bool Flatulence_Torpedo::OutofBounds() const {
	return getX() < 0 || getX() >= VIEW_WIDTH;
}

void Flatulence_Torpedo::moveProjectile() {
	if (shotByAlien())
		moveTo(getX() - 8, getY());
	else
		moveTo(getX() + 8, getY());
}
void Flatulence_Torpedo::rotateProjectile() {
	return;
}

bool Flatulence_Torpedo::shotByAlien() const {
	return m_shotByAlien;
}

bool Flatulence_Torpedo::isTorpedo() const {
	return true;
}