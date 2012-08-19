
#include <d3dx9.h>
#include <stdexcept>
#include <luabind/luabind.hpp>
#include "camera.h"

namespace graphic
{

Camera::Camera()
{
	set_perspective_projection(math::PI/4, 1.0f, 1.0f, 10000.0f);
	set_eye_lookat_up(math::vec<3>(0, 0, 0), math::vec<3>(0, 0, 1), math::vec<3>(0, 1, 0));
}

void Camera::set_perspective_projection(math::scalar fov, math::scalar aspect, math::scalar z_near, math::scalar z_far)
{
	field_of_view_ = fov;
	aspect_ratio_ = aspect;
	near_plane_ = z_near;
	far_plane_ = z_far;

	projection_matrix_.perspective(fov, aspect, z_near, z_far);
	projection_matrix_.inverse(inverted_projection_matrix_);
}

void Camera::set_orthographic_projection(math::scalar width, math::scalar height, math::scalar z_near, math::scalar z_far)
{
	field_of_view_ = math::PI/4;
	aspect_ratio_ = width / height;
	near_plane_ = z_near;
	far_plane_ = z_far;

	projection_matrix_.ortho(width, height, z_near, z_far);

	if (!projection_matrix_.inverse(inverted_projection_matrix_))
		throw std::runtime_error("can't invert projection matrix");
}

void Camera::set_view_matrix(math::matrix<4,4> const &tf)
{
	view_matrix_ = tf;
	tf.inverse(inverted_view_matrix_);
	billboard_matrix_ = inverted_view_matrix_;

	{
		math::matrix<4,4> &bb = billboard_matrix_;

		cross_dir_.set(bb.ij[0][0], bb.ij[0][1], bb.ij[0][2]);
		up_dir_.set(bb.ij[1][0], bb.ij[1][1], bb.ij[1][2]);
		view_dir_.set(bb.ij[2][0], bb.ij[2][1], bb.ij[2][2]);
		eye_point_.set(bb.ij[3][0], bb.ij[3][1], bb.ij[3][2]);

		bb.ij[3][0] = 0;
		bb.ij[3][1] = 0;
		bb.ij[3][2] = 0;
	}

	// view matrices with scaling are not supported
	assert (math::abs(cross_dir_.length_sq() - 1.0f) < math::EPSILON);
	assert (math::abs(up_dir_.length_sq() - 1.0f) < math::EPSILON);
	assert (math::abs(view_dir_.length_sq() - 1.0f) < math::EPSILON);
}

void Camera::set_eye_lookat_up(math::vec<3> const &eye_pt, math::vec<3> const &lookat_pt, math::vec<3> const &up_dir)
{
	math::matrix<4,4> tf;
	tf.lookat(eye_pt, lookat_pt, up_dir);
	set_view_matrix(tf);
}

void Camera::set_translation_rotation(math::vec<3> const &tl, math::vec<3> const &rot)
{
	math::matrix<4, 4> tf;
	tf.translation(tl);
	tf.rotate(rot);
	set_view_matrix(tf);
}

void Camera::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "graphic")
	[
		class_<Camera, CameraPtr >("Camera")
		.def(constructor<>())
		.def("set_view_matrix", &Camera::set_view_matrix)
		.def("set_perspective_projection", &Camera::set_perspective_projection)
		.def("set_orthographic_projection", &Camera::set_orthographic_projection)
		.def("set_eye_lookat_up", &Camera::set_eye_lookat_up)
		.def("set_translation_rotation", &Camera::set_translation_rotation)
		.def("get_eye_point", &Camera::get_eye_point)
		.def("get_up_dir", &Camera::get_up_dir)
		.def("get_view_dir", &Camera::get_view_dir)
		.def("get_cross_dir", &Camera::get_cross_dir)
		.def("get_view_matrix", &Camera::get_view_matrix)
		.def("get_inverted_view_matrix", &Camera::get_inverted_view_matrix)
		.def("get_billboard_matrix", &Camera::get_billboard_matrix)
		.def("get_projection_matrix", &Camera::get_projection_matrix)
		.def("get_inverted_projection_matrix", &Camera::get_inverted_projection_matrix)
	];
}

}
