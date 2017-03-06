#include "InitError.h"

/**
 * Default constructor of exception with SDL_GetError() message.
 */
InitError::InitError() :
	exception(),
	msg(SDL_GetError())
{
}

/**
 * Default constructor of exception with custom message.
 * @param m custom message
 */
InitError::InitError(const std::string& m) :
	exception(),
	msg(m)
{
}

#if (_MSC_VER<1900) && defined(_MSC_VER)
/**
* Default destructor.
*/
InitError::~InitError() throw()
{
}

/**
* Overload of virtual function that gets string identifying exception.
* return char const*
*/
const char* InitError::what() const throw()
{
    return msg.c_str();
}
#else
/**
* Default destructor.
*/
InitError::~InitError() noexcept
{
}

/**
* Overload of virtual function that gets string identifying exception.
* return char const*
*/
const char* InitError::what() const noexcept
{
    return msg.c_str();
}
#endif
