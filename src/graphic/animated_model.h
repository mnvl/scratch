#pragma once

#include "model.h"

namespace graphic
{

class Mesh;
class Renderer;

template<class V = VERTEX_PNJWT>
class BasicAnimatedModel: public Model {
public:
	BasicAnimatedModel(RendererPtr const &renderer_ptr, SchedulerPtr const &scheduler_ptr);
	~BasicAnimatedModel();

	virtual MeshPtr add_mesh();

    static inline void bind(lua_State *L, char const *classname);
};

typedef BasicAnimatedModel<VERTEX_PNJWT> AnimatedModelPNJWT;
typedef BasicAnimatedModel<VERTEX_PTBNJWT> AnimatedModelPTBNJWT;

void bind_animated_model(lua_State *L);

}
