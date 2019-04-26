#include "Actor.h"
#include "StudentWorld.h"
#include "iostream"
Actor::Actor(int imageID, double startX, double startY, int startDirection, int	depth, double size, StudentWorld* studentWorld)
	:GraphObject(imageID, startX, startY, startDirection, depth, size)
{
	m_dead = false;
	m_infected = false;
	m_studentWorld = studentWorld;
	m_infectionCount = 0;
	m_gasCan = 0;
	m_vaccine = 0;
	m_landmine = 0;
}

int Actor::getGasCan()
{
	return m_gasCan;
}

int Actor::getVaccine()
{
	return m_vaccine;
}
int Actor::getLandmine()
{
	return m_landmine;
}

void Actor::incrementGasCan()
{
	m_gasCan += 5;
}

void Actor::decrementGasCan()
{
	m_gasCan--;
}

void Actor::incrementVaccine()
{
	m_vaccine++;
}

void Actor::decrementVaccine()
{
	m_vaccine--;
}

void Actor::incrementLandmine()
{
	m_landmine += 2;
}

void Actor::decrementLandmine()
{
	m_landmine--;
}

bool Actor::isDead()
{
	return m_dead;
}

void Actor::setDead(bool dead)
{
	m_dead = dead;
}
bool Actor::isInfected()
{
	return m_infected;
}

bool Actor::inDanger()
{
	return false;
}

void Actor::setInfection(bool infectionStatus)
{
	m_infected = infectionStatus;
	if (infectionStatus == false)
		m_infectionCount = 0;
}

void Actor::incrementInfection()
{
	m_infectionCount++;
}
StudentWorld* Actor::getWorld()
{
	return m_studentWorld;
}

bool Actor::canOverlap()
{
	return false;
}

bool Actor::canBeInfected()
{
	return false;
}

int Actor::getInfection()
{
	return m_infectionCount;
}

bool Actor::isEntity()
{
	return false;
}

bool Actor::isHazard()
{
	return false;
}

bool Actor::canDamage()
{
	return false;
}

bool Actor::isGoodie()
{
	return false;
}

bool Actor::isProjectile()
{
	return false;
}

