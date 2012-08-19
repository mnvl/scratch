
#include <luabind/luabind.hpp>
#include "vertex.h"

namespace graphic
{

void bind_vertex(lua_State *L)
{
	using namespace luabind;

	module(L, "graphic")
	[
		class_<VERTEX_PT>("vertex_pt")
		.def(constructor<>())
		.def_readwrite("position", &VERTEX_PT::position)
		.def_readwrite("texcoords", &VERTEX_PT::texcoords),

		class_<VERTEX_PNT>("vertex_pnt")
		.def(constructor<>())
		.def_readwrite("position", &VERTEX_PNT::position)
		.def_readwrite("normal", &VERTEX_PNT::normal)
		.def_readwrite("texcoords", &VERTEX_PNT::texcoords),

		class_<VERTEX_PNJWT>("vertex_pnjwt")
		.def(constructor<>())
		.def_readwrite("position", &VERTEX_PNJWT::position)
		.def_readwrite("normal", &VERTEX_PNJWT::normal)
		.def_readwrite("joints", &VERTEX_PNJWT::normal)
		.def_readwrite("wegiths", &VERTEX_PNJWT::normal)
		.def_readwrite("texcoords", &VERTEX_PNJWT::texcoords),

		class_<VERTEX_PTBNJWT>("vertex_ptbnjwt")
		.def(constructor<>())
		.def_readwrite("position", &VERTEX_PTBNJWT::position)
		.def_readwrite("tangent", &VERTEX_PTBNJWT::tangent)
		.def_readwrite("binormal", &VERTEX_PTBNJWT::binormal)
		.def_readwrite("normal", &VERTEX_PTBNJWT::normal)
		.def_readwrite("joints", &VERTEX_PTBNJWT::normal)
		.def_readwrite("wegiths", &VERTEX_PTBNJWT::normal)
		.def_readwrite("texcoords", &VERTEX_PTBNJWT::texcoords)
	 ];
}

}
