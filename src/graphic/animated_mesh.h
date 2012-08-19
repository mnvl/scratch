#pragma once

#include "static_mesh.h"

namespace graphic
{

class MeshShader;
template<class V> class BasicAnimatedModel;

template<class V = VERTEX_PNJWT>
class BasicAnimatedMesh: public BasicStaticMesh<V> {
public:
    BasicAnimatedMesh(Model *model_ptr);
	virtual ~BasicAnimatedMesh();

	virtual MeshPtr shallow_copy() const;

    BasicAnimatedModel<V> *get_model();

	virtual void check();

    virtual void draw(Joint *joint_ptr, CameraPtr const &camera, math::matrix<4,4> const &world_matrix);
};

typedef BasicAnimatedMesh<VERTEX_PNJWT> AnimatedMeshPNJWT;
typedef BasicAnimatedMesh<VERTEX_PTBNJWT> AnimatedMeshPTBNJWT;

}
