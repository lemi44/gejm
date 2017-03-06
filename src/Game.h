#ifndef GAME_H
#define GAME_H

#include "Object.h"
#include "Physics.h"
#include "Controller.h"
#include "PlayerController.h"
#include <algorithm>
#include <list>
#include <ctime>

/**
 * The types of states that can represent Game.
 * @see Game
 */
enum class GameState
{
    /**
     * Currently in menu
     */
	Menu,

    /**
     * Currently playing
     */
	Playing,

    /**
     * Currently in pause menu
     */
	Paused,

    /**
     * Currently on win screen
     */
	Won,

    /**
     * Currently on lose screen
     */
	Lost,

    /**
     * Currently game is closing
     */
	Quit
};

/**
 * Game is a core game class.
 * It's responsible for level loading and updating game state.
 */
class Game
{
public: 
    /**
     * The default constructor.
     * Sets gameState to menu.
     * @see GameState
     */
    Game();

    /**
     * The default destructor.
     * Unloads level.
     */
	~Game();

    /**
     * Updates current state of game.
     * @return void
     */
    void gameLoop();
	
    /**
     * Changes gameState to GameState::Playing and loads level.
     * @return void
     */
    void startGame();

    /**
     * Changes gameState to GameState::Pause.
     * @return void
     */
	void pauseGame();

    /**
     * Changes gameState to GameState::Playing.
     * @return void
     */
	void resumeGame();

    /**
     * Changes gameState to GameState::Menu and unloads level.
     * @return void
     */
	void quitToMenu();

    /**
     * Changes gameState to GameState::Quit.
     * @return void
     */
	void gameOver();

    /**
     * Changes gameState to GameState::Won.
     * @return void
     */
    void wonGame();

    /**
     * Unloads level if loaded, loads level and initializes Physics.
     * @return void
     * @see Physics
     */
	void loadLevel();
	
    /**
     * Unloads level if loaded.
     * @return void
     */
    void unloadLevel();

    /**
     * Get current GameState.
     * @return GameState
     */
	GameState getGameState() const;

    /**
     * Get pointer to PlayerController.
     * @return PlayerController*
     */
	PlayerController* getPlayerController() const;

    /**
     * Check if game has ended.
     * @return bool
     */
    bool getHasEnded() const;
	
    /**
     * Check if game state has changed.
     * @return bool
     */
    bool getPosUpdated() const;

    /**
     * Get width of loaded level.
     * @return int
     */
	int getLevelWidth() const;
	
    /**
     * Get height of loaded level.
     * @return int
     */
    int getLevelHeight() const;
    
    /**
     * Get number of coins spawned on loaded level.
     * @return int
     */
    Uint8 getLevelCoins() const;
	
    /**
     * Get coefficient of game state between steps, where 0 is previous step and 1 is current step.
     * @return double
     */
    double getPosAlpha() const;

    /**
     * Get reference to constant list of loaded game objects.
     * @return std::list<Object*> const&
     */
	std::list<Object*> const& getObjectList() const;
private:
    /**
     * List of loaded game objects.
     */
	std::list<Object*> objectList;

    /**
     * Current state of game.
     */
	GameState gameState;

    /**
     * Answer to whether game state has changed.
     */
	bool posUpdated;

    /**
     * Width of loaded level.
     */
	int levelWidth;

    /**
     * Height of loaded level.
     */
	int levelHeight;

    /**
     * Number of coins on loaded level.
     */
    Uint8 levelCoins;
    
    /**
     * Answer to whether level is loaded.
     */
	bool levelLoaded;

    /**
     * Coefficient of game state between steps, where 0 is previous step and 1 is current step.
     */
	double posAlpha;

    /**
     * Pointer to PlayerController object.
     */
	PlayerController* playerController;

    /**
     * List of Controller objects.
     */
    std::list<Controller*> controllerList;
	
    /**
     * Pointer to Physics object.
     */
    Physics* physics;

    /**
     * Sets posUpdated to true if state of game objects has changed.
     * @return void
     */
	void checkPositions();
};

#endif // GAME_H
