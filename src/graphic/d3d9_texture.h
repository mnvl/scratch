#pragma once

#include <luabind/lua_include.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include "directx_error.h"
#include "texture.h"

namespace graphic
{

class D3D9Texture: public Texture {
	friend class D3D9Renderer;

public:
	D3D9Texture(D3D9RendererPtr const &renderer_ptr, int width, int height,
		unsigned pixel_format, bool dynamic = false, bool writeonly = false);
	virtual ~D3D9Texture();

	IDirect3DTexture9 *get() const
	{
		return texture_.get();
	}

	virtual const void *lock() const;
	virtual void unlock() const;
	virtual void *lock(bool discard);
	virtual void unlock();

	void handle_lost_device();
	void handle_reset_device();

private:
	D3D9RendererPtr renderer_;
	DWORD usage_;
	D3DPOOL pool_;
	boost::intrusive_ptr<IDirect3DTexture9> texture_;
	std::vector<char> clone_;

	void create_texture();
};

}
