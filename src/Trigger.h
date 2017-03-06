#ifndef TRIGGER_H
#define TRIGGER_H

#include "Game.h"
#include "SolidObject.h"
#include <functional>

/**
 * Trigger is an object that executes a function when colliding with PlayerCreature.
 * It derives from SolidObject.
 */
class Trigger :
    public SolidObject
{
public:
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
    Trigger(Game *const gameObject,
        void (Game::*onTrigger)(),
        void (Game::*onStartTouch)(),
        void (Game::*onEndTouch)(),
        double x = 0.0,
        double y = 0.0,
        double width = 3.0,
        double height = 3.0,
        bool triggerOnce = true);

    /**
     * Trigger destructor.
     */
    ~Trigger();

    /**
     * Fire when something collides with trigger.
     */
    void trigger();
    
    /**
     * Fire when something stops collision with trigger.
     */
    void untrigger();

    /**
     * Assignment operator is deleted because Trigger has constant variable.
     */
    Trigger& operator=(Trigger const&) = delete;
private:
    /**
     * Function object binded to member function of a Game object that's to be fired when trigger is triggered.
     */
    std::function<void(void)> onTrigger;
    
    /**
     * Function object binded to member function of a Game object that's to be fired on beginning of collision with trigger.
     */
    std::function<void(void)> onStartTouch;
    
    /**
     * Function object binded to member function of a Game object that's to be fired on ending of collision with trigger.
     */
    std::function<void(void)> onEndTouch;
    
    /**
     * Should the trigger be triggered only once.
     */
    bool const triggerOnce;
    
    /**
     * Is trigger triggered.
     */
    bool isTriggered;
};
#endif // TRIGGER_H