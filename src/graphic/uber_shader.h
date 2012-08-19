#pragma once

#include <boost/unordered_map.hpp>
#include "forward.h"
#include "shader.h"
#include "generic_vertex.h"

namespace graphic
{

class UberShader: public boost::noncopyable {
public:
	static unsigned const FLAG_VERTEX_N_ENABLED = 1;
	static unsigned const FLAG_VERTEX_TB_ENABLED = 2;
	static unsigned const FLAG_VERTEX_JW_ENABLED = 4;
	static unsigned const FLAG_SKELETAL_ANIMATION_ENABLED = 8;
	static unsigned const FLAG_LIGHTING_ENABLED = 16;
	static unsigned const FLAG_NORMAL_MAP_ENABLED = 32;
	static unsigned const FLAG_TEXTURE_BLENDING_ENABLED = 64;
	static unsigned const FLAG_TEXUTRE_BLENDING_WEIGHTS_IN_VERTEX = 128;
	static unsigned const FLAG_TEXUTRE_BLENDING_WEIGHTS_IN_TEXTURE = 256;

	static size_t const DIFFUSE_TEXTURES_COUNT = 4;

	static size_t const MAX_POINT_LIGHTS = 5;

#pragma pack(push, 1)
	struct POINT_LIGHT
	{
		math::vec<3> position;
		math::vec<3> color;
		math::scalar inv_attenuation;
	};

	struct POINT_LIGHTS
	{
		POINT_LIGHT items[MAX_POINT_LIGHTS];
	};
#pragma pack(pop)

	class Instance {
	public:
		// from uber_shader.fx
		Shader::ParameterPtr world_view_projection_matrix;
		Shader::ParameterPtr world_matrix;
		Shader::ParameterPtr diffuse_textures[DIFFUSE_TEXTURES_COUNT];
		Shader::ParameterPtr texture_blending_weights_texture;

		// from point_light.fx
		Shader::ParameterPtr point_lights;

		// from normal_map.fx
		Shader::ParameterPtr normal_map_texture;

		// from skeletal_animation.fx
		Shader::ParameterPtr joint_matrix_array;

		Instance(RendererPtr const &renderer, unsigned flags);
		~Instance();

		ShaderPtr const &get_shader() const { return shader_; }

	private:
		ShaderPtr shader_;
	};

	typedef boost::shared_ptr<Instance> InstancePtr;

	UberShader(RendererPtr const &renderer);
	virtual ~UberShader();

	virtual InstancePtr const &get_instance(GenericVertex::format_type format, MaterialPtr const &material) const;
	void set_lighting_enabled(bool flag) { lighting_enabled_ = true; }

private:
	struct InstanceKey {
		GenericVertex::format_type vertex_format;
		bool lighting_enabled;
		bool texture_blending_enabled;
		bool texture_blending_weights_in_vertex;
		bool texture_blending_weights_in_texture;

		struct Hash
		{
			inline size_t operator ()(InstanceKey const &ik) const
			{
				size_t hash = (size_t) ik.vertex_format;
				hash = (hash << 1) | (size_t) ik.lighting_enabled;
				hash = (hash << 1) | (size_t) ik.texture_blending_enabled;
				hash = (hash << 1) | (size_t) ik.texture_blending_weights_in_vertex;
				hash = (hash << 1) | (size_t) ik.texture_blending_weights_in_texture;
				return hash;
			}
		};

		struct Equal
		{
			inline bool operator ()(InstanceKey const &left, InstanceKey const &right) const
			{
				return left.vertex_format == right.vertex_format
					&& left.lighting_enabled == right.lighting_enabled
					&& left.texture_blending_enabled == right.texture_blending_enabled
					&& left.texture_blending_weights_in_vertex == right.texture_blending_weights_in_vertex
					&& left.texture_blending_weights_in_texture == right.texture_blending_weights_in_texture;
			}
		};
	};

	typedef boost::unordered_map<InstanceKey, InstancePtr, InstanceKey::Hash, InstanceKey::Equal> instances_type;

	RendererWeakPtr renderer_;
	instances_type instances_;
	bool lighting_enabled_;

	void build_instance(GenericVertex::format_type format, unsigned flags);
};

}
