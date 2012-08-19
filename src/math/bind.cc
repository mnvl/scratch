
#include "interval.h"
#include "vec.h"
#include "aabb.h"
#include "obb.h"
#include "matrix.h"
#include "contact_info.h"
#include "ray.h"
#include "line.h"
#include "plane.h"
#include "frustum.h"
#include "sphere.h"
#include "capsule.h"
#include "heightfield.h"
#include "bind.h"

namespace math
{

void bind(lua_State *L)
{
	bind_interval(L);
	bind_vec(L);
	bind_aabb(L);
	bind_obb(L);
	bind_matrix(L);
	bind_contact_info(L);
	bind_ray(L);
	bind_line(L);
	bind_plane(L);
	bind_frustum(L);
	bind_sphere(L);
	bind_capsule(L);
	bind_heightfield(L);
}

}
