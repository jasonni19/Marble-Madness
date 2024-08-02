#include "Actor.h"
#include <cstdlib>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR (and all Abstract Base Classes):
Actor::Actor(int id, StudentWorld* ptr, double x, double y, int dir) : GraphObject(id, x, y, dir), world(ptr) {}

StudentWorld* Actor::getWorld() {
	return world;
}

bool Actor::getliveStatus() {
	return alive;
}

void Actor::setliveStatus(bool status) {
	alive = status;
}

bool Actor::unMovable() {
	return false;
}

bool Actor::isPushable() {
	return false;
}

bool Actor::canbeFilled() {
	return false;
}

bool Actor::notFirable() {
	return true;
}

bool Actor::isRetrievable() {
	return false;
}

bool Actor::collecttoWin() {
	return false;
}

bool Actor::peaHittable() {
	return true;
}

bool Actor::stealsItems() {
	return false;
}

bool Actor::initiallyInvisible() {
	return false;
}

bool Actor::isHostile() {
	return false;
}

void Actor::pushMarble(int dir) {

}

void Actor::fellinPit() {

}

int Actor::getpeaCount() {
	return -1;
}

bool Actor::getV() {
	return true;
}

void Actor::setV(bool v) {
	return;
}

bool Actor::multipleLives() {
	return false;
}


// attackableActor: (Abstract Base Class)
attackableActor::attackableActor(int id, int hp, StudentWorld* ptr, double x, double y, int dir) : Actor(id, ptr, x, y, dir), hitpoints(hp) {}

int attackableActor::getHp() {
	return hitpoints;
}

void attackableActor::setHp(int h) {
	hitpoints = h;
}

// GOODIE: (Abstract Base Class)
Goodie::Goodie(int id, StudentWorld* ptr, double x, double y, int dir) : Actor(id, ptr, x, y, dir) {}

bool Goodie::isRetrievable() {
	return true;
}

bool Goodie::peaHittable() {
	return false;
}

void Goodie::deadGoodie() {
	getWorld()->eraseGoodie(getX(), getY());
	setliveStatus(false);
}

bool Goodie::getpickupStatus() {  // returns the pickup status of a goodie, this tells the program whether a goodie can be picked up or not
	return canpickUp;
}

void Goodie::setpickupStatus(bool b) {  // changes the pickup status, for instance if a goodie is picked up then its pickup status ic changed to false so it can't be picked up again
	canpickUp = b;
}

// Factory: (Abstract Base Class)
Factory::Factory(int id, StudentWorld* ptr, double x, double y, int dir) : Actor(id, ptr, x, y, dir) {}

bool Factory::unMovable() {
	return true;
}


// AVATAR:
Avatar::Avatar(StudentWorld* ptr, double x, double y, int dir) : attackableActor(IID_PLAYER, 20, ptr, x, y, right), peaCount(20), world(ptr)
{

}

void Avatar::damaged() {
	setHp(getHp() - 2);

	if (getHp() > 0) {
		getWorld()->playSound(SOUND_PLAYER_IMPACT);
		setliveStatus(true);
	}
	else {
		setliveStatus(false);
		getWorld()->playSound(SOUND_PLAYER_DIE);
		getWorld()->deadPlayer();
	}
}

void Avatar::doSomething() {
	if (!(getliveStatus())) {
		return;
	}

	int ch;
	if (getWorld()->getKey(ch))
	{
		// user hit a key this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT: // avatar tries to move left
			if (!(getWorld()->detectCollisions(getX() - 1, getY(), 180))) {
				setDirection(left);
				moveTo(getX() - 1, getY());
				getWorld()->detectMarble(getX(), getY(), left);
			}
			break;

		case KEY_PRESS_RIGHT: // avatar tries to move right
			if (!(getWorld()->detectCollisions(getX() + 1, getY(), 0))) {
				setDirection(right);
				moveTo(getX() + 1, getY());
				getWorld()->detectMarble(getX(), getY(), right);
			}
			break;

		case KEY_PRESS_UP: // avatar tries to move up
			if (!(getWorld()->detectCollisions(getX(), getY() + 1, 90))) {
				setDirection(up);
				moveTo(getX(), getY() + 1);
				getWorld()->detectMarble(getX(), getY(), up);
			}
			break;

		case KEY_PRESS_DOWN: // avatar tries to moves down
			if (!(getWorld()->detectCollisions(getX(), getY() - 1, 270))) {
				setDirection(down);
				moveTo(getX(), getY() - 1);
				getWorld()->detectMarble(getX(), getY(), down);

			}
			break;

		case KEY_PRESS_ESCAPE:  // user quits a current life
			setliveStatus(false);
			getWorld()->playSound(SOUND_PLAYER_DIE);
			getWorld()->deadPlayer();
			break;

		case KEY_PRESS_SPACE:  // fire a pea
			if (peaCount > 0) {
				getWorld()->addPea(getX(), getY(), getDirection());
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				peaCount--;

				break;

			}

		}
	}

}

