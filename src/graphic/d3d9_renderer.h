#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <boost/scoped_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/signals2.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include "d3d9_vertex_declarations.h"
#include "renderer.h"

namespace graphic
{

class D3D9Renderer: public Renderer {
public:
	D3D9RendererPtr shared_from_this()
	{
		return boost::static_pointer_cast<D3D9Renderer>(Renderer::shared_from_this());
	}

	D3D9Renderer(int width = 0, int height = 0, bool fullscreen = false, HWND window_handle = 0);
	virtual ~D3D9Renderer();

	void init();

	virtual int get_screen_width() const;
	virtual int get_screen_height() const;
	virtual void resize(int width, int height);
	virtual void *get_window_handle() const;

	virtual void postconstruct();

	void reset_state();
	void reset_device();

	virtual void begin_frame();
	virtual void finish_frame();

	IDirect3DDevice9 *get_device() const { return device_.get(); }
	D3D9VertexDeclarations *get_vertex_declarations() const { return vertex_declarations_.get(); }

	virtual VertexBufferPtr create_vertex_buffer(VertexTraits const *vertex_traits,
		size_t size, bool dynamic = false, bool writeonly = false);
	virtual IndexBufferPtr create_index_buffer(unsigned index_format, size_t size,
		bool dynamic = false, bool writeonly = false);

	virtual TexturePtr create_texture(int width, int height, unsigned pixel_format,
		bool dynamic = false, bool writeonly = false);
	virtual MaterialPtr create_material();
	virtual ShaderPtr create_shader();

	virtual DrawCallPtr create_draw_call();
	virtual UberDrawCallPtr create_uber_draw_call();

	virtual UberShaderPtr get_uber_shader() const;

	template<class T>
	void register_resource(boost::shared_ptr<T> const &resource)
	{
		signal_lost_device_.connect
			(boost::signals2::signal0<void>::slot_type(&T::handle_lost_device, resource.get()).track(resource));

		signal_reset_device_.connect
			(boost::signals2::signal0<void>::slot_type(&T::handle_reset_device, resource.get()).track(resource));
	}

private:
	HWND window_handle_;

	int screen_width_, screen_height_;
	bool fullscreen_;
	D3DPRESENT_PARAMETERS pp_;

	boost::intrusive_ptr<IDirect3D9> direct3d_;
	boost::intrusive_ptr<IDirect3DDevice9> device_;

	boost::signals2::signal0<void> signal_lost_device_;
	boost::signals2::signal0<void> signal_reset_device_;

	UberShaderPtr uber_shader_;
	boost::scoped_ptr<D3D9VertexDeclarations> vertex_declarations_;

	static HWND get_sdl_window_handle();
	void handle_resize();
};

}

