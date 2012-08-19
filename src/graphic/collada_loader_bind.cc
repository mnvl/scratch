
#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "collada_loader.h"

namespace graphic
{

void ColladaLoader::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<ColladaLoader, boost::shared_ptr<ColladaLoader> >("ColladaLoader")
        .def(constructor<Model *>())
		.def("load", &ColladaLoader::load)
		.def("build_model", &ColladaLoader::build_model)
    ];
}

}
