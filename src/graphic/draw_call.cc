
#include <luabind/luabind.hpp>
#include <math/frustum.h>
#include "directx_error.h"
#include "vertex_traits.h"
#include "camera.h"
#include "uber_shader.h"
#include "texture.h"
#include "material.h"
#include "renderer.h"
#include "draw_call.h"

namespace graphic
{

DrawCall::DrawCall(RendererPtr const &renderer):
	renderer_(renderer),
	priority_(0),
	always_visible_(false),
	minimal_vertex_index_(0),
	vertices_count_(0),
	first_index_offset_(0),
	triangles_count_(0)
{
	world_matrix_.identity();
}

DrawCall::~DrawCall()
{
}

void DrawCall::set_priority(int priority)
{
	priority_ = priority_;
}

void DrawCall::set_always_visible(bool always_visible)
{
	always_visible_ = always_visible;
}

void DrawCall::set_obb(math::obb<3> const &bounds)
{
	bounds_ = bounds;
}

void DrawCall::set_vertex_buffer(VertexBufferPtr const &vertex_buffer)
{
	vertex_buffer_ = vertex_buffer;
}

void DrawCall::set_index_buffer(IndexBufferPtr const &index_buffer)
{
	index_buffer_ = index_buffer;
}

void DrawCall::set_material(MaterialPtr const &material)
{
	material_ = material;
}

void DrawCall::set_minimal_vertex_index(size_t minimal_vertex_index)
{
	minimal_vertex_index_ = minimal_vertex_index;
}

void DrawCall::set_vertices_count(size_t vertices_count)
{
	vertices_count_ = vertices_count;
}

void DrawCall::set_first_index_offset(size_t first_index_offset)
{
	first_index_offset_ = first_index_offset;
}

void DrawCall::set_triangles_count(size_t triangles_count)
{
	triangles_count_ = triangles_count;
}

void DrawCall::set_world_matrix(math::matrix<4,4> const &world_matrix)
{
	world_matrix_ = world_matrix;
}

void DrawCall::set_camera(CameraPtr const &camera)
{
	camera_ = camera;
}

void DrawCall::commit()
{
	if (!vertex_buffer_) throw std::logic_error("vertex buffer not set");
	if (!material_) throw std::logic_error("material not set");
	if (!camera_) throw std::logic_error("camera not set");

	view_projection_matrix_ = camera_->get_view_matrix() * camera_->get_projection_matrix();
	world_view_projection_matrix_ = world_matrix_ * view_projection_matrix_;

	frustum_.load(view_projection_matrix_);
}

bool DrawCall::operator <(DrawCall const &rhs) const
{
	if (priority_ < rhs.priority_) return true;
	if (priority_ > rhs.priority_) return false;

	return material_ < rhs.material_;
}

bool DrawCall::is_visible() const
{
	return always_visible_ || frustum_.test_intersection(bounds_);
}

void DrawCall::execute()
{
}

void DrawCall::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<DrawCall, DrawCallPtr >("DrawCall")
		.def(constructor<RendererPtr const &>())
		.def("set_priority", &DrawCall::set_priority)
		.def("set_always_visible", &DrawCall::set_always_visible)
		.def("set_obb", &DrawCall::set_obb)
		.def("set_vertex_buffer", &DrawCall::set_vertex_buffer)
		.def("set_index_buffer", &DrawCall::set_index_buffer)
		.def("set_material", &DrawCall::set_material)
		.def("set_minimal_vertex_index", &DrawCall::set_minimal_vertex_index)
		.def("set_vertices_count", &DrawCall::set_vertices_count)
		.def("set_first_index_offset", &DrawCall::set_first_index_offset)
		.def("set_triangles_count", &DrawCall::set_triangles_count)
		.def("set_world_matrix", &DrawCall::set_world_matrix)
		.def("set_camera", &DrawCall::set_camera)
		.def("commit", &DrawCall::commit)
	];
}

}
