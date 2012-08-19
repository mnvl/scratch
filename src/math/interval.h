#pragma once

#include <luabind/lua_include.hpp>
#include "scalar.h"

namespace math
{

template<class T = scalar>
class interval
{
public:
	typedef T scalar_t;

	scalar_t low, high;

	interval()
	{
	}

	interval(scalar_t l, scalar_t h):
		low(l),
		high(h)
	{
	}

	scalar_t length() const { return high - low; }
	scalar_t random() const { return length() * rand() / RAND_MAX + low; }
};

void bind_interval(lua_State *L);

}
