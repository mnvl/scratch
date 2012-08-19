
#include <luabind/luabind.hpp>
#include "directx_error.h"
#include <util/logger.h>
#include "d3d9_renderer.h"
#include "d3d9_material.h"

namespace graphic
{

D3D9Material::D3D9Material(D3D9RendererPtr const &renderer_ptr):
	Material(renderer_ptr),
	renderer_(renderer_ptr)
{
}

D3D9Material::~D3D9Material()
{
}

void D3D9Material::select()
{
	static DWORD cull_mode_table[] = { D3DCULL_CW, D3DCULL_CCW, D3DCULL_NONE };

	static DWORD blend_mode_table[] = {
		D3DBLEND_ZERO,
		D3DBLEND_ONE,
		D3DBLEND_SRCALPHA,
		D3DBLEND_INVSRCALPHA,
		D3DBLEND_DESTALPHA,
		D3DBLEND_INVDESTALPHA
	};

	HRESULT hr = 0;

	hr = renderer_->get_device()->SetRenderState(D3DRS_CULLMODE, cull_mode_table[cull_mode_]);
	if (FAILED(hr)) throw directx_error(hr);

	hr = renderer_->get_device()->SetRenderState(D3DRS_ZWRITEENABLE, zwrite_enabled_);
	if (FAILED(hr)) throw directx_error(hr);

	hr = renderer_->get_device()->SetRenderState(D3DRS_ALPHABLENDENABLE, alpha_blend_enabled_);
	if (FAILED(hr)) throw directx_error(hr);

	if (alpha_blend_enabled_)
	{
		hr = renderer_->get_device()->SetRenderState(D3DRS_SRCBLEND, blend_mode_table[src_blend_mode_]);
		if (FAILED(hr)) throw directx_error(hr);

		hr = renderer_->get_device()->SetRenderState(D3DRS_DESTBLEND, blend_mode_table[dest_blend_mode_]);
		if (FAILED(hr)) throw directx_error(hr);
	}
}

}

