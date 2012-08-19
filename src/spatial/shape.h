#pragma once

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <luabind/lua_include.hpp>
#include <math/aabb.h>

namespace spatial
{

class Shape;
class SphereShape;
class LineShape;
class CapsuleShape;

typedef boost::weak_ptr<Shape> ShapeWeakPtr;
typedef boost::shared_ptr<Shape> ShapePtr;

class Shape
{
public:
	virtual ~Shape();
	virtual math::aabb<3> get_bounds() const = 0;

	virtual bool test_intersection(Shape const *shape) const = 0;
	virtual bool test_intersection(SphereShape const *shape) const = 0;
	virtual bool test_intersection(LineShape const *shape) const = 0;
	virtual bool test_intersection(CapsuleShape const *shape) const = 0;

	static void bind(lua_State *L);
};

}
