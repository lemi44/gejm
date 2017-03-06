#include "Object.h"

/**
 * Object implementation
 */

/**
 * The default constructor of Object.
 * @param x X position of Object. Defaults to 0.0
 * @param y Y position of Object. Defaults to 0.0
 * @param width width of Object. Defaults to 1.0
 * @param height height of Object. Defaults to 1.0
 */
Object::Object(double x, double y, double width, double height) :
	x(x),
	y(y),
	width(width),
	height(height),
	prevX(x),
	prevY(y),
	destroyed(false)
{
}

/**
 * The default destructor.
 */
Object::~Object()
{
}

/**
 * Virtual function that changes members that hold information about previous step to hold current state of Object.
 * @return void
 */
void Object::savePrevious()
{
    prevX = x;
    prevY = y;
}

/**
 * Destroy object by seeting it's destroyed value to true.
 * If object is destroyed, it shouldn't be used.
 * @return void
 */
void Object::destroy()
{
    destroyed = true;
}

/**
 * Get Y position.
 * @return double
 */
double Object::getY() const
{
    return y;
}

/**
 * Get X position.
 * @return double
 */
double Object::getX() const
{
    return x;
}

/**
 * Get objects width.
 * @return double
 */
double Object::getWidth() const
{
    return width;
}

/**
 * Get objects height.
 * @return double
 */
double Object::getHeight() const
{
    return height;
}

/**
 * Get previous X position.
 * @return double
 */
double Object::getPrevX() const
{
	return prevX;
}

/**
 * Get previous Y position.
 * @return double
 */
double Object::getPrevY() const
{
	return prevY;
}

/**
 * Check if object is destroyed.
 * @return bool
 */
bool Object::getDestroyed() const
{
	return destroyed;
}
