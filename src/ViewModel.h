#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include "Game.h"
#include "SDLWrapper.h"
#include "Timer.h"

/**
 * The types of how text should be align.
 */
enum class TextAlignment
{
    Left,
    Right,
    Center
};

/**
 * ViewModel is a class responsible for drawing game.
 * It interprets game states and objects and draws them on screen using SDLWrapper.
 */
class ViewModel
{
public:
    /**
     * Initializes variables.
     * @param game a constant pointer to a Game object
     * @param sdlWrapper a constant pointer to an SDLWrapper
     */
	ViewModel(Game* const game, SDLWrapper* const sdlWrapper);
	
    /**
     * Default destructor
     */
    ~ViewModel();
	
    /**
     * Handling events.
     * @return void
     */
    void handleEvents();

    /**
     * A draw loop.
     * Interprets current game state and chooses what to draw.
     * @return void
     */
	void drawLoop();

    /**
     * Assigment operator is overloaded because it cannot be generated by compiler but because it shouldn't be used it's deleted.
     */
    ViewModel& operator=(ViewModel const&) = delete;

private:
    /**
     * A constant pointer to Game object.
     */
	Game* const game;
	
    /**
     * A constant pointer to SDLWrapper object.
     */
    SDLWrapper* const sdlWrapper;
	
    /**
     * Was a pause menu drawn.
     */
    bool hasDrawnPauseMenu;

    /**
     * A rectangle representing what should be displayed.
     */
    SDL_Rect camera;
    
    /**
     * A timer used to count average value of frames per second.
     */
    Timer fpsTimer;
    
    /**
     * A timer used to count current value of frames per second.
     */
    Timer curFPSTimer;

    /**
     * A number of every frames since start of drawing.
     */
    int countedFrames;
    
    /**
     * A number of frames that's reset every second.
     */
    int otherCountedFrames;
    
    /**
     * An average number of frames per second.
     */
    float avgFPS;

    /**
     * A current number of frames per second.
     */
    int curFPS;

    /**
     * A Timer used for fade in/outs while drawing.
     */
    Timer fadeTimer;
    
    /**
     * Interprets all game objects and draws them accordingly on screen.
     * @return void
     */
    void drawGame();
	
    /**
     * Draws menu screen.
     * @return void
     */
    void drawMenu();
	
    /**
     * Draws pause screen.
     * @return void
     */
    void drawPauseMenu();
    
    /**
     * Draws failure screen.
     * @return void
     */
    void drawLost();

    /**
     * Draws success screen.
     * @return void
     */
    void drawWon();

    /**
     * Draws an interactive button.
     * @param buttonX X position of button
     * @param buttonY Y position of button
     * @param buttonWidth width of button
     * @param buttonHeight height of button
     * @param texture a pointer to texture that should be used on button
     * @param onClick a member function pointer to Game member function that should be called upon button press
     * @return void
     */
	void drawButton(int buttonX, int buttonY, int buttonWidth, int buttonHeight, SDL_Texture* texture, void(Game::*onClick)());
    
    /**
     * Draws text.
     * @param x X position of text
     * @param y Y position of text
     * @param texture a pointer to texture that should be drawn (preferably a texture representing text)
     * @param align how text should be aligned meaning how x and y values should be treated.
     * @return void
     */
    void drawText(int x, int y, SDL_Texture* texture, TextAlignment align = TextAlignment::Center);
    
    /**
     * Calculate an average number of frames per second.
     * @return void
     */
	void correctFPS();

    /**
     * Calculate a number of frames per second.
     * @return void
     */
	void currentFPS();
};

#endif // VIEWMODEL_H