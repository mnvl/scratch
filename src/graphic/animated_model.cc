
#include <luabind/luabind.hpp>
#include "animated_mesh.h"
#include "animated_model.h"

namespace graphic
{

template<class V>
BasicAnimatedModel<V>::BasicAnimatedModel(RendererPtr const &renderer_ptr, SchedulerPtr const &scheduler_ptr):
	Model(renderer_ptr, scheduler_ptr)
{
}

template<class V>
BasicAnimatedModel<V>::~BasicAnimatedModel()
{
}

template<class V>
MeshPtr BasicAnimatedModel<V>::add_mesh()
{
	meshes.push_back(MeshPtr(new BasicAnimatedMesh<V>(this)));
	return meshes.back();
}

template<class V>
void BasicAnimatedModel<V>::bind(lua_State *L, char const *classname)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<BasicAnimatedModel, Model, boost::shared_ptr<BasicAnimatedModel> >(classname)
		.def(constructor<BasicAnimatedModel const &>())
        .def(constructor<RendererPtr const &, SchedulerPtr const &>())
	];
}

void bind_animated_model(lua_State *L)
{
	AnimatedModelPNJWT::bind(L, "AnimatedModelPNJWT");
	AnimatedModelPTBNJWT::bind(L, "AnimatedModelPTBNJWT");
}

template class BasicAnimatedModel<VERTEX_PNJWT>;
template class BasicAnimatedModel<VERTEX_PTBNJWT>;

}
