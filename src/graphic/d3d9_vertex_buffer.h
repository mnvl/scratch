#pragma once

#include <d3d9.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include "forward.h"
#include "vertex.h"
#include "vertex_buffer.h"
#include "vertex_traits.h"

namespace graphic
{

class D3D9VertexBuffer: public VertexBuffer
{
	friend class D3D9Renderer;

public:
	virtual ~D3D9VertexBuffer();

	virtual const void *lock() const;
	virtual void unlock() const;
	virtual void *lock(bool discard);
	virtual void unlock();

	size_t get_size() const { return clone_.size(); }

	void *begin() { return &clone_[0]; }
	void const *begin() const { return &clone_[0]; }
	void *end() { return &clone_[0] + get_size(); }
	void const *end() const { return &clone_[0] + get_size(); }

	IDirect3DVertexBuffer9 *get() const { return buffer_.get(); }

	VertexTraits const *get_vertex_traits() const { return traits_; }
	IDirect3DVertexDeclaration9 *get_vertex_decl() const { return decl_; }

	void handle_lost_device();
	void handle_reset_device();

protected:
	D3D9RendererPtr renderer_;
	boost::intrusive_ptr<IDirect3DVertexBuffer9> buffer_;
	std::vector<char> clone_;
	DWORD usage_;
	D3DPOOL pool_;
	VertexTraits const *traits_;
	IDirect3DVertexDeclaration9 *decl_;

	D3D9VertexBuffer(D3D9RendererPtr const &renderer, size_t size, bool dynamic, bool writeonly,
		VertexTraits const *traits, IDirect3DVertexDeclaration9 *decl);
};

}
