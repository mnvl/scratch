
#include <boost/filesystem.hpp>
#include <luabind/luabind.hpp>
#include <util/logger.h>
#include "d3d9_renderer.h"
#include "d3d9_texture.h"

namespace graphic
{

D3D9Texture::D3D9Texture(D3D9RendererPtr const &renderer_ptr, int width, int height,
	unsigned pixel_format, bool dynamic, bool writeonly):
	Texture(width, height, pixel_format, dynamic, writeonly),
	renderer_(renderer_ptr),
	usage_(0),
	pool_(dynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED)
{
	if(dynamic) usage_ |= D3DUSAGE_DYNAMIC;
	if(writeonly) usage_ |= D3DUSAGE_WRITEONLY;

	create_texture();
}

D3D9Texture::~D3D9Texture()
{
}

void D3D9Texture::create_texture() {
	PixelFormat const &pixel_format = PixelFormat::get(pixel_format_);

	IDirect3DTexture9 *texture_ptr;
	HRESULT hr = renderer_->get_device()->CreateTexture(width_, height_, 0, usage_,
		pixel_format.direct3d_format, pool_, &texture_ptr, 0);
	if (FAILED(hr)) throw directx_error(hr);
	texture_.reset(texture_ptr);
	texture_ptr->Release();

	clone_.resize(width_ * height_ * pixel_format.bits / 8);
}

const void *D3D9Texture::lock() const {
	return &clone_[0];
}

void D3D9Texture::unlock() const {
}

void *D3D9Texture::lock(bool discard) {
	return &clone_[0];
}

void D3D9Texture::unlock() {
	PixelFormat const &pixel_format = PixelFormat::get(pixel_format_);

	D3DLOCKED_RECT locked_rect;
	HRESULT hr = texture_->LockRect(0, &locked_rect, 0, D3DLOCK_DISCARD);
	if (FAILED(hr)) throw directx_error(hr);

	size_t source_pitch = width_ * pixel_format.bits / 8;

	char *get = &clone_[0];
	char *put = (char *) locked_rect.pBits;

	for (int y = 0; y < height_; ++y) {
		memcpy(put, get, source_pitch);

		get += source_pitch;
		put += locked_rect.Pitch;
	}

	texture_->UnlockRect(0);
}

void D3D9Texture::handle_lost_device()
{
	texture_.reset();
}

void D3D9Texture::handle_reset_device()
{
	create_texture();
	unlock();
}

}
