#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>

class Actor;
class Avatar;
class Goodie;

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void setDisplayText();

	bool detectCollisions(double x, double y, int dir);
	void detectMarble(double x, double y, int dir);
	bool marbleonPit(double x, double y);

	bool notMarbleorPlayer(double x, double y);

	void addPea(double x, double y, int dir);
	bool peaCollision(double x, double y);

	bool playeronGoodie(double x, double y);
	void playeronExit(double x, double y);

	int getplayerAmmo();

	void eraseActor(double x, double y);
	void eraseMarble(double x, double y);
	void erasePea(double x, double y);
	void eraseGoodie(double x, double y);
	void eraseRobot(double x, double y);

	bool collectedallCrystals();

	bool countThiefBots(double x, double y);
	void addthiefBot(double x, double y, int dir, int imageID);
	void addmeanthiefBot(double x, double y, int dir, int imageID);

	bool canshootPlayer(double x, double y, int dir);

	bool canstealGoodie(double x, double y);
	Goodie* getGoodie(double x, double y);

	Avatar* getAvatar();

	void deadPlayer();
	int getgameOutcome();

	string format(int score, int level, int livesLeft, int health, int ammo, int bonus);

	~StudentWorld();

private:
	vector<Actor*> gameWorld;
	vector<Actor*> newPeas;
	vector<Actor*> newthiefBots;
	vector<Actor*> newmeanthiefBots;
	vector<Actor*> returnedstolenGoodies;

	Avatar* m_avatar;

	int bonus = 1000;
	int ticks;
	int tickMultiplier;
	int gameOutcome = 0;

};

#endif // STUDENTWORLD_H_
