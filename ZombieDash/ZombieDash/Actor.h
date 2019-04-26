#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual void doSomething() = 0;
	bool isDead();
	void setDead(bool dead);
	StudentWorld* getWorld();
	virtual bool canOverlap();
	virtual bool canBeInfected();
	virtual bool isEntity();
	virtual bool isHazard();
	virtual bool inDanger();
	virtual bool canDamage();
	virtual bool isGoodie();
	virtual bool isProjectile();
	void incrementGasCan();
	void incrementVaccine();
	void incrementLandmine();
	void decrementGasCan();
	void decrementVaccine();
	void decrementLandmine();
	int getGasCan();
	int getVaccine();
	int getLandmine();
	void setInfection(bool infectionStatus);
	bool isInfected();
	void incrementInfection();
	int getInfection();
	virtual void trigger() {}
	virtual void deadZombie();
private:
	int m_infectionCount;
	bool m_dead;
	bool m_infected;
	StudentWorld* m_studentWorld;
	int m_gasCan;
	int m_vaccine;
	int m_landmine;
};

class Goodie : public Actor
{
 public:
	Goodie(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual bool isGoodie();
	virtual bool canOverlap();

};

class GasCanGoodie : public Goodie
{
public:
	GasCanGoodie(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual void doSomething();
};

class VaccineGoodie : public Goodie
{
public:
	VaccineGoodie(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual void doSomething();
};

class LandmineGoodie : public Goodie
{
public:
	LandmineGoodie(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual void doSomething();
};
class Entity : public Actor
{
public:
	Entity(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual bool isEntity();
	bool isParalyzed();
	void setParalyzed(bool value);
private:
	bool m_paralyzed;
};

class GoodGuy : public Entity
{
public:
	GoodGuy(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual bool GoodGuy::canBeInfected();
};
class Penelope: public GoodGuy
{
public:
	Penelope(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual void doSomething();
};

class Citizen : public GoodGuy
{
public:
	Citizen(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual void doSomething();
	bool inDanger();
	void setDanger(bool danger);
private:
	bool m_danger;
};

class Wall: public Actor
{
public:
	Wall(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual void doSomething();
private:
};


class Exit : public Actor
{
public:
	Exit(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual void doSomething();
	virtual bool canOverlap();
	virtual bool isExit();
};

class Zombie : public Entity
{
public:
	Zombie(int imageID, double startX, double startY, int startDirection, int depth, double size, int movementPlan, StudentWorld* studentWorld);
	virtual void doSomething();
	void setMovementPlan(int movementPlan);
	int getMovementPlan();
	void computeVomit(int direction);
	double getVomitX();
	double getVomitY();
	void moveRandomly();
private:
	int m_movementPlan;
	double m_vomitX;
	double m_vomitY;
};

class DumbZombie : public Zombie
{
public:
	DumbZombie(int imageID, double startX, double startY, int startDirection, int depth, double size, int movementPlan, StudentWorld* studentWorld);
	virtual void doSomething();
	virtual void deadZombie();
};

class SmartZombie : public Zombie
{
public:
	SmartZombie(int imageID, double startX, double startY, int startDirection, int depth, double size, int movementPlan, StudentWorld* studentWorld);
	virtual void doSomething();
};
class Hazard : public Actor
{
public:
	Hazard(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	void doSomething();
	virtual bool isHazard();
	virtual bool canOverlap();
	virtual bool canDamage();
	int getTick();
	void incrementTick();
private:
	int m_tickCounter;
};

class Projectile : public Hazard
{
public:
	Projectile(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual void doSomething() { }
	virtual bool isProjectile();
};

class Vomit : public Projectile
{
public:
	Vomit(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual void doSomething();
	virtual bool canDamage();
};

class Flame : public Projectile
{
public:
	Flame(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual void doSomething();
	virtual bool canOverlap();
};
class Pit : public Hazard
{
public:
	Pit(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual void doSomething();
};

class Landmine : public Hazard
{
public:
	Landmine(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld);
	virtual void doSomething();
	virtual void trigger();
};


#endif // ACTOR_H_
