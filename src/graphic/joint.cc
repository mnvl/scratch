
#include <luabind/luabind.hpp>
#include "joint.h"

namespace graphic
{

void Joint::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<Joint, JointPtr>("Joint")
		.def_readwrite("matrix", &Joint::matrix)
	];
}

}
