#include "Game.h"
#include "Coin.h"
#include "MonsterCreature.h"
#include "Trigger.h"

/**
 * Game implementation
 */

/**
 * The default constructor.
 * Sets gameState to menu.
 * @see GameState
 */
Game::Game():
	gameState(GameState::Menu),
	posUpdated(false),
	levelWidth(0),
	levelHeight(0),
    levelCoins(0),
	levelLoaded(false),
	posAlpha(1.0)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Game created!");
}

/**
 * The default destructor.
 * Unloads level.
 */
Game::~Game()
{
	unloadLevel();
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Game destroyed!");
}

/**
 * Updates current state of game.
 * @return void
 */
void Game::gameLoop()
{
    posUpdated = false;

    if (gameState == GameState::Playing)
    {

        //move everything
        std::list<Controller*>::iterator it;
        for (it = controllerList.begin(); it != controllerList.end(); ++it)
        {
            if ((*it)->getCreature()->getY() > levelHeight)
            {
                (*it)->getCreature()->hurt(127);
            }
            if (!(*it)->getCreature()->getIsAlive()) //hide corpses in the closet
            {
                posUpdated = true;
                PlayerController* tmpPC = dynamic_cast<PlayerController*>(*it);
                if (tmpPC != nullptr)
                    gameState = GameState::Lost;
                (*it)->getCreature()->destroy();
                delete (*it);
                controllerList.erase(it);
                it = controllerList.begin();
                if (it == controllerList.end())
                    break;
                continue;
            }
            (*it)->control();
        }
        //clear destroyed objects
        for (std::list<Object*>::iterator it = objectList.begin(); it != objectList.end(); ++it)
        {
            if ((*it)->getDestroyed())
            {
                delete (*it);
                objectList.erase(it);
                it = objectList.begin();
                if (it == objectList.end())
                    break;
            }
        }
        //untrigger the triggers
        std::for_each(objectList.begin(), objectList.end(), [](Object* o){ Trigger* tmpT = dynamic_cast<Trigger*>(o); if (tmpT != nullptr) tmpT->untrigger(); });
        //do physics
        posAlpha = physics->update(objectList);
        //check if positions were updated
        checkPositions();
    }
    if (gameState == GameState::Menu)
        if (levelLoaded)
            unloadLevel();
}

/**
 * Changes gameState to GameState::Playing and loads level.
 * @return void
 */
void Game::startGame()
{
	//Load level
	loadLevel();
	gameState = GameState::Playing;
}

/**
 * Changes gameState to GameState::Pause.
 * @return void
 */
void Game::pauseGame()
{
	gameState = GameState::Paused;
}

/**
 * Changes gameState to GameState::Playing.
 * @return void
 */
void Game::resumeGame()
{
	gameState = GameState::Playing;
}

/**
 * Changes gameState to GameState::Menu and unloads level.
 * @return void
 */
void Game::quitToMenu()
{
	gameState = GameState::Menu;
}

/**
 * Changes gameState to GameState::Quit.
 * @return void
 */
void Game::gameOver()
{
	gameState = GameState::Quit;
}

/**
 * Changes gameState to GameState::Won.
 * @return void
 */
void Game::wonGame()
{
    gameState = GameState::Won;
}

/**
 * Unloads level if loaded, loads level and initializes Physics.
 * @return void
 * @see Physics
 */