bool Avatar::peaHittable() {
	return true;
}

int Avatar::getpeaCount() {
	return peaCount;
}

void Avatar::setpeaCount(int peas) {  // used to change the amount of peas or ammo a player has
	peaCount += peas;
}

bool Avatar::multipleLives() {
	return true;
}

// WALL:
Wall::Wall(StudentWorld* ptr, double x, double y, int dir) : Actor(IID_WALL, ptr, x, y, none), world(ptr) {

}

void Wall::doSomething() { // Wall does nothing during each tick

}

bool Wall::unMovable() {
	return true;
}
void Wall::damaged() { // Wall takes no damage

}

// MARBLE:
Marble::Marble(StudentWorld* ptr, double x, double y, int dir) : attackableActor(IID_MARBLE, 10, ptr, x, y, none), world(ptr) {

}

void Marble::doSomething() { // Marble does nothing during each tick
	return;
}

void Marble::fellinPit() {    // this is called when a marble falls in a pit (the marble gets erased)
	setVisible(false);
	getWorld()->eraseMarble(getX(), getY());
	setliveStatus(false);

}

void Marble::damaged() {
	setHp(getHp() - 2);

	if (getHp() <= 0) {
		getWorld()->eraseMarble(getX(), getY());
		setliveStatus(false);
	}
	else {
		setliveStatus(true);
	}

}

bool Marble::isPushable() {
	return true;
}

void Marble::pushMarble(int dir) { // dir is the players direction, the marble is attempted to be pushed in the direction the player is currently facing. This calls StudentWorld's detectCollision function
	int pushDir = dir;
	switch (pushDir)
	{
	case left: {
		if (!(getWorld()->detectCollisions(getX() - 1, getY(), 180))) {
			moveTo(getX() - 1, getY());
		}
		break;
	}
	case right: {
		if (!(getWorld()->detectCollisions(getX() + 1, getY(), 0))) {
			moveTo(getX() + 1, getY());
		}
		break;
	}
	case up: {
		if (!(getWorld()->detectCollisions(getX(), getY() + 1, 90))) {
			moveTo(getX(), getY() + 1);
		}
		break;

	}
	case down: {
		if (!(getWorld()->detectCollisions(getX(), getY() - 1, 270))) {
			moveTo(getX(), getY() - 1);
		}
		break;
	}
	}

}

// PIT:
Pit::Pit(StudentWorld* ptr, double x, double y, int dir) : attackableActor(IID_PIT, -1, ptr, x, y, none), world(ptr) {}

void Pit::doSomething() {
	if (!(getliveStatus())) {
		return;
	}
}

bool Pit::canbeFilled() {
	return true;
}

void Pit::fellinPit() {
	setliveStatus(false);
	setVisible(false);
	getWorld()->eraseActor(getX(), getY()); // remove the pit if a marble falls into it

}

void Pit::damaged() { // Pit takes no damage
	return;
}

bool Pit::peaHittable() {
	return false;
}

// PEA:
Pea::Pea(StudentWorld* ptr, double x, double y, int dir) : attackableActor(IID_PEA, -1, ptr, x, y, none), world(ptr) {}

void Pea::doSomething() {  // If a pea is shot and created, the pea moves across the map during each tick
	if (!(getliveStatus())) {
		return;
	}
	peaShoot(getX(), getY(), getDirection());

}

