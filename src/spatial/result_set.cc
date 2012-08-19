
#include <luabind/luabind.hpp>
#include <luabind/iterator_policy.hpp>
#include "result_set.h"

namespace spatial
{

void ResultSet::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "spatial")
	[
		class_<ResultSet, ResultSetPtr>("ResultSet")
		.def(constructor<>())
		.def("get_shapes", &ResultSet::get_shapes, return_stl_iterator)
	];
}

}
