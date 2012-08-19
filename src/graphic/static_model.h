#pragma once

#include <boost/shared_ptr.hpp>
#include <luabind/lua_include.hpp>
#include <graphic/model.h>
#include <graphic/memory_model.h>
#include <graphic/static_mesh.h>
#include <graphic/renderer.h>

namespace graphic {

template<class V = VERTEX_PNT>
class BasicStaticModel: public Model {
public:
	BasicStaticModel(RendererPtr const &renderer_ptr, SchedulerPtr const &scheduler_ptr);
	~BasicStaticModel();

	MeshPtr add_mesh();

	static inline void bind(lua_State *L, char const *classname);
};

typedef BasicStaticModel<VERTEX_PNT> StaticModelPNT;
typedef BasicStaticModel<VERTEX_PTBNT> StaticModelPTBNT;

void bind_static_model(lua_State *L);

}

