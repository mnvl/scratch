
#include <luabind/luabind.hpp>
#include "memory_model.h"

void graphic::MemoryModel::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
		class_<MemoryModel, Model, boost::shared_ptr<MemoryModel> >("MemoryModel")
        .def(constructor<RendererPtr const &, SchedulerPtr const &>())
    ];
}