void Pea::peaShoot(double x, double y, int dir) {  // handles the shooting of the pea
	if (getWorld()->peaCollision(getX(), getY())) { // if a pea collides with an obsatcle at current square, call the damage function which kills the pea
		damaged();
	}
	else {
		switch (dir)
		{
		case left: {  // pea is shot left
			moveTo(getX() - 1, getY());

			if (getWorld()->peaCollision(getX(), getY())) {
				damaged(); // return true if sucessful collison, then kill the pea
			}
			break;
		}
		case right: {  // pea is shot right
			moveTo(getX() + 1, getY());
			if (getWorld()->peaCollision(getX(), getY())) {
				damaged();
			}
			break;
		}
		case up: { // pea is shot up
			moveTo(getX(), getY() + 1);
			if (getWorld()->peaCollision(getX(), getY())) {
				damaged();
			}
			break;

		}
		case down: { // pea is shot down
			moveTo(getX(), getY() - 1);

			if (getWorld()->peaCollision(getX(), getY())) {
				damaged();

			}
			break;


		}
		}
	}

}

bool Pea::notFirable() {
	return false;
}

bool Pea::peaHittable() {
	return false;
}

void Pea::damaged() {
	getWorld()->erasePea(getX(), getY());
	setliveStatus(false);
}

// CRYSTAL:

Crystal::Crystal(StudentWorld* ptr, double x, double y, int dir) : Goodie(IID_CRYSTAL, ptr, x, y, none), world(ptr) {}

void Crystal::doSomething() {   // pickupGoodie is called to check whether the player is on the same square as a crystal and can pick it up
	if (!(getliveStatus())) {
		return;
	}
	pickupGoodie();

}

void Crystal::damaged() {
	return;
}

void Crystal::pickupGoodie() { // 
	if (getWorld()->playeronGoodie(getX(), getY())) { // if player successfully picks up crystal
		getWorld()->increaseScore(50);
		deadGoodie();
	}

}

bool Crystal::collecttoWin() {
	return true;
}

// EXIT:
Exit::Exit(StudentWorld* ptr, double x, double y, int dir) : Actor(IID_EXIT, ptr, x, y, none), world(ptr) {}

bool Exit::unMovable() {
	return true;
}

void Exit::doSomething() { // if the player has collected all the crystals, exit is revealed.
	if (getWorld()->collectedallCrystals()) {
		if (getF()) {
			getWorld()->playSound(SOUND_REVEAL_EXIT);
		}
		setVisible(true);
		setF(false);

		getWorld()->playeronExit(getX(), getY());

	}
}

void Exit::damaged() {
	return;
}

bool Exit::initiallyInvisible() {
	return true;
}

bool Exit::getF() {
	return firstOccurence;
}

void Exit::setF(bool f) {
	firstOccurence = f;
}


bool Exit::peaHittable() {
	return false;
}

// EXTRALIFE GOODIE:
extraLifeGoodies::extraLifeGoodies(StudentWorld* ptr, double x, double y, int dir) : Goodie(IID_EXTRA_LIFE, ptr, x, y, none), world(ptr) {}

void extraLifeGoodies::doSomething() {
	if (!getliveStatus()) {
		return;
	}
	pickupGoodie();
}

void extraLifeGoodies::damaged() {
	return;
}

void extraLifeGoodies::pickupGoodie() {
	if (getpickupStatus() && getWorld()->playeronGoodie(getX(), getY())) { // if player successfully picks up extraLifeGoodie
		getWorld()->increaseScore(1000);
		getWorld()->incLives();
		deadGoodie();
	}
}

// RESTORELIFE GOODIE
restoreHealthGoodies::restoreHealthGoodies(StudentWorld* ptr, double x, double y, int dir) : Goodie(IID_RESTORE_HEALTH, ptr, x, y, none), world(ptr) {}

void restoreHealthGoodies::doSomething() {
	if (!getliveStatus()) {
		return;
	}
	pickupGoodie();
}

void restoreHealthGoodies::damaged() {
	return;
}

void restoreHealthGoodies::pickupGoodie() {  // if player successfully picks up restoreHealthGoodie
	if (getpickupStatus() && getWorld()->playeronGoodie(getX(), getY())) {
		getWorld()->increaseScore(500);
		(getWorld()->getAvatar())->setHp(20);
		deadGoodie();
	}
}

// AMMO GOODIE:
ammoGoodies::ammoGoodies(StudentWorld* ptr, double x, double y, int dir) : Goodie(IID_AMMO, ptr, x, y, none), world(ptr) {}

void ammoGoodies::doSomething() {
	if (!getliveStatus()) {
		return;
	}
	pickupGoodie();
}

void ammoGoodies::damaged() {
	return;
}

void ammoGoodies::pickupGoodie() { // if player successfully picks up ammoGoodie
	if (getpickupStatus() && getWorld()->playeronGoodie(getX(), getY())) {

		getWorld()->increaseScore(100);
		(getWorld()->getAvatar())->setpeaCount(20);
		deadGoodie();
	}
}

