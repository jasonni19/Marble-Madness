#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <math.h>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{
    ticks = (28 - getLevel()) / 4;  // used for thiefbots, mean thiefbots, and ragebots so they don't do something during every tick
    tickMultiplier = 0;

    if (ticks < 3) {
        ticks = 3;
    }

    m_avatar = nullptr;
}

StudentWorld:: ~StudentWorld() {
    cleanUp();
}

Avatar* StudentWorld::getAvatar() {
    return m_avatar;
}

int StudentWorld::init()
{
    string txt;

    Level lev(assetPath());


    if (getLevel() < 10) {
        txt = "level0" + to_string(this->getLevel()) + ".txt";
    }
    else {
        txt = "level" + to_string(this->getLevel()) + ".txt";
    }

    Level::LoadResult result = lev.loadLevel(txt);


    if (result == Level::load_fail_bad_format) {
        return GWSTATUS_LEVEL_ERROR;
    }

    if (result == Level::load_fail_file_not_found || this->getLevel() == 100) {
        return GWSTATUS_PLAYER_WON;
    }

    for (int y = 0; y < VIEW_HEIGHT; y++) {
        for (int x = 0; x < VIEW_WIDTH; x++) {
            Level::MazeEntry item = lev.getContentsOf(y, x);
            // INSERTING ALL ACTORS INTO GAMEWORLD
            switch (item) {
            case Level::player: {
                m_avatar = new Avatar(this, y, x, 0);
                break;
            }

            case Level::wall: {
                Wall* newWall = new Wall(this, y, x, -1);
                gameWorld.push_back(newWall);
                break;
            }
            case Level::exit: {
                Exit* newExit = new Exit(this, y, x, -1);
                newExit->setVisible(false);
                gameWorld.push_back(newExit);
                break;
            }

            case Level::horiz_ragebot: {
                horizontalRageBot* newhorizontalRageBot = new horizontalRageBot(this, y, x, 0);
                gameWorld.push_back(newhorizontalRageBot);
                break;
            }

            case Level::vert_ragebot: {
                verticalRageBot* newverticalRageBot = new verticalRageBot(this, y, x, 2);
                gameWorld.push_back(newverticalRageBot);
                break;
            }

            case Level::thiefbot_factory: {
                thiefBotFactory* newthiefbotfactory = new thiefBotFactory(this, y, x, -1);
                gameWorld.push_back(newthiefbotfactory);
                break;
            }

            case Level::mean_thiefbot_factory: {
                meanthiefBotFactory* newmeanthiefbotfactory = new meanthiefBotFactory(this, y, x, -1);
                gameWorld.push_back(newmeanthiefbotfactory);
                break;
            }

            case Level::marble: {
                Marble* newMarble = new Marble(this, y, x, -1);
                gameWorld.push_back(newMarble);
                break;
            }

            case Level::pit: {
                Pit* newPit = new Pit(this, y, x, -1);
                gameWorld.push_back(newPit);
                break;
            }

            case Level::crystal: {
                Crystal* newCrystal = new Crystal(this, y, x, -1);
                gameWorld.push_back(newCrystal);
                break;
            }

            case Level::restore_health: {
                restoreHealthGoodies* newrestoreHealthGoodies = new restoreHealthGoodies(this, y, x, -1);
                gameWorld.push_back(newrestoreHealthGoodies);
                break;
            }
            case Level::extra_life: {
                extraLifeGoodies* newextraLifeGoodies = new extraLifeGoodies(this, y, x, -1);
                gameWorld.push_back(newextraLifeGoodies);
                break;
            }
            case Level::ammo: {
                ammoGoodies* newammoGoodies = new ammoGoodies(this, y, x, -1);
                gameWorld.push_back(newammoGoodies);
                break;
            }
            default:
                break;

            }
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}


bool StudentWorld::notMarbleorPlayer(double x, double y) {  // returns true if an actor is not a marble or a player, false otherwise
    for (int i = 0; i < gameWorld.size(); i++) {
        if (getAvatar()->getX() == x && getAvatar()->getY() == y) {
            return false;
        }

        if ((gameWorld[i]->isPushable()) && x == gameWorld[i]->getX() && y == gameWorld[i]->getY()) {

            return false;
        }
    }
    return true;
}



bool StudentWorld::detectCollisions(double x, double y, int dir) {

    for (int i = 0; i < gameWorld.size(); i++) {
        // Below is checking for collisions that occur during marble pushing. It checks whether
        // a marble is being pushed into a wall or another marble, robots, factories, pits  (a marble will be blocked by these actors) 
        if (gameWorld[i]->isPushable() && x == gameWorld[i]->getX() && y == gameWorld[i]->getY()) {


            switch (dir) {
            case 180: { // left
                for (int i = 0; i < gameWorld.size(); i++) {
                    if ((gameWorld[i]->isPushable() || gameWorld[i]->isHostile() || gameWorld[i]->unMovable() || gameWorld[i]->isRetrievable()) && x - 1 == (gameWorld[i]->getX()) && y == (gameWorld[i]->getY())) { // marble cannot be pushed if these obstacles are present
                        return true;
                    }
                }
                for (int j = 0; j < gameWorld.size(); j++) { // checking for if a marble is pushed into a pit
                    if ((gameWorld[j]->canbeFilled() && x - 1 == (gameWorld[j]->getX()) && y == (gameWorld[j]->getY()))) {
                        gameWorld[j]->fellinPit(); // marble falls into the pit
                        for (int i = 0; i < gameWorld.size(); i++) {
                            if (gameWorld[i]->isPushable() && x == gameWorld[i]->getX() && y == (gameWorld[i]->getY())) {
                                gameWorld[i]->fellinPit(); // pit is filled with the marble
                            }
                        }


                    }
                }

                break;
            }

            case 0: { // right
                for (int i = 0; i < gameWorld.size(); i++) {
                    if ((gameWorld[i]->isPushable() || gameWorld[i]->unMovable() || gameWorld[i]->isHostile() || gameWorld[i]->isRetrievable()) && x + 1 == (gameWorld[i]->getX()) && y == (gameWorld[i]->getY())) {
                        return true;
                    }
                }

                for (int j = 0; j < gameWorld.size(); j++) {
                    if ((gameWorld[j]->canbeFilled()) && (x + 1 == (gameWorld[j]->getX())) && (y == (gameWorld[j]->getY()))) {
                        gameWorld[j]->fellinPit();
                        for (int i = 0; i < gameWorld.size(); i++) {
                            if (gameWorld[i]->isPushable() && x == gameWorld[i]->getX() && y == (gameWorld[i]->getY())) {
                                gameWorld[i]->fellinPit();
                            }
                        }

                    }
                }
                break;
            }

            case 90: { // up
                for (int i = 0; i < gameWorld.size(); i++) {
                    if ((gameWorld[i]->isPushable() || gameWorld[i]->unMovable() || gameWorld[i]->isHostile() || gameWorld[i]->isRetrievable()) && x == gameWorld[i]->getX() && y + 1 == (gameWorld[i]->getY())) { // added check for isGoodie
                        return true;
                    }
                }

                for (int j = 0; j < gameWorld.size(); j++) {
                    if ((gameWorld[j]->canbeFilled() && x == (gameWorld[j]->getX()) && y + 1 == (gameWorld[j]->getY()))) {
                        gameWorld[j]->fellinPit();
                        for (int i = 0; i < gameWorld.size(); i++) {
                            if (gameWorld[i]->isPushable() && x == gameWorld[i]->getX() && y == (gameWorld[i]->getY())) {
                                gameWorld[i]->fellinPit();
                            }
                        }
                    }


                }
                break;
            }


            case 270: { // down
                for (int i = 0; i < gameWorld.size(); i++) {
                    if ((gameWorld[i]->isPushable() || gameWorld[i]->isHostile() || gameWorld[i]->unMovable() || gameWorld[i]->isRetrievable()) && x == gameWorld[i]->getX() && y - 1 == (gameWorld[i]->getY())) {
                        return true;
                    }
                }
                for (int j = 0; j < gameWorld.size(); j++) {
                    if ((gameWorld[j]->canbeFilled() && x == (gameWorld[j]->getX()) && y - 1 == (gameWorld[j]->getY()))) {
                        gameWorld[j]->fellinPit();
                        for (int i = 0; i < gameWorld.size(); i++) {
                            if (gameWorld[i]->isPushable() && x == gameWorld[i]->getX() && y == (gameWorld[i]->getY())) {
                                gameWorld[i]->fellinPit();
                            }
                        }
                    }
                }
                break;
            }

            }

        }

        if ((gameWorld[i]->initiallyInvisible() && x == gameWorld[i]->getX() && y == gameWorld[i]->getY())) { // checks for marble collision with exit
            return false;
        }

        if ((gameWorld[i]->unMovable() || gameWorld[i]->canbeFilled() || gameWorld[i]->isHostile()) && x == gameWorld[i]->getX() && y == gameWorld[i]->getY() && !(gameWorld[i]->initiallyInvisible())) {
            return true; // Checks for player to wall collision, player to pit collision, player to robot collision

        }
    }
    return false;
}

void StudentWorld::detectMarble(double x, double y, int dir) {
    for (int i = 0; i < gameWorld.size(); i++) {

        if (gameWorld[i]->isPushable() && x == gameWorld[i]->getX() && y == gameWorld[i]->getY()) {  // if the player is in position to push a marble, attempt to push the marble 
            gameWorld[i]->pushMarble(dir);
        }
    }

}

bool StudentWorld::marbleonPit(double x, double y) { // detects when a marble is on a pit
    for (int i = 0; i < gameWorld.size(); i++) {
        if (gameWorld[i]->isPushable()) {
            int marbleX = gameWorld[i]->getX();
            int marbleY = gameWorld[i]->getY();

            // Check if there is a pit at the same position as the marble
            for (int j = 0; j < gameWorld.size(); j++) {
                if (gameWorld[j]->canbeFilled() && gameWorld[j]->getX() == marbleX && gameWorld[j]->getY() == marbleY) {
                    return true; // Found a marble on top of a pit
                }
            }
        }

    }
    return false;
}

int StudentWorld::getplayerAmmo() {

    return (getAvatar()->getpeaCount());

}

void StudentWorld::eraseActor(double x, double y) {
    for (auto it = gameWorld.begin(); it != gameWorld.end(); it++) {
        if ((*it)->getX() == x && (*it)->getY() == y) {
            delete* it;
            it = gameWorld.erase(it);
            it--;
        }
    }
}

void StudentWorld::eraseMarble(double x, double y) {
    for (auto it = gameWorld.begin(); it != gameWorld.end(); it++) {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->isPushable()) {
            delete* it;
            it = gameWorld.erase(it);
            it--;
        }
    }

}

void StudentWorld::erasePea(double x, double y) {
    for (auto it = gameWorld.begin(); it != gameWorld.end(); it++) {
        if ((*it)->getX() == x && (*it)->getY() == y && !(*it)->notFirable()) {
            delete* it;
            it = gameWorld.erase(it);
            it--;
        }
    }

}

void StudentWorld::eraseGoodie(double x, double y) {
    for (auto it = gameWorld.begin(); it != gameWorld.end(); it++) {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->isRetrievable()) {
            delete* it;
            it = gameWorld.erase(it);
            it--;
        }
    }

}

