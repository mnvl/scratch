
#include <boost/lexical_cast.hpp>
#include <util/get_sources_path.h>
#include <util/logger.h>
#include "vertex.h"
#include "material.h"
#include "renderer.h"
#include "uber_shader.h"

namespace graphic
{

UberShader::Instance::Instance(RendererPtr const &renderer, unsigned flags)
{
	shader_ = renderer->create_shader();

	if (flags & FLAG_VERTEX_N_ENABLED)
		shader_->add_preprocessor_macro("VERTEX_N_ENABLED");

	if (flags & FLAG_VERTEX_TB_ENABLED)
		shader_->add_preprocessor_macro("VERTEX_TB_ENABLED");

	if (flags & FLAG_VERTEX_JW_ENABLED)
		shader_->add_preprocessor_macro("VERTEX_JW_ENABLED");

	if (flags & FLAG_SKELETAL_ANIMATION_ENABLED)
		shader_->add_preprocessor_macro("SKELETAL_ANIMATION_ENABLED");

	if (flags & FLAG_LIGHTING_ENABLED)
		shader_->add_preprocessor_macro("LIGHTING_ENABLED");

	if (flags & FLAG_NORMAL_MAP_ENABLED)
		shader_->add_preprocessor_macro("NORMAL_MAP_ENABLED");

	if (flags & FLAG_TEXTURE_BLENDING_ENABLED)
		shader_->add_preprocessor_macro("TEXTURE_BLENDING_ENABLED");

	if (flags & FLAG_TEXUTRE_BLENDING_WEIGHTS_IN_VERTEX)
		shader_->add_preprocessor_macro("TEXUTRE_BLENDING_WEIGHTS_IN_VERTEX");

	if (flags & FLAG_TEXUTRE_BLENDING_WEIGHTS_IN_TEXTURE)
		shader_->add_preprocessor_macro("TEXUTRE_BLENDING_WEIGHTS_IN_TEXTURE");

	shader_->load_from_file(util::get_graphic_sources_path() + "uber_shader.fx");

	world_view_projection_matrix = shader_->get_parameter("world_view_projection_matrix");
	world_matrix = shader_->get_parameter("world_matrix");

	if (flags & FLAG_TEXTURE_BLENDING_ENABLED)
	{
		for (size_t i = 0; i < DIFFUSE_TEXTURES_COUNT; ++i)
		{
			diffuse_textures[i] = shader_->get_parameter("diffuse_texture_" + boost::lexical_cast<std::string>(i));
		}

		if (flags & FLAG_TEXUTRE_BLENDING_WEIGHTS_IN_TEXTURE)
		{
			texture_blending_weights_texture = shader_->get_parameter("texture_blending_weights_texture");
		}
	}
	else
	{
		diffuse_textures[0] = shader_->get_parameter("diffuse_texture_0");
	}

	if (flags & FLAG_LIGHTING_ENABLED)
		point_lights = shader_->get_parameter("point_lights");

	if (flags & FLAG_NORMAL_MAP_ENABLED)
		normal_map_texture = shader_->get_parameter("normal_map_texture");

	if (flags & FLAG_SKELETAL_ANIMATION_ENABLED)
		joint_matrix_array = shader_->get_parameter("joint_matrix_array");
}

UberShader::Instance::~Instance()
{
}

UberShader::UberShader(RendererPtr const &renderer):
	renderer_(renderer),
	lighting_enabled_(false)
{

	build_instance(VERTEX_PT::FORMAT, 0);

	for (int i = 0; i <= 1; ++i)
	{
		unsigned lighting_enabled_flag = i ? FLAG_LIGHTING_ENABLED : 0;

		build_instance(VERTEX_PNT::FORMAT,
			lighting_enabled_flag | FLAG_VERTEX_N_ENABLED);

		build_instance(VERTEX_PNT::FORMAT,
			lighting_enabled_flag | FLAG_VERTEX_N_ENABLED | FLAG_TEXTURE_BLENDING_ENABLED
			| FLAG_TEXUTRE_BLENDING_WEIGHTS_IN_TEXTURE);

		build_instance(VERTEX_PNTB::FORMAT,
			lighting_enabled_flag | FLAG_VERTEX_N_ENABLED | FLAG_TEXTURE_BLENDING_ENABLED
			| FLAG_TEXUTRE_BLENDING_WEIGHTS_IN_VERTEX);

		build_instance(VERTEX_PTBNT::FORMAT,
			lighting_enabled_flag | FLAG_VERTEX_TB_ENABLED | FLAG_VERTEX_N_ENABLED | FLAG_NORMAL_MAP_ENABLED);

		build_instance(VERTEX_PNJWT::FORMAT,
			lighting_enabled_flag | FLAG_VERTEX_N_ENABLED  | FLAG_VERTEX_JW_ENABLED | FLAG_SKELETAL_ANIMATION_ENABLED);

		build_instance(VERTEX_PTBNJWT::FORMAT,
			lighting_enabled_flag | FLAG_VERTEX_TB_ENABLED | FLAG_VERTEX_N_ENABLED | FLAG_VERTEX_JW_ENABLED
			| FLAG_NORMAL_MAP_ENABLED | FLAG_SKELETAL_ANIMATION_ENABLED);
	}
}

UberShader::~UberShader()
{
}

UberShader::InstancePtr const &UberShader::get_instance(GenericVertex::format_type format,
	MaterialPtr const &material) const
{
	InstanceKey key;
	key.vertex_format = format;
	key.lighting_enabled = lighting_enabled_;
	key.texture_blending_weights_in_vertex = ((format & GenericVertex::FORMAT_TEXTURE_BLEND_WEIGHTS) != 0);
	key.texture_blending_weights_in_texture = !!material->get_texture_blending_weights_texture();
	key.texture_blending_enabled = key.texture_blending_weights_in_vertex || key.texture_blending_weights_in_texture;

	instances_type::const_iterator it = instances_.find(key);
	if (it == instances_.end()) throw std::logic_error("vertex format and material are not supported by uber shader");
	return it->second;
}

void UberShader::build_instance(GenericVertex::format_type format, unsigned flags)
{
	RendererPtr renderer(renderer_);

	InstanceKey key;
	key.vertex_format = format;
	key.lighting_enabled = !!(flags & FLAG_LIGHTING_ENABLED);
	key.texture_blending_enabled = !!(flags & FLAG_TEXTURE_BLENDING_ENABLED);
	key.texture_blending_weights_in_vertex = !!(format & GenericVertex::FORMAT_TEXTURE_BLEND_WEIGHTS);
	key.texture_blending_weights_in_texture = !!(flags & FLAG_TEXUTRE_BLENDING_WEIGHTS_IN_TEXTURE);

	LOG_INFO("*** building uber shader: format = " << format << ", flags = " << flags);
	instances_.insert(std::make_pair(key, InstancePtr(new Instance(renderer, flags))));
}

}
