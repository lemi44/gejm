#include "Trigger.h"

/**
 * Trigger implementation
 */

/**
 * Trigger constructor that sets position and size of a Trigger.
 * @param gameObject A constant pointer to Game object.
 * @param onTrigger A pointer to Game's member function that's called while triggered. If triggerOnce is set, onTrigger is basically the same as onStartTouch.
 * @param onStartTouch A pointer to Game's member function that's called whenever PlayerCreature enters trigger.
 * @param onEndTouch A pointer to Game's member function that's called whenever PlayerCreature leaves trigger.
 * @param x X position of Trigger. Defaults to 0.0.
 * @param y Y position of Trigger. Defaults to 0.0.
 * @param width Width of Trigger. Defaults to 3.0.
 * @param height Height of Trigger. Defaults to 3.0.
 * @param triggerOnce If you want to use trigger multiple times set to false. Otherwise trigger will fire only once.
 * @see SolidObject
 * @see Game
 */
Trigger::Trigger(Game *const gameObject,
    void (Game::*onTrigger)(),
    void (Game::*onStartTouch)(),
    void (Game::*onEndTouch)(),
    double x,
    double y,
    double width,
    double height,
    bool triggerOnce) :
    SolidObject(x, y, width, height),
    onTrigger(nullptr),
    onStartTouch(nullptr),
    onEndTouch(nullptr),
    triggerOnce(triggerOnce),
    isTriggered(false)
{
    if (onTrigger != (void(Game::*)())nullptr)
        this->onTrigger = std::bind(onTrigger, gameObject);
    if (onStartTouch != (void(Game::*)())nullptr)
        this->onStartTouch = std::bind(onStartTouch, gameObject);
    if (onEndTouch != (void(Game::*)())nullptr)
        this->onEndTouch = std::bind(onEndTouch, gameObject);
}

/**
 * Trigger destructor.
 */
Trigger::~Trigger()
{
}

/**
 * Fire when something collides with trigger.
 */
void Trigger::trigger()
{
    if (triggerOnce)
    {
        if (onStartTouch != nullptr)
            onStartTouch();
        destroy();
    }
    else
    {
        if (!isTriggered)
        {
            if (onStartTouch != nullptr)
                onStartTouch();
            isTriggered = true;
        }
    }
    if (onTrigger != nullptr)
        onTrigger();
}

/**
 * Fire when something stops collision with trigger.
 */
void Trigger::untrigger()
{
    if (isTriggered)
    {
        isTriggered = false;
        if (onEndTouch != nullptr)
            onEndTouch();
    }
}
