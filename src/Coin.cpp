#include "Coin.h"
#include <SDL_log.h>

/**
 * Coin implementation
 */

/**
 * Coin constructor that sets position and size of a Coin.
 * Size is a constant 1.0 width and 1.0 height.
 * @param x X position of coin. Defaults to 0.0.
 * @param y Y position of coin. Defaults to 0.0.
 * @see SolidObject
 */
Coin::Coin(double x, double y) :
    SolidObject(x, y, 1.0, 1.0)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Coin created at x:%f, y:%f", x, y);
}

/**
 * Coin destructor.
 */
Coin::~Coin()
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Coin destroyed");
}
