#pragma once

#include <SDL.h>
#include <luabind/lua_include.hpp>
#include <boost/noncopyable.hpp>
#include "forward.h"
#include "pixel.h"

namespace graphic
{

class Texture: public boost::noncopyable {
public:
	Texture(std::string const &filename);
	Texture(int width, int height, unsigned pixel_format,
		bool dynamic = false, bool writeonly = false);
	virtual ~Texture();

	virtual const void *lock() const;
	virtual void unlock() const;
	virtual void *lock(bool discard);
	virtual void unlock();

	// Return resources calling SDL_FreeSurface() for returned
	// surface and unlock() for texture.
	SDL_Surface *lock_as_surface(bool discard);
	SDL_Surface const *lock_as_surface() const;

	int get_width() const { return width_; }
	int get_height() const { return height_; }
	int get_pixel_format() const { return pixel_format_; }
	std::string const &get_filename() const { return filename_; }

	static TexturePtr load_from_surface(RendererPtr const &renderer, SDL_Surface *surface);
	static TexturePtr load_from_file(RendererPtr const &renderer, std::string const &filename);

	// Only BMP format is supported.
	static void save_to_file(TexturePtr const &texture, std::string const &filename);

	static void bind(lua_State *L);

protected:
	int width_, height_;
	unsigned pixel_format_;
	bool dynamic_;
	bool writeonly_;
	std::string filename_;
};

}
