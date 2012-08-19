#pragma once

#include <d3d9.h>
#include <util/boost_com_intrusive_ptr.h>
#include <math/aabb.h>
#include "vertex.h"
#include "vertex_array.h"
#include "index.h"
#include "index_array.h"
#include "mesh.h"

namespace graphic
{

template<class V> class BasicStaticModel;

template<class V = VERTEX_PNT>
class BasicStaticMesh: public Mesh {
public:
	typedef V vertex_type;
	typedef INDEX_U16 index_type;

	BasicStaticMesh(Model *model_ptr);
	~BasicStaticMesh();

	virtual math::aabb<3> const &get_bounds() const;
	virtual MeshPtr shallow_copy() const;

	BasicStaticModel<V> *get_model();

	virtual void check();

	virtual void draw(Joint *joint_ptr, CameraPtr const &camera, math::matrix<4,4> const &world_matrix);

	virtual void set_vertices(GenericVertex *vertices, GenericVertex::format_type format, size_t count);
	virtual void set_indices(unsigned *indices, size_t count);
	virtual void set_always_visible(bool flag);

protected:
	typedef VertexArray<V> vertex_array_type;
	typedef IndexArray16 index_array_type;

	// AABB over positions of all vertices before applyed transform
	math::aabb<3> bounds_;

	GenericVertex::format_type initialized_vertex_components_;
	boost::shared_ptr<vertex_array_type> vertex_array_;
	boost::shared_ptr<index_array_type> index_array_;
	bool always_visible_;
};

typedef BasicStaticMesh<VERTEX_PNT> StaticMeshPNT;
typedef BasicStaticMesh<VERTEX_PTBNT> StaticMeshPTBNT;
typedef BasicStaticMesh<VERTEX_PNJWT> StaticMeshPNJWT;
typedef BasicStaticMesh<VERTEX_PTBNJWT> StaticMeshPTBNJWT;

}
