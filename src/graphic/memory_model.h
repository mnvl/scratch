#pragma once

#include <boost/shared_ptr.hpp>
#include <math/matrix.h>
#include "model.h"
#include "memory_mesh.h"

namespace graphic {

class MemoryModel: public Model {
public:
	MemoryModel(RendererPtr const &renderer_ptr, SchedulerPtr const &scheduler_ptr);
	~MemoryModel();

	MemoryMeshPtr get_mesh_by_id(std::string const &id) {
		return boost::static_pointer_cast<MemoryMesh>(Model::get_mesh_by_id(id));
	}

	MemoryMeshPtr get_mesh_by_name(std::string const &name) {
		return boost::static_pointer_cast<MemoryMesh>(Model::get_mesh_by_name(name));
	}

	virtual MeshPtr add_mesh();

	virtual void draw(CameraPtr const &camera, math::matrix<4,4> const &world_matrix);
	void draw_static(CameraPtr const &camera, math::matrix<4,4> const &world_matrix);
	void draw_animated(CameraPtr const &camera, math::matrix<4,4> const &world_matrix);

	static void bind(lua_State *L);
};

}