Goodie::Goodie(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
	: Actor(imageID, startX, startY, startDirection, depth, size, studentWorld)
{

}

bool Goodie::isGoodie()
{
	return true;
}

bool Goodie::canOverlap()
{
	return true;
}

GasCanGoodie::GasCanGoodie(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
	: Goodie (imageID, startX, startY, startDirection, depth, size, studentWorld)
{

}

void GasCanGoodie::doSomething() //these goodies' dosomethings check if they are intersecting with penelope; if so, give penelope the proper goodie
{
	if (getWorld()->determineGoodieOverlap(this))
		getWorld()->giveGasCan();
}

VaccineGoodie::VaccineGoodie(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
	: Goodie(imageID, startX, startY, startDirection, depth, size, studentWorld)
{

}

void VaccineGoodie::doSomething()
{
	if (getWorld()->determineGoodieOverlap(this))
		getWorld()->giveVaccine();
}

LandmineGoodie::LandmineGoodie(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
	: Goodie(imageID, startX, startY, startDirection, depth, size, studentWorld)
{

}

void LandmineGoodie::doSomething()
{
	if (getWorld()->determineGoodieOverlap(this))
		getWorld()->giveLandmine();
}


Entity::Entity(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
	:Actor(imageID, startX, startY, startDirection, depth, size, studentWorld)
{
	m_paralyzed = false;
}

bool Entity::isEntity()
{
	return true;
}

void Entity::setParalyzed(bool value)
{
	m_paralyzed = value;
}

bool Entity::isParalyzed()
{
	return m_paralyzed;
}

GoodGuy::GoodGuy(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
	:Entity(imageID, startX, startY, startDirection, depth, size, studentWorld)
{
	
}

bool GoodGuy::canBeInfected()
{
	return true;
}

Penelope::Penelope(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
	:GoodGuy(imageID, startX, startY, startDirection, depth, size, studentWorld)
{
	
}

void Penelope::doSomething() 
{
	if (isInfected()) //Check if infected and increment infection counter if necessary
	{
		incrementInfection();
		if (getInfection() == 500) //kill Penelope
		{
			getWorld()->deadPenelope(); //call proper protocol
			return;
		}
	}
	int key;
	if (this->getWorld()->getKey(key))
	{
		switch (key)
		{
		case KEY_PRESS_SPACE: //create flame
			if (getGasCan() > 0)
			{
				decrementGasCan();
				getWorld()->createFlame(getDirection(), this);
			}
			break;
		case KEY_PRESS_ENTER: //give vaccine
			if (getVaccine() > 0)
			{
				decrementVaccine();
				setInfection(false);
			}
			break;
		case KEY_PRESS_TAB: //give landmine
			if (getLandmine() > 0)
			{
				decrementLandmine();
				getWorld()->createLandmine(getX(), getY());
			}
			break;
		case KEY_PRESS_UP: //move in proper direction
			setDirection(up);
			if (this->getWorld()->canMove(this, getX(), getY()+4, up))
				moveTo(getX(), getY() + 4);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (this->getWorld()->canMove(this, getX(), getY() - 4, down))
				moveTo(getX(), getY() - 4);
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (this->getWorld()->canMove(this, getX() + 4, getY(), right))
				moveTo(getX() + 4, getY());
			break;
		case KEY_PRESS_LEFT:
			setDirection(left);
			if (this->getWorld()->canMove(this, getX() - 4, getY(), left))
				moveTo(getX() - 4, getY());
			break;
		}
	}
}




Citizen::Citizen(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
	:GoodGuy(imageID, startX, startY, startDirection, depth, size, studentWorld)
{
	
}


void Citizen::doSomething() 
{
	if (isInfected())
		incrementInfection();
	if (getInfection() == 500) //kill citizen
	{
		//call proper protocol for killing citizen
		int rand = randInt(1, 10);
		if (1 <= rand && rand <= 3) //here, we have a 30% chance of creating a smart zombie in the citizen's location, and a 70% chance of creating a dumb zombie in the citizen's location
			getWorld()->createSmartZombie(this, getX(), getY());
		else
			getWorld()->createDumbZombie(this, getX(), getY());
	}

	if (!isParalyzed())
	{
		double dist_p = getWorld()->determinePenelopeDistance(this);
		double dist_z = getWorld()->determineMinZombieDistance(this, getX(), getY());
		setDanger(false); //assume citizen's can move toward Penelope (not in danger); change this if it turns out zombies are closer
		if ((dist_z > 80 * 80 || dist_z == -1) && dist_p > 80 * 80) //don't move at all; zombies and penelopes aren't nearby
		{
			setParalyzed(true);
			return;
		}
		if (dist_z < dist_p && dist_z != -1)
			setDanger(true);
		int dir = getWorld()->determineMovement(this);
		switch (dir)
		{
		case up: 
			if (this->getWorld()->canMove(this, getX(), getY() + 2, up))
			{
				setDirection(up);
				moveTo(getX(), getY() + 2);
			}
			break;
		case down:
			if (this->getWorld()->canMove(this, getX(), getY() - 2, down))
			{
				setDirection(down);
				moveTo(getX(), getY() - 2);
			}
			break;
		case right:
			if (this->getWorld()->canMove(this, getX() + 2, getY(), right))
			{
				setDirection(right);
				moveTo(getX() + 2, getY());
			}
			break;
		case left:
			if (this->getWorld()->canMove(this, getX() - 2, getY(), left))
			{
				setDirection(left);
				moveTo(getX() - 2, getY());
			}
			break;
		}
		setParalyzed(true);
	}
	else
		setParalyzed(false);
}


bool Citizen::inDanger()
{
	return m_danger;
}

void Citizen::setDanger(bool danger)
{
	m_danger = danger;
}

Wall::Wall(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
	:Actor(imageID, startX, startY, startDirection, depth, size, studentWorld)
{

}

void Wall::doSomething()
{

}


Exit::Exit(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld) 
	:Actor(imageID, startX, startY, startDirection, depth, size, studentWorld)
{

}

void Exit::doSomething()  
{
	getWorld()->determineExitOverlap(this);
}

bool Exit::canOverlap()
{
	return true;
}

bool Exit::isExit()
{
	return true;
}

Zombie::Zombie(int imageID, double startX, double startY, int startDirection, int depth, double size, int movementPlan, StudentWorld* studentWorld)
	:Entity(imageID, startX, startY, startDirection, depth, size, studentWorld)
{
	m_movementPlan = 0;
}

void Zombie::setMovementPlan(int movementPlan)
{
	m_movementPlan = movementPlan;
}

int Zombie::getMovementPlan()
{
	return m_movementPlan;
}

void Zombie::doSomething() {}


void Zombie::computeVomit(int direction) //create future vomit coords; will use later
{
	switch (direction)
	{
	case left:
	{
		m_vomitX = getX() - SPRITE_WIDTH;
		m_vomitY = getY();
		break;
	}
	case right:
	{
		m_vomitX = getX() + SPRITE_WIDTH;
		m_vomitY = getY();
		break;
	}
	case up:
	{
		m_vomitX = getX();
		m_vomitY = getY() + SPRITE_HEIGHT;
		break;
	}
	case down:
	{
		m_vomitX = getX();
		m_vomitY = getY() - SPRITE_HEIGHT;
		break;
	}
	}
}

double Zombie::getVomitX()
{
	return m_vomitX;
}

double Zombie::getVomitY()
{
	return m_vomitY;
}

void Zombie::moveRandomly() //this random movement function will be utilized by both dumb and smart zombie, so it is a zombie function
{
	if (getMovementPlan() == 0) //create new movement plan if necessary
	{
		int rand = randInt(1, 4);
		switch (rand)
		{
		case 1:
			setDirection(up);
			break;
		case 2:
			setDirection(down);
			break;
		case 3:
			setDirection(left);
			break;
		case 4:
			setDirection(right);
			break;
		}
		setMovementPlan(randInt(3, 10));
	}
	int dir = getDirection(); //move in current direction
	switch (dir)
	{
	case up:
		if (this->getWorld()->canMove(this, getX(), getY() + 1, up))
			moveTo(getX(), getY() + 1);
		else
			setMovementPlan(0);
		break;
	case down:
		if (this->getWorld()->canMove(this, getX(), getY() - 1, down))
			moveTo(getX(), getY() - 1);
		else
			setMovementPlan(0);
		break;
	case right:
		if (this->getWorld()->canMove(this, getX() + 1, getY(), right))
			moveTo(getX() + 1, getY());
		else
			setMovementPlan(0);
		break;
	case left:
		if (this->getWorld()->canMove(this, getX() - 1, getY(), left))
			moveTo(getX() - 1, getY());
		else
			setMovementPlan(0);
		break;
	}
	if (getMovementPlan() != 0)
		setMovementPlan(getMovementPlan() - 1); //decrement movement plan after moving
}
DumbZombie::DumbZombie(int imageID, double startX, double startY, int startDirection, int depth, double size, int movementPlan, StudentWorld* studentWorld)
	:Zombie(imageID, startX, startY, startDirection, depth, size, movementPlan, studentWorld)
{
	
}

void DumbZombie::doSomething()
{
	if (!isDead())
	{
		if (!isParalyzed())
		{
			computeVomit(getDirection()); 
			{
				if (getWorld()->createVomit(getVomitX(), getVomitY())) //if vomit coords will overlap and vomit is created, return
					return;
			}
			moveRandomly(); //move randomly if did not vomit
			setParalyzed(true);
		}
		else
			setParalyzed(false);
	}
}

SmartZombie::SmartZombie(int imageID, double startX, double startY, int startDirection, int depth, double size, int movementPlan, StudentWorld* studentWorld)
	: Zombie(imageID, startX, startY, startDirection, depth, size, movementPlan, studentWorld)
{
	
}

void SmartZombie::doSomething()
{
	if (!isDead())
	{
		if (!isParalyzed())
		{
			computeVomit(getDirection()); //if vomited, return
			{
				if (getWorld()->createVomit(getVomitX(), getVomitY()))
					return;
			}
			int movementDir = getWorld()->determineMovement(this);
			if (movementDir == -1)
			{
				moveRandomly();
			}
			else
			{
				switch (movementDir)
				{
				case up: //up
					setDirection(up);
					if (this->getWorld()->canMove(this, getX(), getY() + 1, up))
						moveTo(getX(), getY() + 1);
					else
						setMovementPlan(0);
					break;
				case down: //down
					setDirection(down);
					if (this->getWorld()->canMove(this, getX(), getY() - 1, down))
						moveTo(getX(), getY() - 1);
					else
						setMovementPlan(0);
					break;
				case right: //right
					setDirection(right);
					if (this->getWorld()->canMove(this, getX() + 1, getY(), right))
						moveTo(getX() + 1, getY());
					else
						setMovementPlan(0);
					break;
				case left: //left
					setDirection(left);
					if (this->getWorld()->canMove(this, getX() - 1, getY(), left))
						moveTo(getX() - 1, getY());
					else
						setMovementPlan(0);
					break;
				}
			}
			setParalyzed(true);
		}
		else
			setParalyzed(false);
	}
}

Hazard::Hazard(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
	: Actor(imageID, startX, startY, startDirection, depth, size, studentWorld)
{
	m_tickCounter = 0;
}

void Hazard::doSomething(){}

int Hazard::getTick()
{
	return m_tickCounter;
}

void Hazard::incrementTick()
{
	m_tickCounter++;
}

Pit::Pit(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
	: Hazard(imageID, startX, startY, startDirection, depth, size, studentWorld)
{

}

bool Hazard::isHazard()
{
	return true;
}

bool Hazard::canOverlap()
{
	return true;
}

bool Hazard::canDamage()
{
	return true;
}

Projectile::Projectile(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
 : Hazard(imageID, startX, startY, startDirection, depth, size, studentWorld)
{

}

bool Projectile::isProjectile()
{
	return true;
}

Vomit::Vomit(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
	: Projectile(imageID, startX, startY, startDirection, depth, size, studentWorld)
{
	
}

bool Vomit::canDamage()
{
	return false;
}

void Vomit::doSomething()
{
	if (getTick() == 2)
	{
		setDead(true);
		return;
	}
	getWorld()->determineVomitOverlap(getX(), getY());
	incrementTick();
}

Flame::Flame(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
	: Projectile(imageID, startX, startY, startDirection, depth, size, studentWorld)
{

}

bool Flame::canOverlap()
{
	return true;
}

void Flame::doSomething()
{
	if (getTick() == 2) //should only last for 2 ticks
	{
		setDead(true);
		return;
	}
	getWorld()->determineHazardOverlap(this);
	incrementTick();
}

void Pit::doSomething()
{
	getWorld()->determineHazardOverlap(this);
	
}

Landmine::Landmine(int imageID, double startX, double startY, int startDirection, int depth, double size, StudentWorld* studentWorld)
	: Hazard(imageID, startX, startY, startDirection, depth, size, studentWorld)
{

}

void Landmine::doSomething()
{
	if (getTick() >= 30 && getWorld()->landmineTriggered(this)) //landmine should activate after 30 ticks
	{
		trigger();
	}
	if (getTick() == 30) //stop incrementing (don't want integer overflow)
		return;
	incrementTick();
}

void Landmine::trigger() //landmine exploded; call proper protocol
{
	setDead(true);
	getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
	getWorld()->createFlame(getDirection(), this);
	getWorld()->createPit(getX(), getY());
}

void Actor::deadZombie()
{
	getWorld()->deadZombie(this);
}

void DumbZombie::deadZombie() 
{
	getWorld()->deadDumbZombie(this);
}
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
