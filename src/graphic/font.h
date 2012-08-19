#pragma once

#include <SDL_ttf.h>
#include <boost/noncopyable.hpp>
#include <luabind/lua_include.hpp>
#include <math/vec.h>
#include "forward.h"

namespace graphic
{

class Font: public boost::noncopyable {
public:
	Font(RendererPtr const &renderer, std::string const &filename, int height);
	~Font();

	TexturePtr make_texture(std::string const& text, math::vec<4> const &text_color,
		math::vec<4> const &background_color) const;

	// 1) Points must be passed in clockwise order:
	//   p1 --- p2
	//   |       |
	//   p4 --- p3
    //
	// 2) User should set camera for draw call and commit() it.
	DrawCallPtr make_draw_call(math::vec<3> const &p1, math::vec<3> const &p2,
		math::vec<3> const &p3, math::vec<3> const &p4, std::string const &text,
		math::vec<4> const &text_color, math::vec<4> const &background_color) const;

	static unsigned const FLAG_ALIGN_LEFT = 1;
	static unsigned const FLAG_ALIGN_HCENTER = 2;
	static unsigned const FLAG_ALIGN_RIGHT = 4;
	static unsigned const FLAG_ALIGN_TOP = 8;
	static unsigned const FLAG_ALIGN_VCENTER = 16;
	static unsigned const FLAG_ALIGN_BOTTOM = 32;

	// Creates draw call and executes it immediately.
	void draw(math::vec<3> const &p1, math::vec<3> const &p2,
		math::vec<3> const &p3, math::vec<3> const &p4, std::string const &text,
		math::vec<4> const &text_color, math::vec<4> const &background_color,
		CameraPtr const &camera) const;

	static void bind(lua_State *L);

private:
	RendererPtr const renderer_;
	TTF_Font *font_;

	static void init_library();
	static void throw_error();
};

}
