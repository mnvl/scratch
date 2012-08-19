#pragma once

#include "uber_shader.h"
#include "draw_call.h"

namespace graphic
{

class UberDrawCall: public DrawCall {
public:
	UberDrawCall(RendererPtr const &renderer);
	virtual ~UberDrawCall();

	void swap_joint_matrix_array(std::vector<math::matrix<4,4> > &joint_matrix_array);
	void set_joint_matrix_array(std::vector<math::matrix<4,4> > &joint_matrix_array);

	static void bind(lua_State *L);

protected:
	UberShaderPtr const shader_;
	std::vector<math::matrix<4,4> > joint_matrix_array_;
};

}