void StudentWorld::eraseRobot(double x, double y) {
    for (auto it = gameWorld.begin(); it != gameWorld.end(); it++) {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->isHostile()) {
            delete* it;
            it = gameWorld.erase(it);
            it--;
        }
    }

}

void StudentWorld::addPea(double x, double y, int dir) { // add a new pea to the gameworld when the user fires a pea
    switch (dir) {
    case 0: {
        Pea* newPea = new Pea(this, x + 1, y, dir);
        newPea->setDirection(0);
        newPeas.push_back(newPea);
        break;
    }
    case 90: {
        Pea* newPea = new Pea(this, x, y + 1, dir);
        newPea->setDirection(90);
        newPeas.push_back(newPea);
        break;
    }
    case 180: {
        Pea* newPea = new Pea(this, x - 1, y, dir);
        newPea->setDirection(180);
        newPeas.push_back(newPea);
        break;
    }
    case 270: {
        Pea* newPea = new Pea(this, x, y - 1, dir);
        newPea->setDirection(270);
        newPeas.push_back(newPea);
        break;
    }
    }
}

bool StudentWorld::peaCollision(double x, double y) { // checks if pea collides with certain actors
    for (int i = 0; i < gameWorld.size(); i++) {
        if (gameWorld[i]->getX() == x && gameWorld[i]->getY() == y && gameWorld[i]->isHostile()) { // if pea collides with any robot
            gameWorld[i]->damaged();
            return true;
        }
    }

    for (int i = 0; i < gameWorld.size(); i++) {
        if (gameWorld[i]->getX() == x && gameWorld[i]->getY() == y) {

            if (gameWorld[i]->peaHittable()) {  // if pea collidies with any object that is hittable by a pea such as wall, factory etc.
                gameWorld[i]->damaged();
                return true;
            }
        }
    }

    if (getAvatar()->getX() == x && getAvatar()->getY() == y) { // if pea collides with the player
        getAvatar()->damaged();
        return true;
    }
    return false;
}


