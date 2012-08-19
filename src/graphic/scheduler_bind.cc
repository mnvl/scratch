
#include <luabind/luabind.hpp>
#include "scheduler.h"

namespace graphic
{

void Scheduler::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "graphic")
	[
		class_<Scheduler, SchedulerPtr >("Scheduler")
		.def(constructor<RendererPtr const &>())
		.def("add", &Scheduler::add)
		.def("flush", &Scheduler::flush)
		.def("get_visible_calls_count", &Scheduler::get_visible_calls_count)
		.def("get_invisible_calls_count", &Scheduler::get_invisible_calls_count)
	];
}

}
