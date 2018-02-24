#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
using Direction = int;
class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(StudentWorld* s, int imageID, int startX, int startY, Direction dir = 0, double size = 1.0, unsigned int depth = 0);
	void virtual doSomething() = 0;
	StudentWorld* getWorld();
	bool isAlive() const;
	bool virtual setAlive() = 0;
private:
	StudentWorld* m_studentWorld; 
	bool m_isAlive;
};

class NachenBlaster : public Actor {
public:
	NachenBlaster(StudentWorld* s);
	void doSomething();
	bool setAlive();
private:
	int m_hitPoints;
	int m_cabbagePoints;
	
};

class Star : public Actor {
public:
	Star(int startX, int startY, StudentWorld* s);
	void doSomething();
	bool setAlive();
};

class Alien : public Actor {
public: 
	Alien(double travelSpeed, int flightLength, int xDirection = -1, int yDirection = 0, int IMAGE_ID); 
	void virtual doSomething(); 
	void flightPlan(int &x, int& y); 
	double travelSpeed();
	int flightLength();
private:
	double m_travelSpeed;
	int m_flightLength;
	//m_xDirection and m_yDirection must be either 1 or 0 or -1
	int m_xDirection;
	int m_yDirection;
};

class Snagglegon : public Alien {
};

class Smallgon : public Alien {};

class Smoregon : public Alien {};
#endif // ACTOR_H_
