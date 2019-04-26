#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h" //check this boi
#include "Level.h"
#include "GraphObject.h"
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;


GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
	m_level = 1;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}


int StudentWorld::init()
{
	m_numCitizens = 0;
	m_levelComplete = false;
	ostringstream oss;
	oss << "level";
	Level lev(assetPath());
	if (m_level == 100) //we just completed level 99, finish
		return GWSTATUS_PLAYER_WON;
	if (m_level/10 == 0) //if number is below 10, add a 0 in front
	{
		oss << "0";
	}
	oss << m_level << ".txt";
	string levelFile = oss.str();
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;
	else if (result == Level::load_fail_bad_format)
	{
		return GWSTATUS_LEVEL_ERROR;
		cerr << "Your level was improperly formatted" << endl;
	}
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded level" << endl;

		for (int i = 0; i < LEVEL_WIDTH; i++)
		{
			int xPos = i * LEVEL_WIDTH;
			for (int j = 0; j < LEVEL_HEIGHT; j++)
			{
				Level::MazeEntry ge = lev.getContentsOf(i, j);
				int yPos = j * LEVEL_HEIGHT;
				switch (ge)
				{
				case Level::empty:
					break;
				case Level::smart_zombie:
				{
					Actor* smartZombie = new SmartZombie(IID_ZOMBIE, xPos, yPos, 0, 0, 1, 0, this);
					actors.push_back(smartZombie);
					break;
				}
				case Level::dumb_zombie:
				{
					Actor* dumbZombie = new DumbZombie(IID_ZOMBIE, xPos, yPos, 0, 0, 1, 0, this);
					actors.push_back(dumbZombie);
					break;
				}
				case Level::player:
				{
					Actor* penelope = new Penelope(IID_PLAYER, xPos, yPos, 0, 0, 1, this);
					actors.push_back(penelope);
					m_penelope = penelope;
					break;
				}
				case Level::exit:
				{
					Actor* exit = new Exit(IID_EXIT, xPos, yPos, 0, 1, 1, this);
					actors.push_back(exit);
					//cout << "Location 80,160 is where an exit is" << endl;
					break;
				}
				case Level::wall:
				{
					Actor* wall = new Wall(IID_WALL, xPos, yPos, 0, 0, 1, this);
					actors.push_back(wall);
					//cout << "Location 80,160 holds a Wall" << endl;
					break;
				}
				case Level::pit:
					//cout << "Location 80,160 has a pit in the ground" << endl;
				{
					Actor* pit = new Pit(IID_PIT, xPos, yPos, 0, 0, 1, this);
					actors.push_back(pit);
					break;
					// etc
				}

				case Level::citizen:
				{
					Actor* citizen = new Citizen(IID_CITIZEN, xPos, yPos, 0, 0, 1, this);
					actors.push_back(citizen);
					m_numCitizens++;
					break;
				}
				case Level::gas_can_goodie:
				{
					Actor* gasCanGoodie = new GasCanGoodie(IID_GAS_CAN_GOODIE, xPos, yPos, 0, 1, 1, this);
					actors.push_back(gasCanGoodie);
					break;
				}
				case Level::vaccine_goodie:
				{
					Actor* vaccineGoodie = new VaccineGoodie(IID_VACCINE_GOODIE, xPos, yPos, 0, 1, 1, this);
					actors.push_back(vaccineGoodie);
					break;
				}
				case Level::landmine_goodie:
				{
					Actor* landmineGoodie = new LandmineGoodie(IID_LANDMINE_GOODIE, xPos, yPos, 0, 1, 1, this);
					actors.push_back(landmineGoodie);
					break;
				}
				}
			}
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	list<Actor*>::iterator it = actors.begin();
	while (it != actors.end())
	{
		ostringstream sscore;
		if (getScore() < 0)
		{
			sscore << "-";
			sscore.fill('0'); //fill score with leading 0's
			sscore << setw(5) << -getScore();
		}
		else
		{
			sscore.fill('0');
			sscore << setw(6) << getScore();
		}
		string score = sscore.str();

		ostringstream oss;
		oss << "Score: " << score << "  " << "Level: " << m_level << "  " << "Lives: " << getLives() << "  " << "Vaccines: " << m_penelope->getVaccine() << "  " << "Flames: " << m_penelope->getGasCan() << "  " << "Mines: " << m_penelope->getLandmine() << "  " << "Infected: " << m_penelope->getInfection();
		string gameStats = oss.str();
		setGameStatText(gameStats);
		int prevLives = this->getLives(); 
		if (!(*it)->isDead())
		{
			(*it)->doSomething();
			if (prevLives > this->getLives())
			{
				playSound(SOUND_PLAYER_DIE);
				return GWSTATUS_PLAYER_DIED;
			}
			if (m_levelComplete)
			{					
				m_level++;
				playSound(SOUND_LEVEL_FINISHED);
				return GWSTATUS_FINISHED_LEVEL;
			}
		} 
		else
		{
			delete *it;
			it = actors.erase(it);
			continue;
		}
		it++;
	} 
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() //iterate through every actor and delete
{
	list<Actor*>::iterator it = actors.begin();
	while (it != actors.end())
	{
		delete *it;
		it = actors.erase(it);
	}
}


bool StudentWorld::overlap(double x1, double y1, double x2, double y2)  //compute the euclidean distance necessary to 'overlap'
{
	double euclidean = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
	if (euclidean <= 100)
		return true;
	return false;
}

bool StudentWorld::canMove(Actor* actor, double xPos, double yPos, int dir)
{
	list<Actor*>::iterator it = actors.begin();
	while (it != actors.end())
	{
		if (*it != actor)
		{
			double otherX = (*it)->getX();
			double otherY = (*it)->getY();
			if (!(*it)->canOverlap()) //we check if we can overlap with this object in the first place (e.g. can overlap with a goodie, but not a person)
			{
				switch (dir) //here, we check the bounding boxes of the object, with test cases on how the object moved there in the first place; returns false if an intersection will occur
				{
				case GraphObject::up:
					if (otherY <= yPos + SPRITE_HEIGHT - 1 && yPos <= otherY && (xPos >= otherX && xPos <= otherX + SPRITE_WIDTH - 1 || xPos <= otherX && xPos + SPRITE_WIDTH - 1 >= otherX))
						return false;
					break;
				case GraphObject::down:
					if (yPos <= otherY + SPRITE_HEIGHT - 1 && yPos >= otherY && (xPos >= otherX && xPos <= otherX + SPRITE_WIDTH - 1 || xPos <= otherX && xPos + SPRITE_WIDTH - 1 >= otherX))
						return false;
					break;
				case GraphObject::right:
					if (otherX <= xPos + SPRITE_WIDTH - 1 && xPos <= otherX && (yPos >= otherY && yPos <= otherY + SPRITE_HEIGHT - 1 || yPos <= otherY && yPos + SPRITE_HEIGHT - 1 >= otherY))
						return false;
					break;
				case GraphObject::left:
					if (xPos <= otherX + SPRITE_WIDTH - 1 && xPos >= otherX && (yPos >= otherY && yPos <= otherY + SPRITE_HEIGHT - 1 || yPos <= otherY && yPos + SPRITE_HEIGHT - 1 >= otherY))
						return false;
					break;
				}
			}
		}
		it++;
	}
	return true;
}

void StudentWorld::determineExitOverlap(Actor* actor)
{
	list<Actor*>::iterator it = actors.begin();

	while (it != actors.end())
	{
		if (*it != actor)
		{
			double x1 = actor->getX();
			double y1 = actor->getY();
			double x2 = (*it)->getX();
			double y2 = (*it)->getY();
			if ((*it)->canBeInfected() && overlap(x1, y1, x2, y2)) //check if it is penelope or citizen
			{
				if (*it == m_penelope)
				{
					if (m_numCitizens == 0)
						m_levelComplete = true;
				}
				else //if it's not Penelope, we just saved a citizen
				{
					increaseScore(1000); //can't call dead citizen; that would decrease the score
					(*it)->setDead(true);
					m_numCitizens--;
					playSound(SOUND_CITIZEN_SAVED);
				}

			}
		}
		it++;
	}
}

int StudentWorld::determineMovement(Actor* actor) //This serves as a dual-purpose function for both citizens and zombies, as many of the mechanics remain the same for both
{
	list<Actor*>::iterator it = actors.begin();
	double minVal;
	bool foundMin = false;
	Actor* minActor;
	while (it != actors.end())
	{
		double x1 = actor->getX();
		double y1 = actor->getY();
		double x2 = (*it)->getX();
		double y2 = (*it)->getY();
		if (actor->isEntity() && !actor->canBeInfected()) //method specifically for zombies
		{
			if ((*it) != actor && (*it)->canBeInfected()) //Here, we find the closest good guy that the zombie can follow
			{
				double euclidean = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
				if (euclidean <= 80 * 80)
				{
					if (!foundMin || euclidean < minVal)
					{
						foundMin = true; //we set a boolean flag that we found an actor in the first place, so that we don't go out of bounds
						minVal = euclidean;
						minActor = *it;
					}
				}
			}
		}
		else //method specifically for citizens
		{
			if ((*it) != actor && (*it)->isEntity() && !(*it)->canBeInfected()) //here, we are essentially checking if the actor is a zombie
			{
				double euclidean = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2); //if the euclidean distance is less than 80 pixels, we have a match
				if (euclidean <= 80 * 80)
				{
					if (!foundMin || euclidean < minVal)
					{
						foundMin = true; //Like before, we must set off the flag to avoid undefined behavior
						minVal = euclidean;
						minActor = *it;
					}
				}
			}
		}
		it++;
	}
	if (!actor->canBeInfected() && !foundMin) //if the zombie couldn't find anyone, leave and don't do anything
		return -1;

	if (actor->isEntity() && !actor->canBeInfected()) //if the zombie did find someone, check to see if they have the same x/y coords; if not, choose a random direction that will bring zombie closer
	{
		double xPos = minActor->getX();
		double yPos = minActor->getY();
		if (xPos == actor->getX())
		{
			if (yPos > actor->getY())
				return GraphObject::up;
			else
				return GraphObject::down;
		}
		if (yPos == actor->getY())
		{
			if (xPos > actor->getX())
				return GraphObject::right;
			else
				return GraphObject::left;
		}
		int rand = randInt(0, 1);

		if (xPos > actor->getX())
		{
			if (yPos > actor->getY())
			{
				if (rand == 0)
					return GraphObject::up;
				else
					return GraphObject::right;
			}
			else
			{
				if (rand == 0)
					return GraphObject::down;
				else
					return GraphObject::right;
			}
		}

		else
		{
			if (yPos > actor->getY())
			{
				if (rand == 0)
					return GraphObject::up;
				else
					return GraphObject::left;
			}
			else
			{
				if (rand == 0)
					return GraphObject::down;
				else
					return GraphObject::left;
			}
		}
	}
	else //method for citizens
	{
		if (actor->inDanger()) //protocol for moving when zombie is closer than penelope
		{
			if (!foundMin)
				return -1; //if we didn't find anything, don't try to compare zombie distances
			double distUp, distDown, distRight, distLeft; //Here, we compare all of the minimum zombie distances when a citizen chooses up, down, left right
			if (canMove(actor, actor->getX(), actor->getY() + 2, GraphObject::up))
				distUp = determineMinZombieDistance(actor, actor->getX(), actor->getY() + 2);
			else
				distUp = -1;
			if (canMove(actor, actor->getX(), actor->getY() - 2, GraphObject::down))
				distDown = determineMinZombieDistance(actor, actor->getX(), actor->getY() - 2);
			else
				distDown = -1;
			if (canMove(actor, actor->getX() + 2, actor->getY(), GraphObject::right))
				distRight = determineMinZombieDistance(actor, actor->getX() + 2, actor->getY());
			else
				distRight = -1;
			if (canMove(actor, actor->getX() - 2, actor->getY(), GraphObject::left))
				distLeft = determineMinZombieDistance(actor, actor->getX() - 2, actor->getY());
			else
				distLeft = -1;

			double max1;
			int dir1;
			if (distUp > distDown)
			{
				max1 = distUp;
				dir1 = GraphObject::up;
			}
			else
			{
				max1 = distDown;
				dir1 = GraphObject::down;
			}
			double max2;
			int dir2;
			if (distRight > distLeft)
			{
				max2 = distRight;
				dir2 = GraphObject::right;
			}
			else
			{
				max2 = distLeft;
				dir2 = GraphObject::left;
			}

			double max;
			int dir;
			if (max1 > max2)
			{
				max = max1;
				dir = dir1;
			}
			else
			{
				max = max2;
				dir = dir2;
			}
			if (max > minVal) //we find the direction to move according to which will give us the largest distance from the nearest zombie.
				return dir;
			else
				return -1; //if it turns out no distance will increase our minimum distance, we just return -1 (this means don't move)
		}
		
		else //protocol for when penelope is closer than zombie
		{
			double xPos = m_penelope->getX();
			double yPos = m_penelope->getY();
			if (xPos == actor->getX()) //like before, we choose a direction that will bring us closer to Penelope
			{
				if (yPos > actor->getY())
					return GraphObject::up;
				else
					return GraphObject::down;
			}
			if (yPos == actor->getY())
			{
				if (xPos > actor->getX())
					return GraphObject::right;
				else
					return GraphObject::left;
			}
			int rand = randInt(0, 1);

			if (xPos > actor->getX())
			{
				if (yPos > actor->getY())
				{
					if (rand == 0)
					{
						if (canMove(actor, actor->getX(), actor->getY() + 2, GraphObject::up)) //Here, we are just checking if we can even move in this random direction in the first place.
							return GraphObject::up;												//If we can't, return the other direction.
						else
							return GraphObject::right;
					}
					else
					{
						if (canMove(actor, actor->getX() + 2, actor->getY(), GraphObject::right))
							return GraphObject::right;
						else
							return GraphObject::up;
					}
				}
				else
				{
					if (rand == 0)
					{
						if (canMove(actor, actor->getX(), actor->getY() - 2, GraphObject::down))
							return GraphObject::down;
						else
							return GraphObject::right;
					}
					else
						return GraphObject::right;
				}
			}

			else
			{
				if (yPos > actor->getY())
				{
					if (rand == 0)
					{
						if (canMove(actor, actor->getX(), actor->getY() + 2, GraphObject::up))
							return GraphObject::up;
						else
							return GraphObject::left;
					}
					else
					{
						if (canMove(actor, actor->getX() - 2, actor->getY(), GraphObject::left))
							return GraphObject::left;
						else
							return GraphObject::up;
					}
				}
				else
				{
					if (rand == 0)
					{
						if (canMove(actor, actor->getX(), actor->getY() - 2, GraphObject::down))
							return GraphObject::down;
						else
							return GraphObject::left;
					}
					else
						return GraphObject::left;
				}
			}
		}
	}
	return -1; //So that all control paths return a value (this will never be returned; visual studio was just freaking out)
}
	
double StudentWorld::determinePenelopeDistance(Actor* actor) //compute euclidean distance of penelope with respect to citizen
{
	double x1 = actor->getX();
	double y1 = actor->getY();
	double x2 = m_penelope->getX();
	double y2 = m_penelope->getY();
	double distance = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	
	return distance;
}

double StudentWorld::determineMinZombieDistance(Actor* actor, double x, double y) //compute euclidean distance of zombie with respect to nearest good guy
{
	double x1 = x;
	double y1 = y;
	list<Actor*>::iterator it = actors.begin();
	double minVal;
	bool foundMin = false;
	while (it != actors.end())
	{
		if ((*it) != actor && (*it)->isEntity() && !(*it)->canBeInfected())
		{
			double x2 = (*it)->getX();
			double y2 = (*it)->getY();
			double euclidean = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
			if (!foundMin || euclidean < minVal)
			{
				foundMin = true;
				minVal = euclidean;
			}
		}
		it++;
	}
	if (!foundMin)
		return -1;
	return minVal;
}
bool StudentWorld::createVomit(double xPos, double yPos)
{
	list<Actor*>::iterator it = actors.begin();

	while (it != actors.end())
	{
		double x1 = xPos;
		double y1 = yPos;
		double x2 = (*it)->getX();
		double y2 = (*it)->getY();
		if ((*it)->canBeInfected() && overlap(x1, y1, x2, y2)) //check if citizen or penelope, and if they intersect with vomit coordinates
		{
			int x = randInt(1, 3); //generate a 1 in 3 chance that a vomit object will be introduced 
			if (x == 1)
			{
				Actor* vomit = new Vomit(IID_VOMIT, xPos, yPos, 0, 0, 1, this); //create vomit in vomit coords
				actors.push_back(vomit);
				playSound(SOUND_ZOMBIE_VOMIT);
				return true;
			}

		}
		it++;
	}
	return false;
}


void StudentWorld::determineVomitOverlap(double xPos, double yPos)
{
	list<Actor*>::iterator it = actors.begin();

	while (it != actors.end())
	{
		double x1 = xPos;
		double y1 = yPos;
		double x2 = (*it)->getX(); //see if anyone intersects with vomit
		double y2 = (*it)->getY();
		if ((*it)->canBeInfected() && overlap(x1, y1, x2, y2)) //check if citizen or penelope, and if they intersect with vomit coordinates
		{
			if (*it != m_penelope && !(*it)->isInfected())
				playSound(SOUND_CITIZEN_INFECTED); //only play the sound once; check if already infected
			(*it)->setInfection(true);
		}
		it++;
	}
}

void StudentWorld::createDumbZombie(Actor* citizen, double xPos, double yPos)
{
	citizen->setDead(true);
	m_numCitizens--;
	Actor* dumbZombie = new DumbZombie(IID_ZOMBIE, xPos, yPos, 0, 0, 1, 0, this);
	actors.push_back(dumbZombie);
	playSound(SOUND_ZOMBIE_BORN);
}

void StudentWorld::createSmartZombie(Actor* citizen, double xPos, double yPos)
{
	citizen->setDead(true);
	m_numCitizens--;
	Actor* smartZombie = new SmartZombie(IID_ZOMBIE, xPos, yPos, 0, 0, 1, 0, this);
	actors.push_back(smartZombie);
	playSound(SOUND_ZOMBIE_BORN);
}

void StudentWorld::deadCitizen(Actor* citizen) //protocol that is called when a citizen is killed (not saved)
{
	playSound(SOUND_CITIZEN_DIE);
	m_numCitizens--;
	citizen->setDead(true);
	increaseScore(-1000);
}

void StudentWorld::deadPenelope() //protocol that is called when Penelope is killed
{
	m_penelope->setDead(true);
	decLives();
}

void StudentWorld::deadZombie(Actor* zombie) //protocol that is called when 
{
	playSound(SOUND_ZOMBIE_DIE);
	zombie->setDead(true);
	increaseScore(2000);
}
bool StudentWorld::determineGoodieOverlap(Actor* goodie)
{
	list<Actor*>::iterator it = actors.begin();
	while (it != actors.end())
	{
		if (*it == m_penelope)
		{
			if (overlap(goodie->getX(), goodie->getY(), (*it)->getX(), (*it)->getY()))
			{
				increaseScore(50);
				
				goodie->setDead(true);
				playSound(SOUND_GOT_GOODIE);
				return true;
			}
		}
		it++;
	}
	return false;
}

void StudentWorld::giveGasCan()
{
	m_penelope->incrementGasCan();
}

void StudentWorld::giveVaccine()
{
	m_penelope->incrementVaccine();
}

void StudentWorld::giveLandmine()
{
	m_penelope->incrementLandmine();
}

void StudentWorld::createFlame(int dir, Actor* actor)
{
	double xPos = actor->getX();
	double yPos = actor->getY();
	if (actor == m_penelope)
	{
		playSound(SOUND_PLAYER_FIRE);
		for (int i = 1; i <= 3; i++)
		{
			switch (dir)
			{
			case GraphObject::up:
			{
				if (!overlapEnvironment(xPos, yPos + i * SPRITE_HEIGHT))
				{
					Actor* flame = new Flame(IID_FLAME, xPos, yPos + i * SPRITE_HEIGHT, dir, 0, 1, this);
					actors.push_back(flame);
				}
				else
					return;
				break;
			}
			case GraphObject::down:
			{
				if (!overlapEnvironment(xPos, yPos - i * SPRITE_HEIGHT))
				{
					Actor* flame = new Flame(IID_FLAME, xPos, yPos - i * SPRITE_HEIGHT, dir, 0, 1, this);
					actors.push_back(flame);
				}
				else
					return;
				break;
			}
			case GraphObject::right:
			{
				if (!overlapEnvironment(xPos + i * SPRITE_WIDTH, yPos))
				{
					Actor* flame = new Flame(IID_FLAME, xPos + i * SPRITE_WIDTH, yPos, dir, 0, 1, this);
					actors.push_back(flame);
				}
				else
					return;
				break;
			}
			case GraphObject::left:
			{
				if (!overlapEnvironment(xPos - i * SPRITE_WIDTH, yPos))
				{
					Actor* flame = new Flame(IID_FLAME, xPos - i * SPRITE_WIDTH, yPos, dir, 0, 1, this);
					actors.push_back(flame);
				}
				else
					return;
				break;
			}
			}
		}


	}
	else //called by a landmine, not penelope (slightly different orientation of flames; must account for this)
	{
		if (!overlapEnvironment(xPos, yPos))
		{
			Actor* flame = new Flame(IID_FLAME, xPos + SPRITE_WIDTH, yPos - SPRITE_HEIGHT, 90, 0, 1, this);
			actors.push_back(flame);
		}
		if (!overlapEnvironment(xPos + SPRITE_WIDTH, yPos))
		{
			Actor* flame = new Flame(IID_FLAME, xPos + SPRITE_WIDTH, yPos, 90, 0, 1, this);
			actors.push_back(flame);
		}
		if (!overlapEnvironment(xPos - SPRITE_WIDTH, yPos))
		{
			Actor* flame = new Flame(IID_FLAME, xPos - SPRITE_WIDTH, yPos, 90, 0, 1, this);
			actors.push_back(flame);
		}
		if (!overlapEnvironment(xPos + SPRITE_WIDTH, yPos + SPRITE_HEIGHT))
		{
			Actor* flame = new Flame(IID_FLAME, xPos + SPRITE_WIDTH, yPos + SPRITE_HEIGHT, 90, 0, 1, this);
			actors.push_back(flame);
		}
		if (!overlapEnvironment(xPos - SPRITE_WIDTH, yPos + SPRITE_HEIGHT))
		{
			Actor* flame = new Flame(IID_FLAME, xPos - SPRITE_WIDTH, yPos + SPRITE_HEIGHT, 90, 0, 1, this);
			actors.push_back(flame);
		}
		if (!overlapEnvironment(xPos, yPos + SPRITE_HEIGHT))
		{
			Actor* flame = new Flame(IID_FLAME, xPos, yPos + SPRITE_HEIGHT, 90, 0, 1, this);
			actors.push_back(flame);
		}
		if (!overlapEnvironment(xPos, yPos - SPRITE_HEIGHT))
		{
			Actor* flame = new Flame(IID_FLAME, xPos, yPos - SPRITE_HEIGHT, 90, 0, 1, this);
			actors.push_back(flame);
		}
		if (!overlapEnvironment(xPos + SPRITE_WIDTH, yPos - SPRITE_HEIGHT))
		{
			Actor* flame = new Flame(IID_FLAME, xPos + SPRITE_WIDTH, yPos - SPRITE_HEIGHT, 90, 0, 1, this);
			actors.push_back(flame);
		}
		if (!overlapEnvironment(xPos - SPRITE_WIDTH, yPos - SPRITE_HEIGHT))
		{
			Actor* flame = new Flame(IID_FLAME, xPos - SPRITE_WIDTH, yPos - SPRITE_HEIGHT, 90, 0, 1, this);
			actors.push_back(flame);
		}
	}
}


void StudentWorld::determineHazardOverlap(Actor* hazard)
{
	list<Actor*>::iterator it = actors.begin();
	while (it != actors.end())
	{
		if (hazard->isProjectile() && (*it)->isHazard() && !(*it)->isProjectile()) 
		{
			if (overlap((*it)->getX(), (*it)->getY(), hazard->getX(), hazard->getY()))
			{
				(*it)->trigger(); //trigger the landmine if flame overlaps
			}
		}
		else if (hazard->isProjectile() && (*it)->isGoodie()) //The reason we must check if the hazard is a projectile is simply to check if it's a flame. Flames destroy goodies, pits do not.
		{
			if (overlap((*it)->getX(), (*it)->getY(), hazard->getX(), hazard->getY()))
			{
				(*it)->setDead(true);
			}
		}
		if ((*it)->isEntity())
		{
			if (overlap((*it)->getX(), (*it)->getY(), hazard->getX(), hazard->getY()))
			{
				if ((*it)->canBeInfected()) //checking if penelope or citizen
				{
					if (*it == m_penelope)
					{
						if (!m_penelope->isDead()) //Here, I'm checking to see if these objects aren't already dead (otherwise, 2 flames could technically overlap and count as two kills)
							deadPenelope();
					}
					else
					{
						if (!(*it)->isDead())
							deadCitizen(*it);
					}
				}
				else  //if it's neither of those, it must be a zombie; call dead zombie protocol
				{
					if (!(*it)->isDead())
						(*it)->deadZombie();
				}
			}
		}
		it++;
	}

}


void StudentWorld::createLandmine(double xPos, double yPos)
{
	Actor* landmine = new Landmine(IID_LANDMINE, xPos, yPos, 0, 1, 1, this);
	actors.push_back(landmine);
}

void StudentWorld::createPit(double xPos, double yPos)
{
	Actor* pit = new Pit(IID_PIT, xPos, yPos, 0, 0, 1, this);
	actors.push_back(pit);
}

bool StudentWorld::landmineTriggered(Actor* landmine) //protocol called to see if landmine is triggered by an entity 
{
	list<Actor*>::iterator it = actors.begin();
	while (it != actors.end())
	{
		if ((*it)->isEntity())
		{
			if (overlap((*it)->getX(), (*it)->getY(), landmine->getX(), landmine->getY()))
			{
				return true;
			}
		}
		it++;
	}
	return false;
}

void StudentWorld::deadDumbZombie(Actor* zombie)
{
	deadZombie(zombie);
	increaseScore(-1000); //we just called dead zombie, which will provide 2000 points; we just want 1000 so decrement it (for sake of clean code reuse, this was my approach)
	int randDrop = randInt(1, 10); //generate a 1 in 10 chance that a vaccine will drop' if so, fling in random direction (if possible)
	if (randDrop == 1)
	{
		int rand = randInt(1, 4); 
		list<Actor*>::iterator it = actors.begin();
		while (it != actors.begin())
		{
			double x2 = (*it)->getX();
			double y2 = (*it)->getY();
			switch (rand)
			{
			case 1:
			{
				if (overlap(zombie->getX(), zombie->getY() + SPRITE_HEIGHT, x2, y2)) //Here, we check if the vaccine will intersect with anything (including good guys), as according to the spec
				{
					return;
				}
				break;
			}
			case 2:
			{
				if (overlap(zombie->getX(), zombie->getY() - SPRITE_HEIGHT, x2, y2))
				{
					return;
				}
				break;
			}
			case 3:
			{
				if (overlap(zombie->getX() + SPRITE_WIDTH, zombie->getY(), x2, y2))
				{
					return;
				}
				break;
			}
			case 4:
			{
				if (overlap(zombie->getX() - SPRITE_WIDTH, zombie->getY(), x2, y2))
				{
					return;
				}
				break;
			}
			}
			it++;
		}
		switch (rand)
		{
		case 1:
		{
			Actor* vaccine = new VaccineGoodie(IID_VACCINE_GOODIE, zombie->getX(), zombie->getY() + SPRITE_HEIGHT, 0, 1, 1, this);
			actors.push_back(vaccine);
			break;
		}
		case 2:
		{
			Actor* vaccine = new VaccineGoodie(IID_VACCINE_GOODIE, zombie->getX(), zombie->getY() - SPRITE_HEIGHT, 0, 1, 1, this);
			actors.push_back(vaccine);
			break;
		}
		case 3:
		{
			Actor* vaccine = new VaccineGoodie(IID_VACCINE_GOODIE, zombie->getX() + SPRITE_WIDTH, zombie->getY(), 0, 1, 1, this);
			actors.push_back(vaccine);
			break;
		}
		case 4:
		{
			Actor* vaccine = new VaccineGoodie(IID_VACCINE_GOODIE, zombie->getX() - SPRITE_WIDTH, zombie->getY(), 0, 1, 1, this);
			actors.push_back(vaccine);
			break;
		}
		}
	}
}

bool StudentWorld::overlapEnvironment(double x1, double y1)
{
	list<Actor*>::iterator it = actors.begin();
	while (it != actors.end())
	{
		if (overlap(x1, y1, (*it)->getX(), (*it)->getY()) && !(*it)->isEntity() && !(*it)->isGoodie() && !(*it)->isHazard())
			return true;
		it++;
	}
	return false;
}
