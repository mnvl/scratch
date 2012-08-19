
#include <luabind/luabind.hpp>
#include "line_shape.h"
#include "capsule_shape.h"
#include "sphere_shape.h"

namespace spatial
{

namespace
{
	ShapePtr upcast_to_shape(boost::shared_ptr<SphereShape> const &shape_ptr)
	{
		return boost::static_pointer_cast<Shape>(shape_ptr);
	}
}

SphereShape::SphereShape()
{
}

SphereShape::SphereShape(math::vec<3> const &centre, math::scalar radius):
	math::sphere<3>(centre, radius)
{
}

SphereShape::~SphereShape()
{
}

math::aabb<3> SphereShape::get_bounds() const
{
	return get_aabb();
}

bool SphereShape::test_intersection(Shape const *shape) const
{
	return shape->test_intersection(this);
}

bool SphereShape::test_intersection(SphereShape const *shape) const
{
	return sphere::test_intersection(*shape);
}

bool SphereShape::test_intersection(LineShape const *shape) const
{
	return sphere::test_intersection((math::line<3> const &) *shape);
}

bool SphereShape::test_intersection(CapsuleShape const *shape) const
{
	return sphere::test_intersection(*shape);
}

void SphereShape::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "spatial")
	[
		class_<SphereShape, Shape, math::sphere<3>, boost::shared_ptr<SphereShape> >("SphereShape")
		.def(constructor<math::vec<3> const &, math::scalar>())
		.def("set_centre", &SphereShape::set_centre)
		.def("set_radius", &SphereShape::set_radius)
		.scope [ def("upcast_to_shape", &upcast_to_shape) ]
	];
}

}
