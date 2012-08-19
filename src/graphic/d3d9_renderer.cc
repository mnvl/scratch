
#include <SDL.h>
#include <SDL_syswm.h>
#include "directx_error.h"
#include <util/logger.h>
#include <util/get_sources_path.h>
#include "uber_shader.h"
#include "d3d9_vertex_buffer.h"
#include "d3d9_index_buffer.h"
#include "d3d9_texture.h"
#include "d3d9_material.h"
#include "d3d9_shader.h"
#include "d3d9_draw_call.h"
#include "d3d9_uber_draw_call.h"
#include "d3d9_renderer.h"

namespace graphic
{

D3D9Renderer::D3D9Renderer(int width, int height, bool fullscreen, HWND window_handle):
	Renderer(),
	window_handle_(window_handle ? window_handle : get_sdl_window_handle()),
	screen_width_(width),
	screen_height_(height),
	fullscreen_(fullscreen)
{
	init();
}

D3D9Renderer::~D3D9Renderer()
{
}


void D3D9Renderer::init() {
	HRESULT hr;

	if (!screen_width_ || !screen_height_)
	{
		RECT rc;
		GetClientRect(window_handle_, &rc);

		screen_width_ = rc.right - rc.left;
		screen_height_ = rc.bottom - rc.top;

		LOG_INFO("no width or height specified, taking dimensions from window client dimensions "
			<< screen_width_ << 'x' << screen_height_);
	}

	{
		IDirect3D9 *direct3d_ptr = Direct3DCreate9(D3D_SDK_VERSION);
		direct3d_.reset(direct3d_ptr);
		direct3d_ptr->Release();
	}

	{
		UINT adapter = D3DADAPTER_DEFAULT;
		D3DDEVTYPE device_type = D3DDEVTYPE_HAL;

		LOG_INFO("looking for PerfHUD adapter...");

		for (UINT i = 0, n = direct3d_->GetAdapterCount(); i < n; ++i)
		{
			D3DADAPTER_IDENTIFIER9 id;
			hr = direct3d_->GetAdapterIdentifier(i, 0, &id);

			LOG_INFO("*** " << id.Description);

			if (strstr(id.Description,"PerfHUD") != 0)
			{
				LOG_INFO("PerfHUD adapter found!");

				adapter = i;
				device_type = D3DDEVTYPE_REF;

				break;
			}
		}

		IDirect3DDevice9 *device_ptr;

		pp_.BackBufferWidth = screen_width_;
		pp_.BackBufferHeight = screen_height_;
		pp_.BackBufferCount = 1;
		pp_.BackBufferFormat = fullscreen_ ? D3DFMT_A8R8G8B8 : D3DFMT_UNKNOWN;
		pp_.MultiSampleType = D3DMULTISAMPLE_NONE;
		pp_.MultiSampleQuality = 0;
		pp_.SwapEffect = D3DSWAPEFFECT_DISCARD;
		pp_.hDeviceWindow = window_handle_;
		pp_.Windowed = !fullscreen_;
		pp_.EnableAutoDepthStencil = TRUE;
		pp_.AutoDepthStencilFormat = D3DFMT_D24S8;
		pp_.Flags = 0;
		pp_.FullScreen_RefreshRateInHz = 0;
		pp_.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		hr = direct3d_->CreateDevice(adapter, device_type, window_handle_,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp_, &device_ptr);

		if (FAILED(hr))
		{
			LOG_ERROR("failed to create direct3d device with hr = " << std::hex << hr
				<< ", trying sw vertex processing");

			hr = direct3d_->CreateDevice(adapter, device_type, window_handle_,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp_, &device_ptr);
		}

		if (FAILED(hr)) throw directx_error(hr);

		device_.reset(device_ptr);
		device_ptr->Release();
	}
}

int D3D9Renderer::get_screen_width() const
{
	return screen_width_;
}

int D3D9Renderer::get_screen_height() const
{
	return screen_height_;
}

void D3D9Renderer::resize(int width, int height)
{
	if (fullscreen_) return;

	if (!width || !height) {
		RECT rc;
		GetClientRect(window_handle_, &rc);

		screen_width_ = rc.right - rc.left;
		screen_height_ = rc.bottom - rc.top;
	} else {
		screen_width_ = width;
		screen_height_ = height;
	}

	LOG_INFO("window resized, new client dimensions are " << screen_width_ << 'x' << screen_height_);

	pp_.BackBufferWidth = screen_width_;
	pp_.BackBufferHeight = screen_height_;

	reset_device();
}

void *D3D9Renderer::get_window_handle() const {
	return window_handle_;
}

void D3D9Renderer::postconstruct()
{
	uber_shader_.reset(new UberShader(shared_from_this()));

	vertex_declarations_.reset(new D3D9VertexDeclarations(this));

	reset_state();
}

void D3D9Renderer::reset_state()
{
	HRESULT hr;

	hr = device_->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (FAILED(hr)) throw directx_error(hr);

	hr = device_->SetRenderState(D3DRS_ALPHAREF, 1u);
	if (FAILED(hr)) throw directx_error(hr);

	hr = device_->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	if (FAILED(hr)) throw directx_error(hr);

	hr = device_->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	if (FAILED(hr)) throw directx_error(hr);

	hr = device_->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (FAILED(hr)) throw directx_error(hr);

	{
		D3DMATERIAL9 material;
		material.Ambient.r = material.Ambient.g = material.Ambient.b = material.Ambient.a = 1.0f;
		material.Diffuse = material.Ambient;
		material.Specular = material.Ambient;
		material.Emissive = material.Ambient;
		material.Power = 1.0f;

		hr = device_->SetMaterial(&material);
		if (FAILED(hr)) throw directx_error(hr);
	}
}

void D3D9Renderer::reset_device()
{
	LOG_INFO("preparing all resources to device reset...");
	signal_lost_device_();

	LOG_INFO("resetting device...");
	HRESULT hr = get_device()->Reset(&pp_);
	if (FAILED(hr)) throw directx_error(hr);

	LOG_INFO("restoring all resources...");
	signal_reset_device_();

	LOG_INFO("resetting render state...");
	reset_state();

	LOG_INFO("device reset completed successfully...");
}

void D3D9Renderer::begin_frame()
{
	HRESULT hr;

	hr = device_->TestCooperativeLevel();

	while (hr == D3DERR_DEVICELOST)
	{
		LOG_INFO("device lost");
		Sleep(1);
		hr = device_->TestCooperativeLevel();
	}

	while (hr == D3DERR_DEVICENOTRESET)
	{
		LOG_INFO("device not reset");
		reset_device();
		hr = device_->TestCooperativeLevel();
	}

	hr = device_->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (FAILED(hr)) throw directx_error(hr);

	hr = device_->BeginScene();
	if (FAILED(hr)) throw directx_error(hr);
}

void D3D9Renderer::finish_frame()
{
	HRESULT hr;

	hr = device_->EndScene();
	if (FAILED(hr)) throw directx_error(hr);

	hr = device_->Present(0, 0, 0, 0);
	if (FAILED(hr)) throw directx_error(hr);
}

VertexBufferPtr D3D9Renderer::create_vertex_buffer(VertexTraits const *traits, size_t size, bool dynamic, bool writeonly)
{
	boost::shared_ptr<D3D9VertexBuffer> vertex_buffer(new D3D9VertexBuffer(shared_from_this(), size, dynamic, writeonly,
		traits, vertex_declarations_->get(traits)));
	register_resource(vertex_buffer);
	return vertex_buffer;
}

IndexBufferPtr D3D9Renderer::create_index_buffer(unsigned format, size_t size, bool dynamic, bool writeonly)
{
	boost::shared_ptr<D3D9IndexBuffer> index_buffer(new D3D9IndexBuffer(shared_from_this(), (D3DFORMAT) format, size, dynamic, writeonly));
	register_resource(index_buffer);
	return index_buffer;
}

TexturePtr D3D9Renderer::create_texture(int width, int height, unsigned pixel_format,
		bool dynamic, bool writeonly)
{
	boost::shared_ptr<D3D9Texture> texture(new D3D9Texture(shared_from_this(), width, height,
		pixel_format, dynamic, writeonly));
	register_resource(texture);
	return texture;
}

MaterialPtr D3D9Renderer::create_material()
{
	return MaterialPtr(new D3D9Material(shared_from_this()));
}

ShaderPtr D3D9Renderer::create_shader()
{
	boost::shared_ptr<D3D9Shader> shader(new D3D9Shader(shared_from_this()));
	register_resource(shader);
	return shader;
}

DrawCallPtr D3D9Renderer::create_draw_call()
{
	return DrawCallPtr(new D3D9DrawCall(shared_from_this()));
}

UberDrawCallPtr D3D9Renderer::create_uber_draw_call()
{
	return UberDrawCallPtr(new D3D9UberDrawCall(shared_from_this()));
}

UberShaderPtr D3D9Renderer::get_uber_shader() const
{
	return uber_shader_;
}

HWND D3D9Renderer::get_sdl_window_handle() {
	SDL_SysWMinfo si;
	SDL_VERSION(&si.version);
	if (SDL_GetWMInfo(&si) <= 0) throw std::runtime_error(SDL_GetError());
	return (HWND) si.window;
}

}
