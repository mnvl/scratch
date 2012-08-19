
#include <boost/lexical_cast.hpp>
#include <luabind/luabind.hpp>
#include "texture.h"
#include "material.h"
#include "uber_draw_call.h"
#include "renderer.h"
#include "font.h"

#if defined(max)
#undef max
#endif

namespace graphic
{

Font::Font(RendererPtr const &renderer, std::string const &filename, int height):
	renderer_(renderer)
{
	init_library();

	font_ = TTF_OpenFont(filename.c_str(), height);
	if (!font_) throw_error();
}

Font::~Font() {
	TTF_CloseFont(font_);
}

TexturePtr Font::make_texture(std::string const& text, math::vec<4> const &text_color,
	math::vec<4> const &background_color) const {

	SDL_Color c1;
	c1.r = Uint8(text_color.r * 255);
	c1.g = Uint8(text_color.g * 255);
	c1.b = Uint8(text_color.b * 255);
	c1.unused = Uint8(text_color.a * 255);

	SDL_Color c2;
	c2.r = Uint8(background_color.r * 255);
	c2.g = Uint8(background_color.g * 255);
	c2.b = Uint8(background_color.b * 255);
	c2.unused = Uint8(background_color.a * 255);

	SDL_Surface *surface = TTF_RenderUTF8_Shaded(font_, text.c_str(), c1, c2);

	TexturePtr texture = Texture::load_from_surface(renderer_, surface);

	SDL_FreeSurface(surface);

	return texture;
}

DrawCallPtr Font::make_draw_call(math::vec<3> const &p1, math::vec<3> const &p2,
	math::vec<3> const &p3, math::vec<3> const &p4, std::string const &text,
	math::vec<4> const &text_color, math::vec<4> const &background_color) const {
	graphic::TexturePtr texture = make_texture(text, text_color, background_color);

	graphic::MaterialPtr material = renderer_->create_material();
	material->set_diffuse_texture(texture);

	boost::shared_ptr<graphic::VertexArrayPT> vertex_array = renderer_->create_vertex_array<graphic::VERTEX_PT>(4);

	{
		graphic::VERTEX_PT *v = vertex_array->lock(true);

		v->position = p1;
		v->texcoords.set(0, 0);
		++v;

		v->position = p2;
		v->texcoords.set(1, 0);
		++v;

		v->position = p3;
		v->texcoords.set(1, 1);
		++v;

		v->position = p4;
		v->texcoords.set(0, 1);
		++v;

		vertex_array->unlock();
	}

	boost::shared_ptr<graphic::IndexArray16> index_array = renderer_->create_index_array<graphic::INDEX_U16>(6);

	{
		graphic::INDEX_U16 *i = index_array->lock(true);

		*i++ = 0;
		*i++ = 1;
		*i++ = 2;
		*i++ = 0;
		*i++ = 2;
		*i++ = 3;

		index_array->unlock();
	}

	math::matrix<4,4> world_matrix;
	world_matrix.identity();

	graphic::UberDrawCallPtr draw_call = renderer_->create_uber_draw_call();

	draw_call->set_always_visible(true);
	draw_call->set_vertex_buffer(vertex_array->get_vertex_buffer());
	draw_call->set_index_buffer(index_array->get_index_buffer());
	draw_call->set_material(material);
	draw_call->set_vertices_count(4);
	draw_call->set_triangles_count(2);

	return draw_call;
}

void Font::draw(math::vec<3> const &p1, math::vec<3> const &p2,
	math::vec<3> const &p3, math::vec<3> const &p4, std::string const &text,
	math::vec<4> const &text_color, math::vec<4> const &background_color,
	CameraPtr const &camera) const
{
	DrawCallPtr draw_call = make_draw_call(p1, p2, p3, p4, text, text_color, background_color);
	draw_call->set_camera(camera);
	draw_call->set_always_visible(true);
	draw_call->commit();
	draw_call->execute();
}

void Font::init_library() {
	struct Guard {
		Guard() {
			if (TTF_Init() == -1) {
				throw_error();
			}
		}

		~Guard() {
			TTF_Quit();
		}
	};

	static Guard guard;
}

void Font::throw_error() {
	throw std::runtime_error(std::string("SDL_ttf error ") + TTF_GetError());
}

void Font::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<Font, boost::shared_ptr<Font> >("Font")
		.enum_("flags")
		[
			value("FLAG_ALIGN_LEFT", FLAG_ALIGN_LEFT),
			value("FLAG_ALIGN_HCENTER", FLAG_ALIGN_HCENTER),
			value("FLAG_ALIGN_RIGHT", FLAG_ALIGN_RIGHT),
			value("FLAG_ALIGN_TOP", FLAG_ALIGN_TOP),
			value("FLAG_ALIGN_VCENTER", FLAG_ALIGN_VCENTER),
			value("FLAG_ALIGN_BOTTOM", FLAG_ALIGN_BOTTOM)
		]
		.def(constructor<RendererPtr const &, std::string const &, int>())
		.def("make_texture", &Font::make_texture)
		.def("make_draw_call", &Font::make_draw_call)
		.def("draw", &Font::draw)
    ];
}

}
