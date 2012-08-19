
#include <luabind/luabind.hpp>
#include "line.h"

namespace math
{

void bind_line(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<line<3> >("line3")
		.def_readwrite("A", &line<3>::A)
		.def_readwrite("B", &line<3>::B)
	];
}

}
