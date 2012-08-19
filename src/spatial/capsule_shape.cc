
#include <luabind/luabind.hpp>
#include "line_shape.h"
#include "sphere_shape.h"
#include "capsule_shape.h"

namespace spatial
{

namespace
{
	ShapePtr upcast_to_shape(boost::shared_ptr<CapsuleShape> const &shape_ptr)
	{
		return boost::static_pointer_cast<Shape>(shape_ptr);
	}
}

CapsuleShape::CapsuleShape()
{
}

CapsuleShape::CapsuleShape(math::vec<3> const &A, math::vec<3> const &B, math::scalar radius):
	math::capsule<3>(A, B, radius)
{
}

CapsuleShape::~CapsuleShape()
{
}

math::aabb<3> CapsuleShape::get_bounds() const
{
	math::aabb<3> bounds;
	get_aabb(bounds);
	return bounds;
}

bool CapsuleShape::test_intersection(Shape const *shape) const
{
	return shape->test_intersection(this);
}

bool CapsuleShape::test_intersection(SphereShape const *shape) const
{
	return shape->sphere::test_intersection(*shape);
}

bool CapsuleShape::test_intersection(LineShape const *shape) const
{
	// NOT IMPLEMENTED
	assert(0);
	return false;
}

bool CapsuleShape::test_intersection(CapsuleShape const *shape) const
{
	// NOT IMPLEMENTED
	assert(0);
	return false;
}

void CapsuleShape::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "spatial")
	[
		class_<CapsuleShape, Shape, math::capsule<3>, boost::shared_ptr<CapsuleShape> >("CapsuleShape")
		.def(constructor<math::vec<3> const &, math::vec<3> const &, math::scalar>())
		.scope [ def("upcast_to_shape", &upcast_to_shape) ]
	];
}

}
