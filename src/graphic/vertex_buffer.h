#pragma once

#include <boost/noncopyable.hpp>
#include <luabind/lua_include.hpp>
#include "forward.h"

namespace graphic
{

class D3D9VertexBuffer;

class VertexBuffer: public boost::noncopyable {
public:
	virtual ~VertexBuffer();

	virtual const void *lock() const = 0;
	virtual void unlock() const = 0;
	virtual void *lock(bool discard) = 0;
	virtual void unlock() = 0;

	virtual size_t get_size() const = 0;

	virtual void *begin() = 0;
	virtual void const *begin() const = 0;
	virtual void *end() = 0;
	virtual void const *end() const = 0;

	static void bind(lua_State *L);
};

}
