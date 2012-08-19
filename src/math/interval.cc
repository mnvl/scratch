
#include <luabind/luabind.hpp>
#include "interval.h"

namespace math
{

void bind_interval(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<interval<> >("interval")
		.def(constructor<>())
		.def(constructor<scalar, scalar>())
		.def("length", &interval<>::length)
		.def("random", &interval<>::random)
	];
}

}
