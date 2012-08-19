#pragma once

#include <boost/shared_ptr.hpp>
#include "forward.h"
#include "vertex.h"
#include "vertex_buffer.h"

namespace graphic
{

class Renderer;

template <class T>
class VertexArray {
public:
	typedef T vertex_type;

	VertexArray(VertexBufferPtr const &vertex_buffer):
		vertex_buffer_(vertex_buffer)
	{
	}

	~VertexArray()
	{
	}

	vertex_type *lock(bool discard) { return (vertex_type *) vertex_buffer_->lock(discard); }
	vertex_type const *lock() const { return (vertex_type const *) vertex_buffer_->lock(); }

	void unlock() { vertex_buffer_->unlock(); }
	void unlock() const { vertex_buffer_->unlock(); }

	vertex_type *begin() { return (vertex_type *) vertex_buffer_->begin(); }
	vertex_type const *begin() const { return (vertex_type const *) vertex_buffer_->begin(); }

	vertex_type *end() { return (vertex_type *) vertex_buffer_->end(); }
	vertex_type const *end() const { return (vertex_type const *) vertex_buffer_->end(); }

	vertex_type *at(size_t index) { return begin() + index; }
	vertex_type const *at(size_t index) const { return begin() + index; }

	size_t get_size() const { return vertex_buffer_->get_size(); }
	size_t get_count() const { return get_size() / sizeof(T); }

	VertexBufferPtr const &get_vertex_buffer() { return vertex_buffer_;	}

protected:
	VertexBufferPtr vertex_buffer_;
};

typedef VertexArray<VERTEX_PT> VertexArrayPT;
typedef VertexArray<VERTEX_PNT> VertexArrayPNT;
typedef VertexArray<VERTEX_PNTB> VertexArrayPNTB;
typedef VertexArray<VERTEX_PTBNT> VertexArrayPTBNT;
typedef VertexArray<VERTEX_PNJWT> VertexArrayPNJWT;
typedef VertexArray<VERTEX_PTBNJWT> VertexArrayPTBNJWT;

void bind_vertex_array(lua_State *L);

}
