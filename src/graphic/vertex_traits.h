#pragma once

#include <d3d9.h>
#include "generic_vertex.h"

namespace graphic
{

class VertexTraits {
public:
	virtual GenericVertex::format_type get_format() const = 0;
	virtual size_t get_size() const = 0;
	virtual D3DVERTEXELEMENT9 const *get_descr() const = 0;

	template<class V>
	static VertexTraits const *get();
};

}

