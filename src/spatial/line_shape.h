#pragma once

#include <math/line.h>
#include "shape.h"

namespace spatial
{

class LineShape: public Shape, public math::line<3>
{
public:
	LineShape();
	LineShape(math::vec<3> const &A, math::vec<3> const &B);
	virtual ~LineShape();

	virtual math::aabb<3> get_bounds() const;

	virtual bool test_intersection(Shape const *shape) const;
	virtual bool test_intersection(SphereShape const *shape) const;
	virtual bool test_intersection(LineShape const *shape) const;
	virtual bool test_intersection(CapsuleShape const *shape) const;

	static void bind(lua_State *L);
};

}
