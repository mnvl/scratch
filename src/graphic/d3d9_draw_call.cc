
#include <math/frustum.h>
#include "directx_error.h"
#include "vertex_traits.h"
#include "camera.h"
#include "uber_shader.h"
#include "d3d9_vertex_buffer.h"
#include "d3d9_index_buffer.h"
#include "d3d9_texture.h"
#include "d3d9_vertex_declarations.h"
#include "d3d9_texture.h"
#include "d3d9_material.h"
#include "d3d9_renderer.h"
#include "d3d9_draw_call.h"

namespace graphic
{

D3D9DrawCall::D3D9DrawCall(D3D9RendererPtr const &renderer):
	DrawCall(renderer),
	renderer_(renderer)
{
}

D3D9DrawCall::~D3D9DrawCall()
{
}

void D3D9DrawCall::execute()
{
	if (!triangles_count_) return;

	HRESULT hr;

	IDirect3DDevice9 *device_ptr = renderer_->get_device();
	D3D9VertexBuffer *vertex_buffer = static_cast<D3D9VertexBuffer *>(vertex_buffer_.get());
	D3D9IndexBuffer *index_buffer = static_cast<D3D9IndexBuffer *>(index_buffer_.get());
	VertexTraits const *vertex_traits = vertex_buffer->get_vertex_traits();

	hr = device_ptr->SetVertexShader(0);
	if (FAILED(hr)) throw directx_error(hr);

	hr = device_ptr->SetPixelShader(0);
	if (FAILED(hr)) throw directx_error(hr);

	hr = device_ptr->SetTransform(D3DTS_PROJECTION, (D3DMATRIX const *) &camera_->get_projection_matrix());
	if (FAILED(hr)) throw directx_error(hr);

	hr = device_ptr->SetTransform(D3DTS_VIEW, (D3DMATRIX const *) &camera_->get_view_matrix());
	if (FAILED(hr)) throw directx_error(hr);

    hr = device_ptr->SetTransform(D3DTS_WORLD, (D3DMATRIX const *) &world_matrix_);
	if (FAILED(hr)) throw directx_error(hr);

	hr = device_ptr->SetVertexDeclaration(renderer_->get_vertex_declarations()->get(vertex_traits));
	if (FAILED(hr)) throw directx_error(hr);

    hr = device_ptr->SetStreamSource(0, vertex_buffer->get(), 0, vertex_traits->get_size());
	if (FAILED(hr)) throw directx_error(hr);

	hr = device_ptr->SetTexture(0, static_cast<D3D9Texture *>(material_->get_diffuse_texture().get())->get());
	if (FAILED(hr)) throw directx_error(hr);

	if (index_buffer_)
	{
		hr = device_ptr->SetIndices(index_buffer->get());
		if (FAILED(hr)) throw directx_error(hr);
	}

	if (material_) material_->select();

	if (index_buffer_)
	{
		hr = device_ptr->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, minimal_vertex_index_, vertices_count_,
			first_index_offset_, triangles_count_);
		if (FAILED(hr)) throw directx_error(hr);
	}
	else
	{
		hr = device_ptr->DrawPrimitive(D3DPT_TRIANGLELIST, minimal_vertex_index_, triangles_count_);
		if (FAILED(hr)) throw directx_error(hr);
	}
}

}
