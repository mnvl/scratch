
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include "shape.h"

namespace spatial
{

namespace
{

ptrdiff_t naked_cast(ShapePtr const &shape_ptr)
{
	return (ptrdiff_t) shape_ptr.get();
}

}

Shape::~Shape()
{
}

void Shape::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "spatial")
	[
		class_<Shape, ShapePtr>("Shape")
		.def("get_bounds", &Shape::get_bounds)
		.def("test_intersection", (bool (Shape::*)(Shape const *) const) &Shape::test_intersection)
		.scope [ def("naked_cast", &naked_cast) ]
	];
}

}
