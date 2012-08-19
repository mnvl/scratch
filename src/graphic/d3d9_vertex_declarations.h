#pragma once

#include <map>
#include <d3d9.h>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include "vertex.h"
#include "vertex_traits.h"

namespace graphic
{

class D3D9Renderer;

class D3D9VertexDeclarations {
public:
    D3D9VertexDeclarations(D3D9Renderer * const renderer_ptr);
    ~D3D9VertexDeclarations();

    template<class V> inline
    IDirect3DVertexDeclaration9 *get()
	{
		return get(VertexTraits::get<V>());
	}

	IDirect3DVertexDeclaration9 *get(VertexTraits const *vertex_traits);

private:
	D3D9Renderer * const renderer_;

	typedef boost::intrusive_ptr<IDirect3DVertexDeclaration9> VertexDeclarationPtr;
	typedef std::map<VertexTraits const *,  VertexDeclarationPtr> traits_to_declarations_t;

	traits_to_declarations_t traits_to_declarations_;
};

}
