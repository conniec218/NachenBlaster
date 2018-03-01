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
	void setAlive(bool alive);
	bool virtual isAlien() const;
	bool virtual isProjectile() const;
	bool virtual isStar() const;
private:
	StudentWorld* m_studentWorld; 
	bool m_isAlive;
};

class NachenBlaster : public Actor {
public:
	NachenBlaster(StudentWorld* s);
	void doSomething();
	int hitPoints() const;
	void sufferDamage(int damage);
	int cabbagePoints() const;
	void addCabbagePoint();
	void shootCabbage();
	void shootTorpedo();
	int torpedoInventory() const;
private:
	int m_hitPoints;
	int m_cabbagePoints;
	int m_torpedoInventory;
};

class Star : public Actor {
public:
	Star(int startX, int startY, StudentWorld* s);
	void doSomething();
	bool isStar() const;
};

class Alien : public Actor {
public: 
	Alien(double hitPoints, StudentWorld * s, double travelSpeed, int flightLength, int IMAGE_ID, int xDirection = -1, int yDirection = 0);
	void doSomething();
	void flightPlan(int &x, int& y); 
	double travelSpeed() const;
	int flightLength() const;
	bool needsNewFlightPlan() const;
	void setNewFlightPlan();
	bool isAlien() const;
	bool virtual reactToPlayerInLineOfFire() = 0;
	int hitPoints() const;
	void sufferDamage(int cause, Actor* a);
	bool virtual isSnagglegon() const;
private:
	double m_travelSpeed;
	int m_flightLength;
	//m_xDirection and m_yDirection must be either 1 or 0 or -1
	int m_xDirection;
	int m_yDirection;
	int m_hitPoints;  
};

class Smallgon : public Alien {
public:
	Smallgon(double hitPoints, StudentWorld *s);
	bool reactToPlayerInLineOfFire();
};

class Smoregon : public Alien {
public:
	Smoregon(double hitPoints, StudentWorld *s);
	bool reactToPlayerInLineOfFire();
};

class Snagglegon : public Alien {
public:
	Snagglegon(double hitPoints, StudentWorld *s);
	bool reactToPlayerInLineOfFire();
	bool isSnagglegon() const;
};

class Projectile : public Actor {
public:
	Projectile(StudentWorld* s, int startX, int startY, int IMAGE_ID, Direction dir);
	void doSomething();  //create a doDifferentiateThing function that is virtual? then specify in cabbage/turnip/flatulence torpedo class. or pure virtual?
	bool virtual OutofBounds() const = 0;
	void virtual moveProjectile() = 0;
	void virtual rotateProjectile();
	bool virtual isProjectile() const;
	bool virtual shotByAlien() const = 0;
	void sufferDamage(int cause, Actor* a);
	bool virtual isTorpedo() const;
};

class Cabbage : public Projectile {
public:
	Cabbage(StudentWorld * s, int startX, int startY);
	bool OutofBounds() const;
	void moveProjectile();
	bool virtual shotByAlien() const;
};

class Turnip : public Projectile {
public:
	Turnip(StudentWorld * s, int startX, int startY);
	bool OutofBounds() const;
	void moveProjectile();
	bool virtual shotByAlien() const;
};

class Flatulence_Torpedo : public Projectile {
public:
	Flatulence_Torpedo(bool shotByAlien, StudentWorld * s, int startX, int startY, Direction dir);
	bool OutofBounds() const;
	void moveProjectile();
	void rotateProjectile();
	bool shotByAlien() const;
	bool isTorpedo() const;
private:
	bool m_shotByAlien;
};
#endif // ACTOR_H_
