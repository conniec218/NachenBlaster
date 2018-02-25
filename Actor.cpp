#include "Actor.h"
#include "StudentWorld.h"

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

NachenBlaster::NachenBlaster(StudentWorld* s)
	: Actor(s,IID_NACHENBLASTER, 0, 128, 0, 1.0, 0), m_hitPoints(50), m_cabbagePoints(30) {}

void NachenBlaster::doSomething() {
	if (m_hitPoints <= 0)
		return;
	int ch;
	if(getWorld()->getKey(ch)){
		//std::cerr << "getKey" << std::endl;
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
		}
	}
	return;
}

bool Actor::setAlive(bool alive) {
	m_isAlive = alive;
	return m_isAlive;
}

Star::Star(int startX, int startY, StudentWorld* s)
	: Actor(s, IID_STAR, startX, startY, 0, randInt(5, 50) * .01, 3) {}

void Star::doSomething() {
	moveTo(getX() - 1, getY());
}

Alien::Alien(StudentWorld* s, double travelSpeed, int flightLength, int IMAGE_ID, int xDirection, int yDirection)
	: Actor(s, IMAGE_ID, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), 0, 1.5, 1), m_flightLength(flightLength), m_travelSpeed(travelSpeed), 
	m_xDirection(xDirection), m_yDirection(yDirection) {}

void Alien::doSomething() {
	//For smoregon/smallgon
	if (!isAlive())
		return;
	if (getX() < 0)
		setAlive(false);
	if (needsNewFlightPlan())
		setNewFlightPlan();
	//Now the alien will try to move itself
	moveTo(getX() + (m_xDirection * travelSpeed()), getY() + (m_yDirection * travelSpeed()));

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

bool Alien::needsNewFlightPlan() {
	return m_flightLength == 0 || getY() <= 0 || getY() >= VIEW_HEIGHT - 1;
}

void Alien::setNewFlightPlan() {
	//for smore/small
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

Smallgon::Smallgon(StudentWorld *s) 
: Alien(s, 2.0, 0, IID_SMALLGON){}
