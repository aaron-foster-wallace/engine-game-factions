#pragma once
#ifndef ICLOCK_HPP
#define ICLOCK_HPP

#include "../Export.hpp"

class GAMEAPI IClock
{

public:

	virtual ~IClock( ) { };

	virtual float GetDeltaMilliseconds( ) = 0;

	virtual float GetTime( ) const = 0;

};

#endif