bool StudentWorld::playeronGoodie(double x, double y) { // checks if player is on same square as a goodie
    if (getAvatar()->getX() == x && getAvatar()->getY() == y) {
        playSound(SOUND_GOT_GOODIE);
        return true;
    }
    return false;

}

bool StudentWorld::collectedallCrystals() { // checks if player has collected all the crystals in the game
    for (int i = 0; i < gameWorld.size(); i++) {
        if (gameWorld[i]->collecttoWin()) {
            return false;
        }
    }
    return true;

}

void StudentWorld::playeronExit(double x, double y) { // checks if player is on the same square as the exit
    if (getAvatar()->getX() == x && getAvatar()->getY() == y) {
        playSound(SOUND_FINISHED_LEVEL);
        increaseScore(2000);
        increaseScore(bonus);
        gameOutcome = 1;
    }

}

bool StudentWorld::countThiefBots(double x, double y) { // // countThiefBots of StudentWorld is used to count 
    // how many ThiefBots of either type are present in the square region that extends from itself 3 squares up, 3 squares left through 3 squares down, 3 squares right. Returns true if there is less < 3, false otherwise
    int count = 0;
    bool botonFactory = false;
    for (int i = 0; i < gameWorld.size(); i++) {
        if (gameWorld[i]->getX() == x && gameWorld[i]->getY() == y && gameWorld[i]->stealsItems()) {
            botonFactory = true;
        }
        if (sqrt(pow(gameWorld[i]->getX() - x, 2) + pow(gameWorld[i]->getY() - y, 2)) <= sqrt(18)) { // uses the distance formula to check whether determines whether each thiefbot 
            //is within the specified square region (3 squares in each direction from the center point)
            if (gameWorld[i]->stealsItems()) {
                count++;
            }
        }
    }

    if (count < 3 && !(botonFactory)) { // if count is less than 3 and there is no bot on the factory
        return true;
    }
    return false;
}

