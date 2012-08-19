
#include <stddef.h>
#include "vertex.h"
#include "vertex_traits.h"

namespace graphic
{

namespace
{

template<class V> D3DVERTEXELEMENT9 const *get_vertex_descr();

template<> D3DVERTEXELEMENT9 const *
get_vertex_descr<VERTEX_PT>()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{
			0,
			offsetof(VERTEX_PT, position),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},

		{
			0,
			offsetof(VERTEX_PT, texcoords),
			D3DDECLTYPE_FLOAT2,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD,
			0
		},

		D3DDECL_END()
	};

	return descr;
}

template<> D3DVERTEXELEMENT9 const *
get_vertex_descr<VERTEX_PNT>()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{
			0,
			offsetof(VERTEX_PNT, position),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},

		{
			0,
			offsetof(VERTEX_PNT, normal),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_NORMAL,
			0
		},

		{
			0,
			offsetof(VERTEX_PNT, texcoords),
			D3DDECLTYPE_FLOAT2,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD,
			0
		},

		D3DDECL_END()
	};

	return descr;
}

template<> D3DVERTEXELEMENT9 const *
get_vertex_descr<VERTEX_PNTB>()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{
			0,
			offsetof(VERTEX_PNTB, position),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},

		{
			0,
			offsetof(VERTEX_PNTB, normal),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_NORMAL,
			0
		},

		{
			0,
			offsetof(VERTEX_PNTB, texcoords),
			D3DDECLTYPE_FLOAT2,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD,
			0
		},

		{
			0,
			offsetof(VERTEX_PNTB, texutre_blend_weights),
			D3DDECLTYPE_FLOAT4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD,
			1
		},

		D3DDECL_END()
	};

	return descr;
}

template<> D3DVERTEXELEMENT9 const *
get_vertex_descr<VERTEX_PTBNT>()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{
			0,
			offsetof(VERTEX_PTBNT, position),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},

		{
			0,
			offsetof(VERTEX_PTBNT, tangent),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TANGENT,
			0
		},

		{
			0,
			offsetof(VERTEX_PTBNT, binormal),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BINORMAL,
			0
		},

		{
			0,
			offsetof(VERTEX_PTBNT, normal),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_NORMAL,
			0
		},

		{
			0,
			offsetof(VERTEX_PTBNT, texcoords),
			D3DDECLTYPE_FLOAT2,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD,
			0
		},

		D3DDECL_END()
	};

	return descr;
}

template<> D3DVERTEXELEMENT9 const *
get_vertex_descr<VERTEX_PNJWT>()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{
			0,
			offsetof(VERTEX_PNJWT, position),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},

		{
			0,
			offsetof(VERTEX_PNJWT, normal),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_NORMAL,
			0
		},

		{
			0,
			offsetof(VERTEX_PNJWT, joints),
			D3DDECLTYPE_UBYTE4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDINDICES,
			0
		},

        {
			0,
			offsetof(VERTEX_PNJWT, joints) + 4,
			D3DDECLTYPE_UBYTE4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDINDICES,
			1
		},

		{
			0,
			offsetof(VERTEX_PNJWT, weights),
			D3DDECLTYPE_FLOAT4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDWEIGHT,
			0
		},

        {
			0,
			offsetof(VERTEX_PNJWT, weights) + 16,
			D3DDECLTYPE_FLOAT4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDWEIGHT,
			1
		},

		{
			0,
			offsetof(VERTEX_PNJWT, texcoords),
			D3DDECLTYPE_FLOAT2,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD,
			0
		},

		D3DDECL_END()
	};

	return descr;
}

template<> D3DVERTEXELEMENT9 const *
get_vertex_descr<VERTEX_PTBNJWT>()
{
	static D3DVERTEXELEMENT9 descr[] = {
		{
			0,
			offsetof(VERTEX_PTBNJWT, position),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},

		{
			0,
			offsetof(VERTEX_PTBNJWT, tangent),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TANGENT,
			0
		},

		{
			0,
			offsetof(VERTEX_PTBNJWT, binormal),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BINORMAL,
			0
		},

		{
			0,
			offsetof(VERTEX_PTBNJWT, normal),
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_NORMAL,
			0
		},

		{
			0,
			offsetof(VERTEX_PTBNJWT, joints),
			D3DDECLTYPE_UBYTE4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDINDICES,
			0
		},

        {
			0,
			offsetof(VERTEX_PTBNJWT, joints) + 4,
			D3DDECLTYPE_UBYTE4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDINDICES,
			1
		},

		{
			0,
			offsetof(VERTEX_PTBNJWT, weights),
			D3DDECLTYPE_FLOAT4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDWEIGHT,
			0
		},

        {
			0,
			offsetof(VERTEX_PTBNJWT, weights) + 16,
			D3DDECLTYPE_FLOAT4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_BLENDWEIGHT,
			1
		},

		{
			0,
			offsetof(VERTEX_PTBNJWT, texcoords),
			D3DDECLTYPE_FLOAT2,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD,
			0
		},

		D3DDECL_END()
	};

	return descr;
}

template <class V>
class SpecificVertexTraits: public VertexTraits {
public:
	virtual GenericVertex::format_type get_format() const { return V::FORMAT; }
	virtual size_t get_size() const { return sizeof(V); }
	virtual D3DVERTEXELEMENT9 const *get_descr() const { return get_vertex_descr<V>(); }

	static SpecificVertexTraits const &get()
	{
		static SpecificVertexTraits instance;
		return instance;
	}
};

}

template<class V>
VertexTraits const *VertexTraits::get()
{
	return &SpecificVertexTraits<V>::get();
}

template VertexTraits const *VertexTraits::get<VERTEX_PT>();
template VertexTraits const *VertexTraits::get<VERTEX_PNT>();
template VertexTraits const *VertexTraits::get<VERTEX_PNTB>();
template VertexTraits const *VertexTraits::get<VERTEX_PTBNT>();
template VertexTraits const *VertexTraits::get<VERTEX_PNJWT>();
template VertexTraits const *VertexTraits::get<VERTEX_PTBNJWT>();

}
