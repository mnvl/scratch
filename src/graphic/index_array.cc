
#include <luabind/luabind.hpp>
#include "directx_error.h"
#include "renderer.h"
#include "index_array.h"

namespace graphic
{

void bind_index_array(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<INDEX_U16>("INDEX_U16"),

		class_<IndexArray16, boost::shared_ptr<IndexArray16> >("IndexArray16")
		.def("lock", (INDEX_U16 *(IndexArray16::*)(bool)) &IndexArray16::lock)
		.def("unlock", (void (IndexArray16::*)()) &IndexArray16::unlock)
		.def("put", &IndexArray16::put)
		.def("get", &IndexArray16::get)
		.def("get_index_buffer", &IndexArray16::get_index_buffer)
	 ];
}

}
