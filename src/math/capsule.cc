
#include "aabb.h"
#include "obb.h"
#include "capsule.h"

namespace math
{

template<int N, class T> void
capsule<N, T>::get_aabb(aabb<ARITY, scalar_t> &bounds) const
{
	vec_t diagonal(radius, radius, radius);

	bounds.lo = bounds.hi = axe.A;
	bounds.extend(axe.B);

	bounds.extend(axe.A - diagonal);
	bounds.extend(axe.A + diagonal);

	bounds.extend(axe.B - diagonal);
	bounds.extend(axe.B + diagonal);
}

void bind_capsule(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<capsule<3> >("capsule3")
		.def_readwrite("axe", &capsule<3>::axe)
		.def_readwrite("radius", &capsule<3>::radius)
	];
}

template class capsule<3>;

}
