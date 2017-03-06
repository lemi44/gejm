#ifndef COIN_H
#define COIN_H

#include "SolidObject.h"

/**
 * Coin is a collectable coin object that derives from SolidObject.
 * It's width and height is fixed to 1.0 x 1.0.
 */
class Coin :
    public SolidObject
{
public:
    /**
     * Coin constructor that sets position and size of a Coin.
     * Size is a constant 1.0 width and 1.0 height.
     * @param x X position of coin. Defaults to 0.0.
     * @param y Y position of coin. Defaults to 0.0.
     * @see SolidObject
     */
    Coin(double x = 0.0, double y = 0.0);

    /**
     * Coin destructor.
     */
    ~Coin();
};
#endif // COIN_H