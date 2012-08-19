
#include "directx_error.h"
#include "d3d9_renderer.h"
#include "d3d9_vertex_declarations.h"

namespace graphic
{

D3D9VertexDeclarations::D3D9VertexDeclarations(D3D9Renderer * const renderer_ptr):
	renderer_(renderer_ptr)
{
}

D3D9VertexDeclarations::~D3D9VertexDeclarations()
{
}

IDirect3DVertexDeclaration9 *D3D9VertexDeclarations::get(VertexTraits const *vertex_traits)
{
	traits_to_declarations_t::const_iterator it = traits_to_declarations_.find(vertex_traits);

	if (it == traits_to_declarations_.end())
	{
		IDirect3DVertexDeclaration9 *decl_ptr;

		HRESULT hr = renderer_->get_device()->CreateVertexDeclaration(vertex_traits->get_descr(), &decl_ptr);
		if (FAILED(hr)) throw directx_error(hr);

		traits_to_declarations_.insert(std::make_pair(vertex_traits, VertexDeclarationPtr(decl_ptr)));

		decl_ptr->Release();

		return decl_ptr;
	}
	else
	{
		return it->second.get();
	}
}

}
