
#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "vertex_buffer.h"

namespace graphic
{

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<VertexBuffer, VertexBufferPtr>("VertexBuffer")
	];
}

}
