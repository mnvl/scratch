#pragma once

#include <luabind/lua_include.hpp>
#include <math/vec.h>
#include <math/matrix.h>
#include "forward.h"

namespace graphic
{

class Camera {
public:
	Camera();

    math::matrix<4,4> const &get_view_matrix() const {
		return view_matrix_;
	}

    math::matrix<4,4> const &get_inverted_view_matrix() const {
		return inverted_view_matrix_;
	}

	math::matrix<4,4> const &get_billboard_matrix() const {
		return billboard_matrix_;
	}

    math::matrix<4,4> const &get_projection_matrix() const {
		return projection_matrix_;
	}

    math::matrix<4,4> const &get_inverted_projection_matrix() const {
		return inverted_projection_matrix_;
	}

	math::vec<3> const &get_eye_point() const { return eye_point_; }
	math::vec<3> const &get_up_dir() const { return up_dir_; }
	math::vec<3> const &get_view_dir() const { return view_dir_; }
	math::vec<3> const &get_cross_dir() const { return cross_dir_; }

	void set_perspective_projection(math::scalar fov, math::scalar aspect,math::scalar z_near,
		math::scalar z_far);
	void set_orthographic_projection(math::scalar width, math::scalar height, math::scalar z_near,
		math::scalar z_far);

	void set_view_matrix(math::matrix<4,4> const &tf);
   	void set_eye_lookat_up(math::vec<3> const &eye_pt, math::vec<3> const &lookat_pt, math::vec<3> const &up_dir);
	void set_translation_rotation(math::vec<3> const &tl, math::vec<3> const &rot);

	static void bind(lua_State *L);

private:
	math::vec<3> eye_point_;
	math::vec<3> up_dir_;
	math::vec<3> view_dir_;
	math::vec<3> cross_dir_;
	math::matrix<4,4> view_matrix_;
	math::matrix<4,4> inverted_view_matrix_;
	math::matrix<4,4> billboard_matrix_;

	math::scalar field_of_view_;
	math::scalar aspect_ratio_;
	math::scalar near_plane_;
	math::scalar far_plane_;
	math::matrix<4,4> projection_matrix_;
	math::matrix<4,4> inverted_projection_matrix_;
};

}
