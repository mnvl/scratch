
#include <SDL.h>
#include <math/vec.h>
#include <luabind/luabind.hpp>

namespace util
{

namespace
{

struct dummy {};

math::vec<2> SDL_GetMousePosition() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return math::vec<2>((math::scalar) x, (math::scalar) y);
}

math::vec<3, bool> SDL_GetMouseButtons() {
	Uint8 state;
	state = SDL_GetMouseState(0, 0);
	return math::vec<3, bool>((state & 1) != 0, (state & 2) != 0, (state & 4) != 0);
}

}

void bind_SDL(lua_State* L)
{
	using namespace luabind;

	module(L, "SDL")
	[
		class_<dummy>("const")
		.enum_("")
		[
			value("SDL_INIT_EVERYTHING", SDL_INIT_EVERYTHING),
			value("SDL_MOUSEMOTION", SDL_MOUSEMOTION),
			value("SDL_NOEVENT", SDL_NOEVENT),
			value("SDL_QUIT", SDL_QUIT),
			value("SDL_RESIZABLE", SDL_RESIZABLE),
			value("SDL_VIDEORESIZE", SDL_VIDEORESIZE)
		],

		class_<SDL_Surface>("SDL_Surface"),

		class_<SDL_Event>("SDL_Event")
		.def_readonly("type", &SDL_Event::type)
		.def_readonly("motion", &SDL_Event::motion)
		.def(constructor<>()),

		class_<SDL_MouseMotionEvent>("SDL_MouseMotionEvent")
		.def_readonly("x", &SDL_MouseMotionEvent::x)
		.def_readonly("y", &SDL_MouseMotionEvent::y),

		class_<SDL_MouseButtonEvent>("SDL_MouseButtonEvent"),

		def("SDL_Init", &SDL_Init),
		def("SDL_GetError", &SDL_GetError),
		def("SDL_Quit", &SDL_Quit),

		def("SDL_SetVideoMode", &SDL_SetVideoMode),

		def("SDL_GetMousePosition", &SDL_GetMousePosition),
		def("SDL_GetMouseButtons", &SDL_GetMouseButtons),
		def("SDL_PollEvent", &SDL_PollEvent),

		def("SDL_GetTicks", &SDL_GetTicks),
		def("SDL_Delay", &SDL_Delay)
	];
}

}