// THIEFBOT FACTORY:
thiefBotFactory::thiefBotFactory(StudentWorld* ptr, double x, double y, int dir) : Factory(IID_ROBOT_FACTORY, ptr, x, y, none), world(ptr) {}

void thiefBotFactory::doSomething() {
	if (getWorld()->countThiefBots(getX(), getY())) {	// countThiefBots of StudentWorld is used to count how many ThiefBots of either type are present in the square region that extends from itself 3 squares up, 3 squares left through 3 squares down, 3 squares right. Returns true if there is less < 3, false otherwise
		int randomNumber = rand() % 50 + 1;

		if (randomNumber == 1) {
			getWorld()->addthiefBot(getX(), getY(), 0, IID_THIEFBOT);
			getWorld()->playSound(SOUND_ROBOT_BORN);
		}
	}
	return;
}


void thiefBotFactory::damaged() {
	return;
}

// MEANTHIEFBOT FACTORY:
meanthiefBotFactory::meanthiefBotFactory(StudentWorld* ptr, double x, double y, int dir) : Factory(IID_ROBOT_FACTORY, ptr, x, y, none), world(ptr) {}

void meanthiefBotFactory::doSomething() { // behaves just like thiefBotFactory, except it spawns mean thiefbots
	if (getWorld()->countThiefBots(getX(), getY())) {
		int randomNumber = rand() % 50 + 1;

		if (randomNumber == 2) {
			getWorld()->addmeanthiefBot(getX(), getY(), 0, IID_MEAN_THIEFBOT);
			getWorld()->playSound(SOUND_ROBOT_BORN);
		}

	}
	return;
}

void meanthiefBotFactory::damaged() {
	return;
}

// THIEFBOT:
thiefBot::thiefBot(StudentWorld* ptr, int hp, double x, double y, int dir, int imageID) : attackableActor(imageID, 5, ptr, x, y, 0), world(ptr) {
	int random = rand() % 6 + 1;
	distanceBeforeTurning = random;
	pickedupGoodie = false;
	firstMove = true;
	distanceTraveled = 0;
	currentDirection = right;
	stolen = nullptr;

}

void thiefBot::doSomething() {
	if (!(getliveStatus())) {
		getWorld()->eraseRobot(getX(), getY());
		return;
	}

	if (getWorld()->canstealGoodie(getX(), getY()) && !(pickedupGoodie)) { // if a thiefbot can steal a goodie and has not picked up a goodie already
		int randomNumber = rand() % 10 + 1;
		if (randomNumber == 7) {

			stealGoodie(getWorld()->getGoodie(getX(), getY()));
			stolen = getWorld()->getGoodie(getX(), getY());

			getWorld()->playSound(SOUND_ROBOT_MUNCH);
			pickedupGoodie = true;
		}
		else {
			thiefbotMove();  // if a thiefbot does not steal a goodie, it moves around
		}
	}

	else {
		thiefbotMove(); // if a thiefbot does not steal a goodie, it moves around
	}

}

void thiefBot::resetthiefDirection() { // used to change a thiefbot's direction each time it encounters an obstacle in its current direction or if it has traveled distanceBeforeTurning steps in its current direction
	if (!dontchangeDirection) {
		currentDirection = rand() % 4 + 1;
		switch (currentDirection) {
		case 1:
			setDirection(0);
			break;
		case 2:
			setDirection(180);
			break;

		case 3:
			setDirection(90);
			break;

		case 4:
			setDirection(270);
			break;
		}

		distanceTraveled = 0;
	}
}

