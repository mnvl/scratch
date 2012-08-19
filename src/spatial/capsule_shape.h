#pragma once

#include <math/capsule.h>
#include "shape.h"

namespace spatial
{

class CapsuleShape: public Shape, public math::capsule<3>
{
public:
	CapsuleShape();
	CapsuleShape(math::vec<3> const &A, math::vec<3> const &B, math::scalar radius);
	virtual ~CapsuleShape();

	virtual math::aabb<3> get_bounds() const;

	virtual bool test_intersection(Shape const *shape) const;
	virtual bool test_intersection(SphereShape const *shape) const;
	virtual bool test_intersection(LineShape const *shape) const;
	virtual bool test_intersection(CapsuleShape const *shape) const;

	static void bind(lua_State *L);
};

}
