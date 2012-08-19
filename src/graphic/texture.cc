
#include <SDL_image.h>
#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "renderer.h"
#include "texture.h"

namespace graphic
{

Texture::Texture(std::string const &filename):
	filename_(filename),
	width_(-1),
	height_(-1),
	pixel_format_(-1),
	dynamic_(false),
	writeonly_(false) {
}

Texture::Texture(int width, int height, unsigned pixel_format,
	bool dynamic, bool writeonly):
	width_(width),
	height_(height),
	pixel_format_(pixel_format),
	dynamic_(dynamic),
	writeonly_(writeonly)
{
}

Texture::~Texture()
{
}

const void *Texture::lock() const {
	return 0;
}

SDL_Surface const *Texture::lock_as_surface() const {
	PixelFormat const &pixel_format = PixelFormat::get(get_pixel_format());

	return SDL_CreateRGBSurfaceFrom(const_cast<void *>(lock()),width_, height_,
		pixel_format.bits, pixel_format.bits * width_ / 8,
		pixel_format.masks.r, pixel_format.masks.g, pixel_format.masks.b,
		pixel_format.masks.a);
}

void Texture::unlock() const {
}

void *Texture::lock(bool discard) {
	return 0;
}

SDL_Surface *Texture::lock_as_surface(bool discard) {
	PixelFormat const &pixel_format = PixelFormat::get(get_pixel_format());

	return SDL_CreateRGBSurfaceFrom(lock(discard), width_, height_,
		pixel_format.bits, pixel_format.bits * width_ / 8,
		pixel_format.masks.r, pixel_format.masks.g, pixel_format.masks.b,
		pixel_format.masks.a);
}

void Texture::unlock() {
}

TexturePtr Texture::load_from_surface(RendererPtr const &renderer, SDL_Surface *surface) {
	TexturePtr texture = renderer->create_texture(surface->w, surface->h,
		false ? PixelFormat::DEFAULT_ALPHA_FORMAT : PixelFormat::DEFAULT_FORMAT,
		true, false);

	SDL_Surface *texture_surface = texture->lock_as_surface(true);

	int status = SDL_BlitSurface(surface, 0, texture_surface, 0);
	if (status < 0) throw std::runtime_error(SDL_GetError());

	SDL_FreeSurface(texture_surface);
	texture->unlock();

	return texture;
}

TexturePtr Texture::load_from_file(RendererPtr const &renderer, std::string const &filename) {
	SDL_Surface *surface = IMG_Load(filename.c_str());
	if (!surface) throw std::runtime_error(SDL_GetError());

	TexturePtr texture = load_from_surface(renderer, surface);
	texture->filename_ = filename;

	SDL_FreeSurface(surface);

	return texture;
}

void Texture::save_to_file(TexturePtr const &texture, std::string const &filename) {
	SDL_Surface *texture_surface = texture->lock_as_surface(true);

	SDL_SaveBMP(texture_surface, filename.c_str());

	SDL_FreeSurface(texture_surface);
	texture->unlock();
}

void Texture::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "graphic")
	[
		class_<Texture, TexturePtr >("Texture")
	];
}

}
