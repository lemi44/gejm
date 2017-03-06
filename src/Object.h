#ifndef OBJECT_H
#define OBJECT_H

/**
 * Object is a base class for all game objects.
 */
class Object
{
public:
    /**
     * The default constructor of Object.
     * @param x X position of Object. Defaults to 0.0
     * @param y Y position of Object. Defaults to 0.0
     * @param width width of Object. Defaults to 1.0
     * @param height height of Object. Defaults to 1.0
     */
    Object(double x = 0.0, double y = 0.0, double width = 1.0, double height = 1.0);

    /**
     * The default destructor.
     */
    virtual ~Object();

    /**
     * Virtual function that changes members that hold information about previous step to hold current state of Object.
     * @return void
     */
    virtual void savePrevious();

    /**
     * Destroy object by seeting it's destroyed value to true.
     * If object is destroyed, it shouldn't be used.
     * @return void
     */
    void destroy();

    /**
     * Get Y position.
     * @return double
     */
    double getY() const;
    
    /**
     * Get X position.
     * @return double
     */
    double getX() const;
    
    /**
     * Get objects width.
     * @return double
     */
    double getWidth() const;
    
    /**
     * Get objects height.
     * @return double
     */
    double getHeight() const;

    /**
     * Get previous X position.
     * @return double
     */
	double getPrevX() const;

    /**
     * Get previous Y position.
     * @return double
     */
	double getPrevY() const;

    /**
     * Check if object is destroyed.
     * @return bool
     */
	bool getDestroyed() const;

protected:
    /**
     * X position of Object.
     */
    double x;
    
    /**
     * Y position of Object.
     */
    double y;
	
    /**
     * Width of Object.
     */
    double width;
    
    /**
     * Height of Object.
     */
    double height;
	
    /**
     * Previous X position of Object.
     */
    double prevX;

    /**
     * Previous Y position of Object.
     */
	double prevY;

    /**
     * Is object destroyed.
     */
	bool destroyed;
    
};

#endif // OBJECT_H
