
#include "directx_error.h"
#include "vertex_array.h"
#include "index_array.h"
#include "material.h"
#include "uber_shader.h"
#include "d3d9_vertex_buffer.h"
#include "d3d9_index_buffer.h"
#include "d3d9_vertex_declarations.h"
#include "d3d9_shader.h"
#include "d3d9_renderer.h"
#include "d3d9_uber_draw_call.h"

namespace graphic
{

D3D9UberDrawCall::D3D9UberDrawCall(D3D9RendererPtr const &renderer):
	UberDrawCall(renderer),
	renderer_(renderer)
{
}

D3D9UberDrawCall::~D3D9UberDrawCall()
{
}

void D3D9UberDrawCall::execute()
{
	HRESULT hr;

	IDirect3DDevice9 *device_ptr = renderer_->get_device();
	D3D9VertexBuffer *vertex_buffer = static_cast<D3D9VertexBuffer *>(vertex_buffer_.get());
	D3D9IndexBuffer *index_buffer = static_cast<D3D9IndexBuffer *>(index_buffer_.get());
	VertexTraits const *vertex_traits = vertex_buffer->get_vertex_traits();
	UberShader::InstancePtr shader_instance = shader_->get_instance(vertex_traits->get_format(), material_);
	D3D9Shader *shader = static_cast<D3D9Shader *>(shader_instance->get_shader().get());

	hr = device_ptr->SetVertexDeclaration(renderer_->get_vertex_declarations()->get(vertex_traits));
	if (FAILED(hr)) throw directx_error(hr);

    hr = device_ptr->SetStreamSource(0, vertex_buffer->get(), 0, vertex_traits->get_size());
	if (FAILED(hr)) throw directx_error(hr);

    hr = device_ptr->SetIndices(index_buffer->get());
	if (FAILED(hr)) throw directx_error(hr);

	if (material_)
	{
		material_->select();

		for (size_t i = 0; i < UberShader::DIFFUSE_TEXTURES_COUNT && i < material_->get_diffuse_textures_count(); ++i)
		{
			shader_instance->diffuse_textures[i]->set(material_->get_diffuse_texture(i));
		}

		if (material_->get_texture_blending_weights_texture())
		{
			shader_instance->texture_blending_weights_texture->set(material_->get_texture_blending_weights_texture());
		}
	}

	if (shader_instance->normal_map_texture)
		shader_instance->normal_map_texture->set(material_ ? material_->get_normal_map_texture() : TexturePtr());

	shader_instance->world_matrix->set(world_matrix_);
	shader_instance->world_view_projection_matrix->set(world_view_projection_matrix_);

	if (shader_instance->joint_matrix_array && joint_matrix_array_.size())
		shader_instance->joint_matrix_array->set(joint_matrix_array_);

	UINT passes_count;
	hr = shader->get()->Begin(&passes_count, D3DXFX_DONOTSAVESTATE);
	if (FAILED(hr)) throw directx_error(hr);

	for(UINT pass = 0; pass < passes_count; ++pass)
	{
		hr = shader->get()->BeginPass(pass);
		if (FAILED(hr)) throw directx_error(hr);

		hr = device_ptr->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, minimal_vertex_index_, vertices_count_,
			first_index_offset_, triangles_count_);
		if (FAILED(hr)) throw directx_error(hr);

		hr = shader->get()->EndPass();
		if (FAILED(hr)) throw directx_error(hr);
	}

	hr = shader->get()->End();
	if (FAILED(hr)) throw directx_error(hr);
}

}
