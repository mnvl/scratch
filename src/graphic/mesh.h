#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <math/aabb.h>
#include <math/matrix.h>
#include "material.h"
#include "generic_vertex.h"

namespace graphic {

class Mesh {
public:
	struct Triangles
	{
		MaterialPtr material_ptr;
		size_t minimal_vertex_index;
		size_t vertices_count;
		size_t first_index_offset;
		size_t triangles_count;
	};

	typedef std::vector<Triangles> triangles_list_type;
	typedef std::vector<math::matrix<4,4> > inverted_bind_matrices_type;
    typedef std::vector<size_t> joint_index_transform_type;

	std::string id, name;
	triangles_list_type triangles_list;
	math::matrix<4,4> bind_shape_matrix;
	inverted_bind_matrices_type inverted_bind_matrices;

    // for all joints influenting on vertices of this mesh this vector
    // contains index of joint in model's joints array
    joint_index_transform_type joint_index_transform;

	Mesh(Model *model_ptr = 0);
	virtual ~Mesh();

	virtual math::aabb<3> const &get_bounds() const = 0;

	virtual MeshPtr shallow_copy() const = 0;

	void set_model(Model *model);
	Model *get_model() const;

	virtual void set_vertices(GenericVertex *vertices, GenericVertex::format_type format, size_t count) = 0;
	virtual void set_indices(unsigned *indices, size_t count) = 0;
	virtual void set_always_visible(bool flag) = 0;

	virtual void check();

	virtual void draw(Joint *joint_ptr, CameraPtr const &camera, math::matrix<4,4> const &world_matrix);

	static void bind(lua_State *L);

protected:
	Model *model_;
	UberShaderPtr shader_;
};

}