void Game::loadLevel()
{
    unloadLevel();
	objectList.push_back(new SolidObject(0, 17, 24, 3));
	objectList.push_back(new SolidObject(28, 17, 8, 3));
	objectList.push_back(new SolidObject(52, 17, 8, 3));
	objectList.push_back(new SolidObject(8, 15.5, 2, 1.5));
	objectList.push_back(new SolidObject(14, 14.5, 2, 2.5));
	objectList.push_back(new SolidObject(20, 14.5, 2, 2.5));
    objectList.push_back(new SolidObject(39, 14.5, 3, 1));
    objectList.push_back(new SolidObject(46, 14.5, 3, 1));
    objectList.push_back(new SolidObject(56, 0, 4, 14));
    objectList.push_back(new SolidObject(32, 13, 4, 1));
    objectList.push_back(new SolidObject(28, 10.5, 3, 1));
    objectList.push_back(new SolidObject(20, 9.5, 4, 1));
    objectList.push_back(new SolidObject(17, 0, 2, 8.5));
    objectList.push_back(new SolidObject(10, 9.5, 6, 1));
    objectList.push_back(new SolidObject(2, 7, 8, 1));

	objectList.push_back(new PlayerCreature(1, 16));
	playerController = new PlayerController(dynamic_cast<PlayerCreature*> (objectList.back()));
	controllerList.push_back(playerController);
    objectList.push_back(new MonsterCreature(9, 14.5, 1, 1));
    controllerList.push_back(new Controller(dynamic_cast<Creature*> (objectList.back())));
    objectList.push_back(new MonsterCreature(19, 16, 1, 1));
    controllerList.push_back(new Controller(dynamic_cast<Creature*> (objectList.back())));

    objectList.push_back(new Coin(4, 14));
    ++levelCoins;
    objectList.push_back(new Coin(14.5, 12));
    ++levelCoins;
    objectList.push_back(new Coin(20.5, 13));
    ++levelCoins;
    objectList.push_back(new Coin(43.5, 12));
    ++levelCoins;
    objectList.push_back(new Coin(33.5, 10.5));
    ++levelCoins;
    objectList.push_back(new Coin(21.5, 7));
    ++levelCoins;
    objectList.push_back(new Coin(13.5, 7));
    ++levelCoins;
    objectList.push_back(new Coin(11.5, 7));
    ++levelCoins;
    objectList.push_back(new Coin(7.5, 4.5));
    ++levelCoins;
    objectList.push_back(new Coin(5.5, 4.5));
    ++levelCoins;
    objectList.push_back(new Coin(3.5, 4.5));
    ++levelCoins;

    objectList.push_back(new Trigger(this, &Game::wonGame, nullptr, nullptr, 59, 14, 1, 3));

	levelWidth = 60;
	levelHeight = 20;
	levelLoaded = true;
	physics = new Physics(levelWidth, levelHeight);
}

/**
 * Unloads level if loaded.
 * @return void
 */
void Game::unloadLevel()
{
	if (levelLoaded)
	{
		std::for_each(controllerList.begin(), controllerList.end(), [](Controller* c){delete(c); });
		controllerList.clear();
		std::for_each(objectList.begin(), objectList.end(), [](Object* o){delete(o); });
		objectList.clear();
		delete physics;
		levelLoaded = false;
        levelCoins = 0;
        levelHeight = 0;
        levelWidth = 0;
	}
}

/**
 * Get current GameState.
 * @return GameState
 */
GameState Game::getGameState() const
{
    return gameState;
}

/**
 * Get pointer to PlayerController.
 * @return PlayerController*
 */
PlayerController* Game::getPlayerController() const
{
    return playerController;
}

/**
 * Check if game has ended.
 * @return bool
 */
bool Game::getHasEnded() const
{
	if (gameState == GameState::Quit)
		return true;
	else
		return false;
}

/**
 * Check if game state has changed.
 * @return bool
 */
bool Game::getPosUpdated() const
{
	return posUpdated;
}

/**
 * Get width of loaded level.
 * @return int
 */
int Game::getLevelWidth() const
{
    return levelWidth;
}

/**
 * Get height of loaded level.
 * @return int
 */
int Game::getLevelHeight() const
{
    return levelHeight;
}

/**
 * Get number of coins spawned on loaded level.
 * @return int
 */
Uint8 Game::getLevelCoins() const
{
    return levelCoins;
}

/**
 * Get coefficient of game state between steps, where 0 is previous step and 1 is current step.
 * @return double
 */
double Game::getPosAlpha() const
{
    return posAlpha;
}

/**
 * Get reference to constant list of loaded game objects.
 * @return std::list<Object*> const&
 */
std::list<Object*> const& Game::getObjectList() const
{
	return objectList;
}

/**
 * Sets posUpdated to true if state of game objects has changed.
 * @return void
 */
void Game::checkPositions()
{
    for (std::list<Object*>::iterator it = objectList.begin(); it != objectList.end(); ++it)
    {
        if ((*it)->getPrevX() != (*it)->getX() || (*it)->getPrevY() != (*it)->getY())
        {
            posUpdated = true;
        }
    }
}
