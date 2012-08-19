
#include <luabind/luabind.hpp>
#include "octree.h"

namespace spatial
{

void Octree::bind(lua_State *L)
{
	using namespace luabind;

    module(L, "spatial")
	[
		class_<Octree, boost::shared_ptr<Octree> >("Octree")
		.def(constructor<math::aabb<3> >())
		.def("add_shape", (void (Octree::*)(ShapePtr const &)) &Octree::add_shape)
		.def("add_shape", (void (Octree::*)(ShapePtr const &, ResultSetPtr const &)) &Octree::add_shape)
		.def("remove_shape", (void (Octree::*)(ShapePtr const &)) &Octree::remove_shape)
		.def("pre_move_shape", (void (Octree::*)(ShapePtr const &, math::aabb<3> const &)) &Octree::pre_move_shape)
		.def("pre_move_shape", (void (Octree::*)(ShapePtr const &, math::aabb<3> const &, ResultSetPtr const &)) &Octree::pre_move_shape)
		.def("post_move_shape", (void (Octree::*)(ShapePtr const &, math::aabb<3> const &)) &Octree::post_move_shape)
		.def("post_move_shape", (void (Octree::*)(ShapePtr const &, math::aabb<3> const &, ResultSetPtr const &)) &Octree::post_move_shape)
		.def("query_intersection", (void (Octree::*)(ShapePtr const &, ResultSetPtr const &)) &Octree::query_intersection)
	];
}

}
