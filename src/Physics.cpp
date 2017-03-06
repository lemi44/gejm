#include "Physics.h"

/**
 * Physics implementation
 */

/**
 * Default constructor of Physics.
 * Sets currentTime, t, dt, accumulator and boundaries of physical simulation.
 * @param boundaryWidth width of bounding box in which physical simulation is simulated
 * @param boundaryHeight height of bounding box in which physical simulation is simulated
 */
Physics::Physics(int boundaryWidth, int boundaryHeight) :
    t(0.0),
    dt(0.01),
    currentTime(SDL_GetTicks() / 1000.0),
    accumulator(0.0),
    boundaryWidth(boundaryWidth),
    boundaryHeight(boundaryHeight)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Physics object created!");
}

/**
 * Default destructor.
 */
Physics::~Physics()
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Physics object destroyed!");
}

/**
 * Calculate next steps of simulation.
 * @param objectList reference to a list of all game objects
 * @return double because physical simulation is calculated in fixed steps, function returns coefficient of game state between steps, where 0 is previous step and 1 is current step
 */
double Physics::update(std::list<Object*> &objectList)
{
	double newTime = SDL_GetTicks() / 1000.0;
	double frameTime = newTime - currentTime;
	if (frameTime > 0.25)
		frameTime = 0.25;
	currentTime = newTime;

	accumulator += frameTime;

	while (accumulator >= dt)
	{
		for (std::list<Object*>::iterator it = objectList.begin(); it != objectList.end(); ++it)
		{
			Creature* tmpC = dynamic_cast<Creature*>(*it);
			if (tmpC == nullptr)
				continue;
            tmpC->savePrevious();
			State stateX = { tmpC->getX(), tmpC->getSpeedX() };
			State stateY = { tmpC->getY(), tmpC->getSpeedY() };
			integrate(stateX, t, dt, false);
			integrate(stateY, t, dt, true);
			tmpC->moveBy(stateX.x - tmpC->getX(), stateY.x - tmpC->getY());
			tmpC->setSpeedVector(stateX.v, stateY.v);
		}
		t += dt;
		accumulator -= dt;
		checkCollision(objectList);
	}

	return accumulator / dt;

}

/**
 * An RK4 integrator used to calculate next State based on current State.
 * @param state reference to state that will be changed
 * @param t current point in discrete time
 * @param dt a timestep
 * @param isY if false gravity won't be added into calculation, otherwise it will be
 * @return void
 */
void Physics::integrate(State &state, double t, double dt, bool isY)
{
    Derivative a, b, c, d;

    a = evaluate(state, t, 0.0f, Derivative(), isY);
    b = evaluate(state, t, dt*0.5f, a, isY);
    c = evaluate(state, t, dt*0.5f, b, isY);
    d = evaluate(state, t, dt, c, isY);

    double dxdt = 1.0f / 6.0f *
        (a.dx + 2.0f*(b.dx + c.dx) + d.dx);

    double dvdt = 1.0f / 6.0f *
        (a.dv + 2.0f*(b.dv + c.dv) + d.dv);

    state.x = state.x + dxdt * dt;
    state.v = state.v + dvdt * dt;
}

/**
 * Calculate a derivative of initial state in discrete time.
 * @param state Reference to state that will be changed
 * @param t current point in discrete time
 * @param dt a timestep
 * @param d a derivative by which state should be changed in time
 * @param isY if false gravity won't be added into calculation, otherwise it will be
 * @return Derivative new derivative after calculations
 */
Derivative Physics::evaluate(const State &initial, double t, double dt, const Derivative &d, bool isY)
{
	State state;
	state.x = initial.x + d.dx*dt;
	state.v = initial.v + d.dv*dt;

	Derivative output;
	output.dx = state.v;
	if (isY)
		output.dv = acceleration(state, t + dt);
	else
		output.dv = 0;
	return output;
}

/**
 * Calculate acceleration by applying spring and damper forces.
 * Currently is changed to return constant 9.81 which is Earth's gravitational acceleration.
 * @param state reference to state that is used in calculations
 * @param t current point in discrete time
 * @return double used to be calculated spring and damper forces. Now always constant 9.81
 */
double Physics::acceleration(const State& state, double t)
{
	//const float k = -10;
	//const float b = 1;
	//return -k * state.x - b*state.v;
	return 9.81;
}

/**
 * Performs check on Axis-Aligned Bounding Boxes if they collide and calls proper collision resolving functions.
 * @param objectList reference to a list of all game objects
 * @return void
 */
void Physics::checkCollision(std::list<Object*> &objectList)
{
	std::list<Object*>::iterator it;
	for (it = objectList.begin(); it != objectList.end(); ++it)
	{
		Creature* tmpC = dynamic_cast<Creature*>(*it);
		if (tmpC == nullptr)
			continue;
        if (tmpC->getX() < 0)
        {
            tmpC->moveBy(-tmpC->getX(), 0);
            tmpC->addCollisionState(CollisionState::FromRight);
        }
        else if (tmpC->getX() > boundaryWidth - tmpC->getWidth())
        {
            tmpC->moveBy(-((tmpC->getWidth() + tmpC->getX()) - boundaryWidth), 0);
            tmpC->addCollisionState(CollisionState::FromLeft);
        }
        if (tmpC->getY() < 0)
        {
            tmpC->moveBy(0, -tmpC->getY());
            tmpC->addCollisionState(CollisionState::FromBelow);
        }

		std::list<Object*>::iterator it2;
		for (it2 = objectList.begin(); it2 != objectList.end(); ++it2)
		{
			SolidObject* tmpSolidObject = dynamic_cast<SolidObject*>(*it2);
			if (tmpSolidObject == nullptr || tmpC == tmpSolidObject)
				continue;

			if (tmpC->getX() < tmpSolidObject->getX() + tmpSolidObject->getWidth() &&
				tmpC->getX() + tmpC->getWidth() > tmpSolidObject->getX() &&
				tmpC->getY() < tmpSolidObject->getY() + tmpSolidObject->getHeight() &&
				tmpC->getHeight() + tmpC->getY() > tmpSolidObject->getY())
			{
				tmpC->onCollision(tmpSolidObject);
			}

		}

	}
}
