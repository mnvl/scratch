#pragma once

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <math/obb.h>
#include <math/frustum.h>
#include "forward.h"

namespace graphic
{

class DrawCall: public boost::noncopyable {
public:
	DrawCall(RendererPtr const &renderer);
	virtual ~DrawCall();

	void set_priority(int priority);
	void set_always_visible(bool always_visible);
	void set_obb(math::obb<3> const &bounds);
	void set_vertex_buffer(VertexBufferPtr const &vertex_buffer);
	void set_index_buffer(IndexBufferPtr const &index_buffer);
	void set_material(boost::shared_ptr<Material> const &material);
	void set_minimal_vertex_index(size_t minimal_vertex_index);
	void set_vertices_count(size_t vertices_count);
	void set_first_index_offset(size_t first_index_offset);
	void set_triangles_count(size_t triangles_count);
	void set_world_matrix(math::matrix<4,4> const &world_matrix);
	void set_camera(CameraPtr const &camera);
	void commit();

	bool operator <(DrawCall const &rhs) const;

	boost::shared_ptr<Material> get_material() const { return material_; }

	bool is_visible() const;

	virtual void execute();

	static void bind(lua_State *L);

protected:
	RendererPtr const renderer_;
	int priority_;
	bool always_visible_;
	math::obb<3> bounds_;
	VertexBufferPtr vertex_buffer_;
	IndexBufferPtr index_buffer_;
	boost::shared_ptr<Material> material_;
	size_t minimal_vertex_index_, vertices_count_, first_index_offset_, triangles_count_;
	math::matrix<4,4> world_matrix_;
	CameraPtr camera_;
	math::matrix<4,4> view_projection_matrix_, world_view_projection_matrix_;
	math::frustum<> frustum_;
};

}
