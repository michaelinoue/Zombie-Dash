#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>
class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	bool overlap(double x1, double y1, double x2, double y2);
	bool canMove(Actor* actor, double xPos, double yPos, int dir);
	void determineExitOverlap(Actor* actor);
	int determineMovement(Actor* actor);
	bool createVomit(double xPos, double yPos);
	void determineVomitOverlap(double xPos, double yPos);
	double determinePenelopeDistance(Actor* actor);
	double determineMinZombieDistance(Actor* actor, double x, double y);
	void createDumbZombie(Actor* citizen, double xPos, double yPos);
	void createSmartZombie(Actor* citizen, double xPos, double yPos);
	void deadCitizen(Actor* citizen);
	void deadPenelope();
	void deadZombie(Actor* zombie);
	bool determineGoodieOverlap(Actor* goodie);
	void giveGasCan();
	void giveVaccine();
	void giveLandmine();
	void createFlame(int dir, Actor* actor);
	void determineHazardOverlap(Actor* hazard);
	void createPit(double xPos, double yPos);
	bool landmineTriggered(Actor* landmine);
	void createLandmine(double xPos, double yPos);
	void deadDumbZombie(Actor* zombie);
	bool overlapEnvironment(double x1, double y1);
private:
	std::list<Actor*> actors; //check
	int m_level;
	bool m_levelComplete;
	Actor* m_penelope; //We will make Penelope an actor so that she is easy to manage (won't have to delete separately, won't have to treat separately, etc.)
	int m_numCitizens;
};


#endif // STUDENTWORLD_H_
