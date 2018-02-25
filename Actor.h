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
	bool setAlive(bool alive);
private:
	StudentWorld* m_studentWorld; 
	bool m_isAlive;
};

class NachenBlaster : public Actor {
public:
	NachenBlaster(StudentWorld* s);
	void doSomething();
private:
	int m_hitPoints;
	int m_cabbagePoints;
	
};

class Star : public Actor {
public:
	Star(int startX, int startY, StudentWorld* s);
	void doSomething();
};

class Alien : public Actor {
public: 
	Alien(StudentWorld* s, double travelSpeed, int flightLength, int IMAGE_ID, int xDirection = -1, int yDirection = 0); 
	void virtual doSomething(); 
	void flightPlan(int &x, int& y); 
	double travelSpeed() const;
	int flightLength() const;
	bool needsNewFlightPlan();
	void virtual setNewFlightPlan();
private:
	double m_travelSpeed;
	int m_flightLength;
	//m_xDirection and m_yDirection must be either 1 or 0 or -1
	int m_xDirection;
	int m_yDirection;
};

class Smallgon : public Alien {
public:
	Smallgon(StudentWorld *s);
};

class Snagglegon : public Alien {
};



class Smoregon : public Alien {};
#endif // ACTOR_H_
