#include "SolidObject.h"

/** 
 * SolidObject implementation
 */

/**
 * The default constructor of SolidObject.
 * @param x X position of SolidObject. Defaults to 0.0
 * @param y Y position of SolidObject. Defaults to 0.0
 * @param width width of SolidObject. Defaults to 1.0
 * @param height height of SolidObject. Defaults to 1.0
 */
SolidObject::SolidObject(double x, double y, double width, double height):
	Object(x,y,width,height)
{
}

/**
 * The default destructor.
 */
SolidObject::~SolidObject()
{
}
