
#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "texture.h"
#include "material.h"
#include "renderer.h"
#include "draw_call.h"
#include "uber_draw_call.h"
#include "d3d9_renderer.h"

namespace graphic
{

void Renderer::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<Renderer, RendererPtr>("Renderer")
		.def("get_screen_width", &Renderer::get_screen_width)
		.def("get_screen_height", &Renderer::get_screen_height)
		.def("resize", &Renderer::resize)
		.def("postconstruct", &Renderer::postconstruct)
		.def("begin_frame", &Renderer::begin_frame)
		.def("finish_frame", &Renderer::finish_frame)
		.def("create_texture", &Renderer::create_texture)
		.def("create_material", &Renderer::create_material)
		.def("create_vertex_array_pt", &Renderer::create_vertex_array<VERTEX_PT>)
		.def("create_vertex_array_pnt", &Renderer::create_vertex_array<VERTEX_PNT>)
		.def("create_vertex_array_pnjwt", &Renderer::create_vertex_array<VERTEX_PNJWT>)
		.def("create_index_array_16", &Renderer::create_index_array<INDEX_U16>)
		.def("create_draw_call", &Renderer::create_draw_call)
		.def("create_uber_draw_call", &Renderer::create_uber_draw_call),

		class_<D3D9Renderer, Renderer, RendererPtr>("D3D9Renderer")
		.def(constructor<int, int, bool>())
    ];
}

}