void StudentWorld::addthiefBot(double x, double y, int dir, int imageID) { // add a new thiefbot into the gameworld, the thiefbot factory uses this
    thiefBot* newthiefBot = new thiefBot(this, 5, x, y, dir, imageID);
    newthiefBots.push_back(newthiefBot);

}

void StudentWorld::addmeanthiefBot(double x, double y, int dir, int imageID) { // add a new thiefbot into the gameworld, the meanthief bot factory uses this
    meanthiefBot* newmeanthiefBot = new meanthiefBot(this, 8, x, y, dir, imageID);
    newmeanthiefBots.push_back(newmeanthiefBot);

}

bool StudentWorld::canshootPlayer(double x, double y, int dir) { // detects whether a robot is in position to shoot a player or not, for (Horizontal/vertical Ragebots, Mean thiefbots)
    switch (dir) {
    case 0: // right
        if (getAvatar()->getY() == y && getAvatar()->getX() > x) {
            for (int j = 0; j < gameWorld.size(); j++) {
                // checks for collisions, a robot cannot shoot at a player if there is an obstacle such as a marble blocking its shooting range
                if (gameWorld[j]->getY() == y && gameWorld[j]->getX() > x && gameWorld[j]->getX() < getAvatar()->getX() && ((gameWorld[j]->unMovable() || gameWorld[j]->isPushable() || gameWorld[j]->isHostile()))) {

                    return false;
                }

            }

            return true;
        }

        return false;
        break;

    case 270: // down
        if (getAvatar()->getY() < y && getAvatar()->getX() == x) {
            for (int j = 0; j < gameWorld.size(); j++) {
                if (gameWorld[j]->getY() < y && gameWorld[j]->getX() == x && gameWorld[j]->getY() > getAvatar()->getY() && ((gameWorld[j]->unMovable() || gameWorld[j]->isPushable() || gameWorld[j]->isHostile()))) {
                    return false;
                }

            }
            return true;
        }

        return false;

        break;

    case 180: // left
        if (getAvatar()->getY() == y && getAvatar()->getX() < x) {
            for (int j = 0; j < gameWorld.size(); j++) {
                if (gameWorld[j]->getY() == y && gameWorld[j]->getX() < x && gameWorld[j]->getX() > getAvatar()->getX() && ((gameWorld[j]->unMovable() || gameWorld[j]->isPushable() || gameWorld[j]->isHostile()))) {
                    return false;
                }

            }
            return true;
        }


        return false;
        break;

    case 90: // up
        if (getAvatar()->getY() > y && getAvatar()->getX() == x) {
            for (int j = 0; j < gameWorld.size(); j++) {
                if (gameWorld[j]->getY() > y && gameWorld[j]->getX() == x && gameWorld[j]->getY() < getAvatar()->getY() && ((gameWorld[j]->unMovable() || gameWorld[j]->isPushable() || gameWorld[j]->isHostile()))) {
                    return false;
                }

            }
            return true;
        }


        return false;
        break;

    }

    return false;

}