void thiefBot::thiefbotMove() { // MOVEMENT FOR THIEFBOTS, they always start moving to the right
	if (!(dontmove)) {
		if (firstMove) {
			currentDirection = 1;
			if (getWorld()->notMarbleorPlayer(getX() + 1, getY())) {
				if (!(getWorld()->detectCollisions(getX() + 1, getY(), 0))) {
					moveTo(getX() + 1, getY());
					distanceTraveled++;
				}

			}
			firstMove = false;
		}
		else {
			// Check if the ThiefBot has traveled a certain distance before turning
			if (distanceTraveled < distanceBeforeTurning) {

				// Try to move in the current direction
				switch (currentDirection) {
				case 1:

					if (getWorld()->notMarbleorPlayer(getX() + 1, getY())) {
						if (!(getWorld()->detectCollisions(getX() + 1, getY(), 0))) {
							moveTo(getX() + 1, getY());
							distanceTraveled++;
						}
						else {

							resetthiefDirection();


						}
					}
					else {
						resetthiefDirection();
						break;
					}

					break;

				case 2:

					if (getWorld()->notMarbleorPlayer(getX() - 1, getY())) {
						if (!(getWorld()->detectCollisions(getX() - 1, getY(), 180)) && getWorld()->notMarbleorPlayer(getX() - 1, getY())) {
							moveTo(getX() - 1, getY());
							distanceTraveled++;
						}
						else {
							resetthiefDirection();

						}
					}
					else {

						resetthiefDirection();
						break;
					}

					break;

				case 3:

					if (getWorld()->notMarbleorPlayer(getX(), getY() + 1)) {
						if (!(getWorld()->detectCollisions(getX(), getY() + 1, 90)) && getWorld()->notMarbleorPlayer(getX(), getY() + 1)) {
							moveTo(getX(), getY() + 1);
							distanceTraveled++;
						}
						else {
							resetthiefDirection();

						}
					}
					else {

						resetthiefDirection();
						break;
					}

					break;

				case 4:

					if (getWorld()->notMarbleorPlayer(getX(), getY() - 1)) {
						if (!(getWorld()->detectCollisions(getX(), getY() - 1, 270)) && getWorld()->notMarbleorPlayer(getX(), getY() - 1)) {
							moveTo(getX(), getY() - 1);
							distanceTraveled++;

						}
						else {
							resetthiefDirection();

						}

					}
					else {

						resetthiefDirection();
						break;
					}

					break;
				}
			}
			else {
				// Reset distanceTraveled and choose a new random direction
				resetthiefDirection();

			}
		}
	}
}


void thiefBot::damaged() {
	if (getHp() > 0) {
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	}

	setHp(getHp() - 2);

	if (getHp() <= 0) {
		if (pickedupGoodie) {  // if a thiefbot dies and picked up a goodie prior, that goodie will be dropped on the square where the thiefbot died
			dropGoodie(stolen);

		}

		setliveStatus(false);
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(10);

	}

	return;
}

void thiefBot::stealGoodie(Goodie* goodie) {  // the goodie that the thiefbot steals is set invisible and now cannot be picked up
	goodie->setVisible(false);
	goodie->setpickupStatus(false);

}

void thiefBot::dropGoodie(Goodie* goodie) {  // when the thiefbot dies and drops the goodie, the goodie is moved to the square where the thiefbot has died; it is now visible and can be picked up
	goodie->moveTo(getX(), getY());
	goodie->setpickupStatus(true);
	goodie->setVisible(true);

}

bool thiefBot::stealsItems() {
	return true;
}

bool thiefBot::isHostile() {
	return true;
}


void thiefBot::setdontchangeDirection(bool b) {
	dontchangeDirection = b;
}

void thiefBot::setdontmove(bool b) {
	dontmove = b;
}

// MEANTHIEFBOT:

meanthiefBot::meanthiefBot(StudentWorld* ptr, int hp, double x, double y, int dir, int imageID) : world(ptr), thiefBot(ptr, 8, x, y, 0, imageID) {
	pickedupGoodie = false;
}

void meanthiefBot::doSomething() {
	if (getWorld()->canshootPlayer(getX(), getY(), getDirection())) {
		setdontchangeDirection(true);
		setdontmove(true);

		switch (getDirection()) {
		case 0:
			getWorld()->addPea(getX(), getY(), 0);
			break;
		case 180:
			getWorld()->addPea(getX(), getY(), 180);
			break;
		case 90:
			getWorld()->addPea(getX(), getY(), 90);
			break;
		case 270:
			getWorld()->addPea(getX(), getY(), 270);
			break;
		}

		getWorld()->playSound(SOUND_ENEMY_FIRE);
	}
	else {

		setdontchangeDirection(false);
		setdontmove(false);
	}

	thiefBot::doSomething();

	return;
}

void meanthiefBot::damaged() {
	thiefBot::damaged();
	if (getHp() <= 0) {
		getWorld()->increaseScore(10);
	}

	return;
}

bool meanthiefBot::stealsItems() {
	return true;
}

bool meanthiefBot::isHostile() {
	return true;
}

// HORIZONTAL RAGE BOT:

