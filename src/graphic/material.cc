
#include <luabind/luabind.hpp>
#include "directx_error.h"
#include <util/logger.h>
#include "material.h"

namespace graphic
{

Material::Material(RendererPtr const &renderer_ptr):
	renderer_(renderer_ptr),
	cull_mode_(CULL_NONE),
	zwrite_enabled_(true),
	alpha_blend_enabled_(false),
	src_blend_mode_(BLEND_ONE),
	dest_blend_mode_(BLEND_ZERO)
{
}

Material::~Material()
{
}

void Material::set_diffuse_texture(TexturePtr const &texture, size_t index) {
	if (diffuse_textures_.size() <= index) diffuse_textures_.resize(index + 1);
	diffuse_textures_[index] = texture;
}

void Material::set_diffuse_texture(std::string const &filename, size_t index)
{
	if (diffuse_textures_.size() <= index) diffuse_textures_.resize(index + 1);

	if (renderer_)
	{
		diffuse_textures_[index] = Texture::load_from_file(renderer_, filename);
	}
	else
	{
		LOG_WARN("material without renderer: set_diffuse_texture() will create texture without renderer");
		diffuse_textures_[index].reset(new Texture(filename));
	}
}

void Material::set_texture_blending_weights_texture(std::string const &filename)
{
	texture_blending_weights_texture_ = Texture::load_from_file(renderer_, filename);
}

void Material::set_normal_map_texture(std::string const &filename)
{
	if (renderer_)
	{
		normal_map_texture_ = Texture::load_from_file(renderer_, filename);
	}
	else
	{
		LOG_WARN("material without renderer: set_normal_map_texture() will create texture without renderer");
		normal_map_texture_.reset(new Texture(filename));
	}
}

void Material::select()
{
	LOG_WARN("selecting material without renderer has no effect");
}


}

