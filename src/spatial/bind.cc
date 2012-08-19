
#include "shape.h"
#include "sphere_shape.h"
#include "line_shape.h"
#include "capsule_shape.h"
#include "result_set.h"
#include "octree.h"
#include "bind.h"

namespace spatial
{

void bind(lua_State *L)
{
	Shape::bind(L);
	SphereShape::bind(L);
	LineShape::bind(L);
	CapsuleShape::bind(L);
	ResultSet::bind(L);
	Octree::bind(L);
}

}
