#pragma once

#include <boost/shared_ptr.hpp>
#include "vertex.h"
#include "index.h"
#include "mesh.h"
#include "model.h"

namespace graphic
{

class MemoryModel;
class Renderer;

typedef boost::shared_ptr<class MemoryMesh> MemoryMeshPtr;

class MemoryMesh: public Mesh {
public:
    typedef VERTEX_PTBNT vertex_type;
	typedef INDEX_U16 index_type;

	#pragma pack(push, 1)
	struct JointWeights
	{
		math::vec<8, unsigned> joints;
		math::vec<8> weights;
	};
	#pragma pack(pop)

	MemoryMesh(MemoryModel *m);
	virtual ~MemoryMesh();

	virtual math::aabb<3> const &get_bounds() const;
	virtual MeshPtr shallow_copy() const;

	std::vector<vertex_type> const &get_vertex_buffer() const { return vertices_; }
	std::vector<index_type> const &get_index_buffer() const { return indices_; }
	std::vector<JointWeights> const &get_vertex_weights() const { return weights_; }

	virtual void set_vertices(GenericVertex *vertices, GenericVertex::format_type format, size_t count);
	virtual void set_indices(unsigned *indices, size_t count);
	virtual void set_always_visible(bool flag);

	void transform_vertices(Joint const *joint_ptr, std::vector<vertex_type> &result) const;

	void draw_static(Joint *joint_ptr, CameraPtr const &camera, math::matrix<4,4> const &world_matrix) const;
	void draw_animated(Joint *joint_ptr, CameraPtr const &camera, math::matrix<4,4> const &world_matrix) const;

	static void bind(lua_State *L);

private:
	GenericVertex::format_type initialized_vertex_components_;
	std::vector<vertex_type> vertices_;
	std::vector<index_type> indices_;
	std::vector<JointWeights> weights_;
};

}
