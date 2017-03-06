#ifndef SDLWRAPPER_H
#define SDLWRAPPER_H

#include "InitError.h"
#include "GameDefs.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <vector>

/**
 * The literals that translate to the position of texture on menuTextureVector.
 * @see SDLWrapper
 */
enum MenuTexture
{
    //General purpose textures
    PauseMenu = 0,
    LostMenu,

    //Button textures
    StartButton,
    ResumeButton,
    MenuButton,
    QuitButton,
    RetryButton,

    //Text textures
    TitleText,
    SubtitleText,
    LostText,
    WinText
};

/**
 * The literals that translate to the position of texture on levelTextureVector.
 * @see SDLWrapper
 */
enum LevelTexture
{
    PlayerLeft = 0,
    PlayerRight,
    PlayerFront,
    MonsterLeft,
    MonsterRight
};

/**
 * The literals that translate to the position of font on fontVector.
 * @see SDLWrapper
 */
enum Font
{
	Regular = 0,
    Title,
    Subtitle,
    RegularOutline
};

/**
 * SDLWrapper is a class that handles window creation, resource loading and displaying graphics.
 */
class SDLWrapper
{
public:
    /**
     * Default constructor that initializes SDL, it's plugins and also loads and stores fonts and textures.
     */
	SDLWrapper();
	
    /**
     * Default destructor that unloads every loaded resource and quits initialized SDL subsystems.
     */
    ~SDLWrapper();

    /**
     * SDL window handle.
     */
	SDL_Window* window;
	
    /**
     * SDL renderer handle.
     */
    SDL_Renderer* renderer;

    /**
     * Vector that stores loaded fonts.
     */
	std::vector<TTF_Font*> fontVector;
	
    /**
     * Vector that stores loaded UI textures.
     */
    std::vector<SDL_Texture*> menuTextureVector;

    /**
     * Vector that stores loaded level textures.
     */
	std::vector<SDL_Texture*> levelTextureVector;

    /**
     * Loads texture from rendered text.
     * @param textureText string with text to render
     * @param textColor color of text
     * @param font which font to use. Defaults to Font::Regular
     * @return SDL_Texture* a pointer to loaded texture
     * @see Font
     */
    SDL_Texture* loadTextureFromRenderedText(std::string textureText, SDL_Color textColor, Font font = Font::Regular);
    
    /**
     * Loads texture from file.
     * @param path string with path to file
     * @return SDL_Texture* a pointer to loaded texture
     */
    SDL_Texture* loadTextureFromFile(std::string path);

    /**
     * Loads all level textures.
     * @return void
     */
    void loadLevelTextures();
    
    /**
     * Unloads all level textures.
     * @return void
     */
    void unloadLevelTextures();
private:
    /**
     * Loads all fonts.
     * @return void
     */
	void loadFonts();
	
    /**
     * Unloads all fonts.
     * @return void
     */
    void unloadFonts();

    /**
     * Loads all menu textures.
     * @return void
     */
	void loadMenuTextures();
	
    /**
     * Unloads all menu textures.
     * @return void
     */
    void unloadMenuTextures();
};

#endif // SDLWRAPPER_H