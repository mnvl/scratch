
#include <assert.h>
#include <luabind/luabind.hpp>
#include "aabb.h"
#include "obb.h"
#include "plane.h"

namespace math
{

template<class T> typename plane<T>::vec_t
plane<T>::get_origin() const
{
	// Ax + By + Cz + D = 0 => A(x + D/A) + By + Cz = 0
	// A(x - x0) + B(y - y0) + C(z - z0) = 0
	// x0 = -D / A

	if (abs(A) > EPSILON) return vec_t(-D / A, 0, 0);
	if (abs(B) > EPSILON) return vec_t(0, -D / B, 0);
	if (abs(C) > EPSILON) return vec_t(0, 0, -D / C);

	assert(0 && "does not define a plane");

	return vec_t(0, 0, 0);
}

template<class T> bool
plane<T>::query_intersection(const plane &p, ray_t &r) const {
	r.a = vec_t(A,B,C) ^ vec_t(p.A, p.B, p.C);
	vec_t dir2 = r.a * r.a;

	scalar_t invdet;
	if(dir2.z > dir2.y && dir2.z > dir2.x && dir2.z > EPSILON)
	{
		/* then get a point on the XY plane */
		invdet = scalar_t(1) / r.a.z;
		/* solve < pl1.x * xpt.x + pl1.y * xpt.y = - pl1.w >
		< pl2.x * xpt.x + pl2.y * xpt.y = - pl2.w > */
		r.r0.x = B * p.D - p.B *D;
		r.r0.y = p.A * D - A * p.D;
		r.r0.z = 0;
	}
	else if (dir2.y > dir2.x && dir2.y > EPSILON)
	{
		/* then get a point on the XZ plane */
		invdet = scalar_t(1) / r.a.y;
		/* solve < pl1.x * xpt.x + pl1.z * xpt.z = -pl1.w >
		< pl2.x * xpt.x + pl2.z * xpt.z = -pl2.w > */
		r.r0.x = C * p.D - p.C * D;
		r.r0.y = 0;
		r.r0.z = p.A * D -A * p.D;
	}
	else if (dir2.x > EPSILON)
	{
		/* then get a point on the YZ plane */
		invdet = scalar_t(1) / r.a.x;
		/* solve < pl1.y * xpt.y + pl1.z * xpt.z = - pl1.w >
		< pl2.y * xpt.y + pl2.z * xpt.z = - pl2.w > */
		r.r0.x = 0;
		r.r0.y = C * p.D - p.C * D;
		r.r0.z = p.B * D -B * p.D;
	}
	else return false;

	r.r0 *= invdet;

	invdet = scalar_t(1) / sqrt(dir2.x + dir2.y + dir2.z);
	r.a *= invdet;

	return true;
}

template<class T> typename plane<T>::classification_t
plane<T>::classify(vec_t const &point) const
{
	scalar_t value = apply(point);
	if (abs(value) < EPSILON) return INTERSECTS;
	return value > 0 ? POSITIVE : NEGATIVE;
}

template<class T> typename plane<T>::classification_t
plane<T>::classify(aabb<ARITY, scalar_t> const &bounds) const
{
	return classify(obb<ARITY, scalar_t>(bounds));
}

template<class T> typename plane<T>::classification_t
plane<T>::classify(obb<ARITY, scalar_t> const &bounds) const
{
	bool positive = false, negative = false;

	for (size_t i = 0; i < obb<ARITY, scalar_t>::VERTICES_COUNT; ++i)
	{
		vec_t vertex;
		bounds.get_vertex(i, vertex);

		switch (classify(vertex)) {
		case POSITIVE: positive = true; break;
		case NEGATIVE: negative = true; break;
		default: return INTERSECTS;
		};

		if (positive && negative) return INTERSECTS;
	}

	if (positive) return POSITIVE;

	assert (negative);
	return NEGATIVE;
}

template class plane<>;

void bind_plane(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<plane<> >("plane")
		.enum_("const")
		[
			value("POSITIVE", plane<>::POSITIVE),
			value("INTERSECTS", plane<>::INTERSECTS),
			value("NEGATIVE", plane<>::NEGATIVE)
		 ]
		.def(constructor<>())
		.def(constructor<const vec<3> &,const vec<3> &>())
		.def("trace", (scalar (plane<>::*)(const ray<3> &) const) &plane<>::trace)
		.def("classify", (plane<>::classification_t (plane<>::*)(vec<3> const &) const) &plane<>::classify)
		.def("classify", (plane<>::classification_t (plane<>::*)(aabb<3> const &) const) &plane<>::classify)
		.def("classify", (plane<>::classification_t (plane<>::*)(obb<3> const &) const) &plane<>::classify)
	];
}

}