bool StudentWorld::canstealGoodie(double x, double y) { // detects whether a thiefbot is in position to steal a goodie when it is on the same square as a goodie
    for (int i = 0; i < gameWorld.size(); i++) {
        if (!(gameWorld[i]->collecttoWin()) && gameWorld[i]->isRetrievable() && gameWorld[i]->getX() == x && gameWorld[i]->getY() == y) {
            return true;
        }
    }
    return false;
}

Goodie* StudentWorld::getGoodie(double x, double y) { // return a goodie at a specific square
    for (int i = 0; i < gameWorld.size(); i++) {
        if (gameWorld[i]->isRetrievable() && gameWorld[i]->getX() == x && gameWorld[i]->getY() == y) {
            Goodie* goodiePtr = static_cast<Goodie*>(gameWorld[i]);
            return goodiePtr;
        }
    }

    return nullptr;

}

void StudentWorld::deadPlayer() { // called when a player dies 
    decLives();
    gameOutcome = 2;
}

int StudentWorld::getgameOutcome() {
    return gameOutcome;
}


string StudentWorld::format(int score, int level, int lives, int health, int ammo, int bonus) {
    ostringstream oss;

    oss << "Score: ";
    oss.fill('0');
    oss << setw(7) << score;

    oss << "  Level: ";
    oss.fill('0');
    oss << setw(2) << level;

    oss << "  Lives: ";
    oss.fill(' ');
    oss << setw(2) << lives;

    oss << "  Health: ";
    oss.fill(' ');
    oss << setw(3) << health << "%";

    oss << "  Ammo: ";
    oss.fill(' ');
    oss << setw(3) << ammo;

    if (bonus < 0) {
        bonus = 0;
    }

    oss << "  Bonus: ";
    oss.fill(' ');
    oss << setw(4) << bonus;

    string s = oss.str();
    return s;
}

void StudentWorld::setDisplayText() {
    int score = getScore();
    int level = getLevel();
    int livesLeft = getLives();
    double health = ((getAvatar()->getHp()) / 20.0) * 100;
    int ammo = getplayerAmmo();

    string s = format(score, level, livesLeft, health, ammo, bonus);
    setGameStatText(s);

}

int StudentWorld::move()
{
    setDisplayText();

    for (int i = 0; i < gameWorld.size(); i++) {
        if (gameWorld[i]->isHostile()) {  // so robots don't do something during every tick
            if ((tickMultiplier % ticks == 0) && (tickMultiplier != 0)) {
                gameWorld[i]->doSomething();
            }
        }
        else {
            gameWorld[i]->doSomething();
        }
    }

    tickMultiplier++;

    m_avatar->doSomething();

    gameWorld.insert(gameWorld.end(), newPeas.begin(), newPeas.end());
    newPeas.clear();

    gameWorld.insert(gameWorld.end(), newthiefBots.begin(), newthiefBots.end());
    newthiefBots.clear();

    gameWorld.insert(gameWorld.end(), newmeanthiefBots.begin(), newmeanthiefBots.end());
    newmeanthiefBots.clear();

    gameWorld.insert(gameWorld.end(), returnedstolenGoodies.begin(), returnedstolenGoodies.end());
    returnedstolenGoodies.clear();

    bonus -= 1;

    if (getgameOutcome() == 1) { // player finishes level
        gameOutcome = 0;
        bonus = 1000;
        return GWSTATUS_FINISHED_LEVEL;
    }
    else if (getgameOutcome() == 2) { // player dies
        gameOutcome = 0;
        bonus = 1000;
        return GWSTATUS_PLAYER_DIED;
    }

    return GWSTATUS_CONTINUE_GAME; // game continues
}

void StudentWorld::cleanUp() // called by game framework after the player loses a life
{
    for (Actor* ptr : gameWorld) {
        if (ptr != nullptr) {
            delete ptr;
        }
    }

    delete m_avatar;
    m_avatar = nullptr;

    gameWorld.clear();
}

