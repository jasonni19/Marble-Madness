#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Level.h"
#include "StudentWorld.h"

// MAYBE MAKE AN ABSTRACT BASE CLASS CALLED class obstacles to store Wall, pits, factories etc.
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject { // abstract base class
public:
	Actor(int id, StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething() = 0; // pure virtual function, this will manage what each actor during each tick

	virtual bool collecttoWin();
	virtual bool isRetrievable();
	virtual bool isHostile();

	virtual bool unMovable(); // to detect whether an actor will cause collision and can not be moved ex. walls, pits
	virtual bool isPushable(); // to detect whether an actor can be pushed
	virtual bool multipleLives();  // to detect whether an actor has multiple lives
	virtual bool canbeFilled();	   // to detect whether an actor can be filled (pits)

	virtual bool initiallyInvisible(); // to detect whether an actor is initially invisible or not
	virtual bool getV();
	virtual void setV(bool v);

	virtual bool peaHittable();  // to detect whether an actor is hittable by a pea
	virtual void damaged() = 0;  // pure virtual function, this will manage all ways each actor takes damage
	virtual bool notFirable();  // to detect wher an actor can be fired or not
	virtual int getpeaCount();  // to return the amount of peas or ammo

	void setliveStatus(bool status);
	bool getliveStatus();

	virtual void pushMarble(int dir);
	virtual void fellinPit();

	virtual bool stealsItems(); // to detect whether an actor can steal goodies

	StudentWorld* getWorld();

private:
	bool alive = true;
	StudentWorld* world;

};

class attackableActor : public Actor { // abstract base class
public:
	attackableActor(int id, int hp, StudentWorld* ptr, double x, double y, int dir);
	int getHp();
	void setHp(int h);

private:
	int hitpoints;

};

class Goodie : public Actor { // Abstract Base Class
public:
	Goodie(int id, StudentWorld* ptr, double x, double y, int dir);
	virtual void pickupGoodie() = 0;
	virtual bool isRetrievable();
	virtual bool peaHittable();

	bool getpickupStatus();
	void setpickupStatus(bool b);

	void deadGoodie();

private:
	bool canpickUp = true;
};

class Factory : public Actor { // Abstract Base Class
public:
	Factory(int id, StudentWorld* ptr, double x, double y, int dir);
	virtual bool unMovable();

};

class Avatar : public attackableActor {
public:
	Avatar(StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething();
	virtual void damaged();
	virtual int getpeaCount();
	void setpeaCount(int peas);
	virtual bool multipleLives();

	virtual bool peaHittable();

private:
	int peaCount;
	StudentWorld* world;

};

class Wall : public Actor {
public:
	Wall(StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething();
	virtual bool unMovable();
	virtual void damaged();

private:
	StudentWorld* world;

};

class Marble : public attackableActor {
public:
	Marble(StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething();
	virtual void damaged();
	virtual void pushMarble(int dir);
	virtual bool isPushable();
	virtual void fellinPit();

private:
	StudentWorld* world;

};

class Pit : public attackableActor {
public:
	Pit(StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething();
	virtual void damaged();
	virtual bool canbeFilled();
	virtual void fellinPit();
	virtual bool peaHittable();

private:
	StudentWorld* world;

};

class Pea : public attackableActor {
public:
	Pea(StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething();
	virtual void damaged();
	void peaShoot(double x, double y, int dir);
	virtual bool notFirable();
	virtual bool peaHittable();

private:
	StudentWorld* world;

};

class Crystal : public Goodie {
public:
	Crystal(StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething();
	virtual void damaged();
	virtual void pickupGoodie();
	virtual bool collecttoWin();

private:
	StudentWorld* world;
};

class Exit : public Actor { // should initially be invisible
public:
	Exit(StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething();
	virtual bool unMovable();
	virtual void damaged();

	virtual bool getF();
	virtual void setF(bool f);

	virtual bool peaHittable();
	virtual bool initiallyInvisible();

private:
	StudentWorld* world;
	bool firstOccurence = true;
};

class extraLifeGoodies : public Goodie {
public:
	extraLifeGoodies(StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething();
	virtual void damaged();
	virtual void pickupGoodie();

private:
	StudentWorld* world;
};

class restoreHealthGoodies : public Goodie {
public:
	restoreHealthGoodies(StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething();
	virtual void damaged();
	virtual void pickupGoodie();

private:
	StudentWorld* world;
};

class ammoGoodies : public Goodie {
public:
	ammoGoodies(StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething();
	virtual void damaged();
	virtual void pickupGoodie();

private:
	StudentWorld* world;
};


class thiefBot : public attackableActor {
public:
	thiefBot(StudentWorld* ptr, int hp, double x, double y, int dir, int imageID);
	virtual void doSomething();
	virtual void damaged();
	virtual bool stealsItems();
	virtual bool isHostile();
	virtual void dropGoodie(Goodie* g);
	void thiefbotMove();

	void resetthiefDirection();
	void setdontchangeDirection(bool b);
	void setdontmove(bool b);

	void stealGoodie(Goodie* g);

private:
	int distanceBeforeTurning;
	bool pickedupGoodie;
	bool firstMove;
	int distanceTraveled;
	int currentDirection;
	StudentWorld* world;
	Goodie* stolen;

	bool dontchangeDirection = false;
	bool dontmove = false;


};


class meanthiefBot : public thiefBot {
public:
	meanthiefBot(StudentWorld* ptr, int hp, double x, double y, int dir, int imageID);
	virtual void doSomething();
	virtual void damaged();
	virtual bool stealsItems();
	virtual bool isHostile();

private:
	bool pickedupGoodie;
	StudentWorld* world;

};

class thiefBotFactory : public Factory {
public:
	thiefBotFactory(StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething();
	virtual void damaged();

private:
	StudentWorld* world;
};


class meanthiefBotFactory : public Factory {
public:
	meanthiefBotFactory(StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething();
	virtual void damaged();

private:
	StudentWorld* world;
};


class horizontalRageBot : public attackableActor {
public:
	horizontalRageBot(StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething();
	virtual void damaged();

	void setcurrentDirection(int dir);
	int getcurrentDirection();

	void resetragedDirection(int dir);
	void setdontmove(bool b);

	void ragebotMove(int dir);

	virtual bool isHostile();
private:
	StudentWorld* world;
	bool dontmove = false;
	int currentDirection;

};


class verticalRageBot : public horizontalRageBot {
public:
	verticalRageBot(StudentWorld* ptr, double x, double y, int dir);
	virtual void doSomething();
	virtual void damaged();

private:
	StudentWorld* world;

};


#endif // ACTOR_H_
