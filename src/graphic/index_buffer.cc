
#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "index_buffer.h"

namespace graphic
{

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<IndexBuffer, IndexBufferPtr>("IndexBuffer")
	];
}

}
