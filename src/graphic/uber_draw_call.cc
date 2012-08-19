
#include <luabind/luabind.hpp>
#include "renderer.h"
#include "uber_draw_call.h"

namespace graphic
{

UberDrawCall::UberDrawCall(RendererPtr const &renderer):
	DrawCall(renderer),
	shader_(renderer->get_uber_shader())
{
}

UberDrawCall::~UberDrawCall()
{
}

void UberDrawCall::swap_joint_matrix_array(std::vector<math::matrix<4,4> > &joint_matrix_array)
{
	joint_matrix_array_.swap(joint_matrix_array);
}

void UberDrawCall::set_joint_matrix_array(std::vector<math::matrix<4,4> > &joint_matrix_array)
{
	joint_matrix_array_ = joint_matrix_array;
}

namespace
{

DrawCallPtr downcast_to_draw_call(UberDrawCallPtr const &uber_draw_call)
{
	return boost::static_pointer_cast<DrawCall>(uber_draw_call);
}

}

void UberDrawCall::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		def("downcast_to_draw_call", &downcast_to_draw_call),

		class_<UberDrawCall, DrawCall, UberDrawCallPtr>("UberDrawCall")
	];
}
}
