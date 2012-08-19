
#include <luabind/luabind.hpp>
#include "memory_mesh.h"

namespace graphic
{

namespace
{

boost::shared_ptr<MemoryMesh> downcast_to_memory_mesh(boost::shared_ptr<Mesh> const &mesh)
{
	return boost::static_pointer_cast<MemoryMesh>(mesh);
}

}

void MemoryMesh::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<MemoryMesh, Mesh, boost::shared_ptr<MemoryMesh> >("MemoryMesh")
		.scope
		[
			class_<std::vector<vertex_type> >("VertexBuffer")
			.def("size", &std::vector<vertex_type>::size)
			.def("at", (vertex_type &(std::vector<vertex_type>::*)(size_t)) &std::vector<vertex_type>::at),

			class_<std::vector<index_type> >("IndexBuffer")
			.def("size", &std::vector<index_type>::size)
			.def("at", (index_type &(std::vector<index_type>::*)(size_t)) &std::vector<index_type>::at),

			def("downcast_to_memory_mesh", &downcast_to_memory_mesh)
		]
		.def_readwrite("vertices", &MemoryMesh::vertices_)
		.def_readwrite("indices", &MemoryMesh::indices_)
		.def("transform_vertices", &MemoryMesh::transform_vertices)
	];
}

}

