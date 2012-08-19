#pragma once

#include <d3d9.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include "index_buffer.h"

namespace graphic
{

class D3D9IndexBuffer: public IndexBuffer
{
	friend class D3D9Renderer;

public:
	virtual ~D3D9IndexBuffer();

	const void *lock() const;
	void unlock() const;
	void *lock(bool discard);
	void unlock();

	void *begin() { return &clone_[0]; }
	void const *begin() const { return &clone_[0]; }

	IDirect3DIndexBuffer9 *get() const { return buffer_.get(); }

	void handle_lost_device();
	void handle_reset_device();

protected:
	D3D9RendererPtr renderer_;

	D3D9IndexBuffer(D3D9RendererPtr const &renderer, D3DFORMAT format, size_t size,
		bool dynamic = false, bool writeonly = false);

private:
	D3DFORMAT format_;
	boost::intrusive_ptr<IDirect3DIndexBuffer9> buffer_;
	std::vector<char> clone_;
	DWORD usage_;
	D3DPOOL pool_;
};

}
