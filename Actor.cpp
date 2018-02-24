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

//Dummy function
bool NachenBlaster::setAlive() {
	return true;
}

Star::Star(int startX, int startY, StudentWorld* s)
	: Actor(s, IID_STAR, startX, startY, 0, randInt(5, 50) * .01, 3) {}

void Star::doSomething() {
	moveTo(getX() - 1, getY());
}

//Dummy function
bool Star::setAlive() {
	return true;
}

Alien::Alien(double travelSpeed, int flightLength, int xDirection, int yDirection, int IMAGE_ID)
	: Actor() {}

void Alien::doSomething() {
	//For smoregon/smallgon
}

void Alien::flightPlan(int &x, int& y) {
	x = m_xDirection;
	y = m_yDirection;
}

double Alien::travelSpeed() {
	return m_travelSpeed;
}

int Alien::flightLength() {
	return m_flightLength;
}

