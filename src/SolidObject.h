#ifndef SOLIDOBJECT_H
#define SOLIDOBJECT_H

#include "Object.h"

/**
 * SolidObject is a base class for all collidable game objects.
 */
class SolidObject :
    public Object
{
public:
    /**
     * The default constructor of SolidObject.
     * @param x X position of SolidObject. Defaults to 0.0
     * @param y Y position of SolidObject. Defaults to 0.0
     * @param width width of SolidObject. Defaults to 1.0
     * @param height height of SolidObject. Defaults to 1.0
     */
    SolidObject(double x = 0.0, double y = 0.0, double width = 1.0, double height = 1.0);

    /**
     * The default destructor.
     */
    virtual ~SolidObject();
};

#endif // SOLIDOBJECT_H
