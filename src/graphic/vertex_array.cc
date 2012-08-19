
#include <luabind/luabind.hpp>
#include "directx_error.h"
#include "renderer.h"
#include "vertex_array.h"

namespace graphic
{

template class VertexArray<VERTEX_PT>;
template class VertexArray<VERTEX_PNT>;
template class VertexArray<VERTEX_PTBNT>;
template class VertexArray<VERTEX_PNJWT>;
template class VertexArray<VERTEX_PTBNJWT>;

void bind_vertex_array(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<VertexArrayPT, boost::shared_ptr<VertexArrayPT> >("VertexArrayPT")
		.def("lock", (VERTEX_PT *(VertexArrayPT::*)(bool)) &VertexArrayPT::lock)
		.def("unlock", (void (VertexArrayPT::*)()) &VertexArrayPT::unlock)
		.def("at", (VERTEX_PT *(VertexArrayPT::*)(size_t)) &VertexArrayPT::at)
		.def("get_vertex_buffer", &VertexArrayPT::get_vertex_buffer),

		class_<VertexArrayPNT, boost::shared_ptr<VertexArrayPNT> >("VertexArrayPNT")
		.def("lock", (VERTEX_PNT *(VertexArrayPNT::*)(bool)) &VertexArrayPNT::lock)
		.def("unlock", (void (VertexArrayPNT::*)()) &VertexArrayPNT::unlock)
		.def("at", (VERTEX_PNT *(VertexArrayPNT::*)(size_t)) &VertexArrayPNT::at)
		.def("get_vertex_buffer", &VertexArrayPNT::get_vertex_buffer),

		class_<VertexArrayPNJWT, boost::shared_ptr<VertexArrayPNJWT> >("VertexArrayPNJWT")
		.def("lock", (VERTEX_PNJWT *(VertexArrayPNJWT::*)(bool)) &VertexArrayPNJWT::lock)
		.def("unlock", (void (VertexArrayPNJWT::*)()) &VertexArrayPNJWT::unlock)
		.def("at", (VERTEX_PNJWT *(VertexArrayPNJWT::*)(size_t)) &VertexArrayPNJWT::at)
		.def("get_vertex_buffer", &VertexArrayPNJWT::get_vertex_buffer),

		class_<VertexArrayPTBNJWT, boost::shared_ptr<VertexArrayPTBNJWT> >("VertexArrayPTBNJWT")
		.def("lock", (VERTEX_PTBNJWT *(VertexArrayPTBNJWT::*)(bool)) &VertexArrayPTBNJWT::lock)
		.def("unlock", (void (VertexArrayPTBNJWT::*)()) &VertexArrayPTBNJWT::unlock)
		.def("at", (VERTEX_PTBNJWT *(VertexArrayPTBNJWT::*)(size_t)) &VertexArrayPTBNJWT::at)
		.def("get_vertex_buffer", &VertexArrayPTBNJWT::get_vertex_buffer)
	];
}

}
