#pragma once

#include <math/sphere.h>
#include "shape.h"

namespace spatial
{

class SphereShape: public Shape, public math::sphere<3>
{
public:
	SphereShape();
	SphereShape(math::vec<3> const &centre, math::scalar radius);
	virtual ~SphereShape();

	virtual math::aabb<3> get_bounds() const;

	virtual bool test_intersection(Shape const *shape) const;
	virtual bool test_intersection(SphereShape const *shape) const;
	virtual bool test_intersection(LineShape const *shape) const;
	virtual bool test_intersection(CapsuleShape const *shape) const;

	void set_centre(math::vec<3> const &c)
	{
		centre = c;
	}

	void set_radius(math::scalar r)
	{
		radius = r;
	}

	static void bind(lua_State *L);
};

}
