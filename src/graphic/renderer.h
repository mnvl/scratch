#pragma once

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <luabind/lua_include.hpp>
#include <math/matrix.h>
#include "forward.h"
#include "camera.h"
#include "vertex_traits.h"
#include "vertex_array.h"
#include "index_array.h"

namespace graphic
{

class Renderer: public boost::enable_shared_from_this<Renderer>, public boost::noncopyable {
public:
	Renderer();
	virtual ~Renderer();

	virtual void postconstruct() = 0;

	virtual int get_screen_width() const = 0;
	virtual int get_screen_height() const = 0;
	virtual void resize(int width, int height) = 0;
	virtual void *get_window_handle() const = 0;

	virtual void begin_frame() = 0;
	virtual void finish_frame() = 0;

	virtual VertexBufferPtr create_vertex_buffer(VertexTraits const *vertex_traits,
		size_t size, bool dynamic = false, bool writeonly = false) = 0;
	virtual IndexBufferPtr create_index_buffer(unsigned index_format, size_t size,
		bool dynamic = false, bool writeonly = false) = 0;

	template <class V> boost::shared_ptr< VertexArray<V> >
	create_vertex_array(size_t count, bool dynamic = false, bool writeonly = false)
	{
		return boost::shared_ptr< VertexArray<V> >(
			new VertexArray<V>(
				create_vertex_buffer(VertexTraits::get<V>(), sizeof(V) * count, dynamic, writeonly)));
	}

	template <class I> boost::shared_ptr<IndexArray<I> >
	create_index_array(size_t count, bool dynamic = false, bool writeonly = false)
	{
		return boost::shared_ptr< IndexArray<I> >(
			new IndexArray<I>(
				create_index_buffer(INDEX_FORMAT_U16, sizeof(I) * count, dynamic, writeonly)));
	}

	virtual TexturePtr create_texture(int width, int height, unsigned pixel_format,
		bool dynamic = false, bool writeonly = false) = 0;
	virtual MaterialPtr create_material() = 0;
	virtual ShaderPtr create_shader() = 0;

	virtual UberShaderPtr get_uber_shader() const = 0;

	virtual DrawCallPtr create_draw_call() = 0;
	virtual UberDrawCallPtr create_uber_draw_call() = 0;

    static void bind(lua_State *L);
};

}
