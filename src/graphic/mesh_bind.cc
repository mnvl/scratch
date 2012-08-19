
#include <luabind/luabind.hpp>
#include <luabind/iterator_policy.hpp>
#include "mesh.h"

namespace graphic
{

void Mesh::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<Mesh, boost::shared_ptr<Mesh> >("Mesh")
		.scope
		[
			class_<Triangles>("Triangles")
			.def_readwrite("material_ptr", &Triangles::material_ptr)
			.def_readwrite("minimal_vertex_index", &Triangles::minimal_vertex_index)
			.def_readwrite("vertices_count", &Triangles::vertices_count)
			.def_readwrite("first_index_offset", &Triangles::first_index_offset)
			.def_readwrite("triangles_count", &Triangles::triangles_count)
		]
		.def_readonly("triangles_list", &Mesh::triangles_list, return_stl_iterator)
	];
}

}

