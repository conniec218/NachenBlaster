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
	void recoverHitPoints(int amt);
	int cabbagePoints() const;
	int torpedoInventory() const;
	void getTorpedoes(int amt);

private:
	void addCabbagePoint();
	void shootCabbage();
	void shootTorpedo();

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
	bool isAlien() const;
	int hitPoints() const;
	void sufferDamage(int cause, Actor* a);
	bool virtual isSnagglegon() const;
	void virtual possiblyDropGoodie() = 0;
private:
	bool virtual reactToPlayerInLineOfFire() = 0;
	bool needsNewFlightPlan() const;
	void setNewFlightPlan();
	double travelSpeed() const;
	int flightLength() const;

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
	void possiblyDropGoodie();
private:
	bool reactToPlayerInLineOfFire();
};

class Smoregon : public Alien {
public:
	Smoregon(double hitPoints, StudentWorld *s);
	void virtual possiblyDropGoodie();
private:
	bool reactToPlayerInLineOfFire();
};

class Snagglegon : public Alien {
public:
	Snagglegon(double hitPoints, StudentWorld *s);
	bool isSnagglegon() const;
	void virtual possiblyDropGoodie();
private:
	bool reactToPlayerInLineOfFire();
};

class Projectile : public Actor {
public:
	Projectile(StudentWorld* s, int startX, int startY, int IMAGE_ID, Direction dir);
	void doSomething();  //create a doDifferentiateThing function that is virtual? then specify in cabbage/turnip/flatulence torpedo class. or pure virtual?
	bool virtual isProjectile() const;
	void sufferDamage(int cause, Actor* a);
	bool virtual isTorpedo() const;
	bool virtual shotByAlien() const = 0;
private:
	bool virtual OutofBounds() const = 0;
	void virtual moveProjectile() = 0;
	void virtual rotateProjectile();
};

class Cabbage : public Projectile {
public:
	Cabbage(StudentWorld * s, int startX, int startY);
	bool virtual shotByAlien() const;
private:

	void moveProjectile();
	bool OutofBounds() const;
};

class Turnip : public Projectile {
public:
	Turnip(StudentWorld * s, int startX, int startY);
	bool virtual shotByAlien() const;
private:
	bool OutofBounds() const;
	void moveProjectile();
	
};

class Flatulence_Torpedo : public Projectile {
public:
	Flatulence_Torpedo(bool shotByAlien, StudentWorld * s, int startX, int startY, Direction dir);
	bool isTorpedo() const;
	bool virtual shotByAlien() const;
private:
	bool OutofBounds() const;
	void moveProjectile();
	void rotateProjectile();
	
	bool m_shotByAlien;
};

class Explosion : public Actor {
public:
	Explosion(StudentWorld* s, int startX, int startY);
	void doSomething();
private:
	int m_ticksPassed;
};

class Goodie : public Actor {
public:
	Goodie(StudentWorld* s, int startX, int startY, int IMAGE_ID);
	void doSomething();
	
private:
	void moveGoodie();
	void virtual goodiePickedUp(NachenBlaster* n) = 0;
};

class Repair_Goodie : public Goodie {
public:
	Repair_Goodie(StudentWorld* s, int startX, int startY);

private:
	void goodiePickedUp(NachenBlaster* n);
};

class Flatulence_Torpedo_Goodie : public Goodie {
public:
	Flatulence_Torpedo_Goodie(StudentWorld* s, int startX, int startY);

private:
	void goodiePickedUp(NachenBlaster* n);
};

class Extra_Life_Goodie : public Goodie {
public:
	Extra_Life_Goodie(StudentWorld* s, int startX, int startY);

private:
	void goodiePickedUp(NachenBlaster* n);
};

#endif // ACTOR_H_