horizontalRageBot::horizontalRageBot(StudentWorld* ptr, double x, double y, int dir) : attackableActor(IID_RAGEBOT, 10, ptr, x, y, 0), world(ptr), currentDirection(dir) {}

void horizontalRageBot::doSomething() {
	if (!(getliveStatus())) {
		getWorld()->eraseRobot(getX(), getY());
		return;
	}
	if (getWorld()->canshootPlayer(getX(), getY(), getDirection())) {  // if the player is in the range of a horizontal rage bot, it fires a pea at the player and stops moving
		setdontmove(true);

		switch (getDirection()) {
		case 0:
			getWorld()->addPea(getX(), getY(), 0);
			break;
		case 180:
			getWorld()->addPea(getX(), getY(), 180);
			break;
		case 90:
			getWorld()->addPea(getX(), getY(), 90);
			break;
		case 270:
			getWorld()->addPea(getX(), getY(), 270);
			break;
		}

		getWorld()->playSound(SOUND_ENEMY_FIRE);
	}
	else {
		setdontmove(false);  // continue moving if the player is not in the horizontal ragebot's range
		ragebotMove(getcurrentDirection());
	}
}

void horizontalRageBot::ragebotMove(int dir) {  // manages how the horizontalRageBot moves
	if (!dontmove) {
		switch (dir) {
		case 0: // moves to the right
			if (getWorld()->notMarbleorPlayer(getX() + 1, getY())) {
				if (!(getWorld()->detectCollisions(getX() + 1, getY(), 0))) {
					moveTo(getX() + 1, getY());
				}
				else {
					resetragedDirection(dir); // if it can't move to the right, it now moves to the left after this call

				}
			}
			else {
				resetragedDirection(dir);

			}

			break;

		case 90: // moves up
			if (getWorld()->notMarbleorPlayer(getX(), getY() + 1)) {
				if (!(getWorld()->detectCollisions(getX(), getY() + 1, 90))) {
					moveTo(getX(), getY() + 1);
				}
				else {
					resetragedDirection(dir); // if it can't move up, it now moves down after this call


				}
			}
			else {
				resetragedDirection(dir);

			}

			break;

		case 180: // moves left
			if (getWorld()->notMarbleorPlayer(getX() - 1, getY())) {
				if (!(getWorld()->detectCollisions(getX() - 1, getY(), 180))) {
					moveTo(getX() - 1, getY());
				}
				else {
					resetragedDirection(dir); // if it can't move left, it now moves right after this call


				}
			}
			else {
				resetragedDirection(dir);

			}

			break;

		case 270: // moves down
			if (getWorld()->notMarbleorPlayer(getX(), getY() - 1)) {
				if (!(getWorld()->detectCollisions(getX(), getY() - 1, 270))) {
					moveTo(getX(), getY() - 1);
				}
				else {
					resetragedDirection(dir); // if it can't move down, it now moves up after this call

				}
			}
			else {
				resetragedDirection(dir);

			}

			break;

		}
	}
}

void horizontalRageBot::damaged() {
	setHp(getHp() - 2);
	if (getHp() > 0) {
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	}

	if (getHp() <= 0) {
		setliveStatus(false);
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(100);

	}
	return;
}

bool horizontalRageBot::isHostile() {
	return true;
}

void horizontalRageBot::resetragedDirection(int dir) {
	switch (dir) {
	case 0:
		setcurrentDirection(180);
		setDirection(180);
		break;

	case 90:
		setcurrentDirection(270);
		setDirection(270);
		break;

	case 180:
		setcurrentDirection(0);
		setDirection(0);
		break;

	case 270:
		setcurrentDirection(90);
		setDirection(90);
		break;

	}
}

void horizontalRageBot::setdontmove(bool b) {
	dontmove = b;
}

void horizontalRageBot::setcurrentDirection(int dir) {
	currentDirection = dir;
}

int horizontalRageBot::getcurrentDirection() {
	return currentDirection;
}

// VERTICAL RAGE BOT:    (same implementations as horizontalRageBot except that its starting direction is down)

verticalRageBot::verticalRageBot(StudentWorld* ptr, double x, double y, int dir) : horizontalRageBot(ptr, x, y, 270), world(ptr) {
	setcurrentDirection(270);
	setDirection(270);
}

void verticalRageBot::doSomething() {
	horizontalRageBot::doSomething();
}

void verticalRageBot::damaged() {
	horizontalRageBot::damaged();
}
