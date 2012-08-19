
#include <luabind/luabind.hpp>
#include "sphere_shape.h"
#include "line_shape.h"

namespace spatial
{

namespace
{
	ShapePtr upcast_to_shape(boost::shared_ptr<LineShape> const &shape_ptr)
	{
		return boost::static_pointer_cast<Shape>(shape_ptr);
	}
}

LineShape::LineShape()
{
}

LineShape::LineShape(math::vec<3> const &A, math::vec<3> const &B):
	math::line<3>(A, B)
{
}

LineShape::~LineShape()
{
}

math::aabb<3> LineShape::get_bounds() const
{
	return math::line<3>::get_aabb();
}

bool LineShape::test_intersection(Shape const *shape) const
{
	return shape->test_intersection(this);
}

bool LineShape::test_intersection(SphereShape const *shape) const
{
	return shape->sphere::test_intersection(*this);
}

bool LineShape::test_intersection(LineShape const *shape) const
{
	// NOT IMPLEMENTED
	assert(0);
	return false;
}

bool LineShape::test_intersection(CapsuleShape const *shape) const
{
	// NOT IMPLEMENTED
	assert(0);
	return false;
}

void LineShape::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "spatial")
	[
		class_<LineShape, Shape, math::line<3>, boost::shared_ptr<LineShape> >("LineShape")
		.def(constructor<math::vec<3> const &, math::vec<3> const &>())
		.scope [ def("upcast_to_shape", &upcast_to_shape) ]
	];
}

}
