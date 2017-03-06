#ifndef INITERROR_H
#define INITERROR_H

#include <exception>
#include <string>
#include <SDL.h>

/**
 * InitError is a custom exception that includes SDL_GetError() message.
 */
class InitError :
    public std::exception
{
public:
    /**
     * Default constructor of exception with SDL_GetError() message.
     */
	InitError();

    /**
     * Default constructor of exception with custom message.
     * @param m custom message
     */
	InitError(const std::string& m);

#if (_MSC_VER<1900) && defined(_MSC_VER)
    /**
     * Default destructor.
     */
	virtual ~InitError() throw();
    
    /**
     * Overload of virtual function that gets string identifying exception.
     * return char const*
     */
    virtual const char* what() const throw();
#else
    /**
     * Default destructor.
     */
    virtual ~InitError() noexcept;

    /**
     * Overload of virtual function that gets string identifying exception.
     * return char const*
     */
    virtual const char* what() const noexcept;
#endif
	
private:
    /**
     * String identifying exception.
     */
	std::string msg;
};

#endif // INITERROR_H