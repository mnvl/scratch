
#include "renderer.h"
#include "memory_model.h"

namespace graphic
{

MemoryModel::MemoryModel(RendererPtr const &renderer_ptr, SchedulerPtr const &scheduler_ptr):
	Model(renderer_ptr, scheduler_ptr)
{
}

MemoryModel::~MemoryModel()
{
}

MeshPtr MemoryModel::add_mesh()
{
	meshes.push_back(MeshPtr(new MemoryMesh(this)));
	return meshes.back();
}

void MemoryModel::draw(CameraPtr const &camera, math::matrix<4,4> const &world_matrix)
{
	if (animations.size()) draw_animated(camera, world_matrix);
	else draw_static(camera, world_matrix);
}

void MemoryModel::draw_static(CameraPtr const &camera, math::matrix<4,4> const &world_matrix)
{
	set_animation_time(0);

	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (MemoryMeshPtr mesh_ptr = boost::static_pointer_cast<MemoryMesh>((*it)->mesh_ptr.lock()))
		{
			mesh_ptr->draw_static(it->get(), camera, world_matrix);
		}
	}
}

void MemoryModel::draw_animated(CameraPtr const &camera, math::matrix<4,4> const &world_matrix)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (MemoryMeshPtr mesh_ptr = boost::static_pointer_cast<MemoryMesh>((*it)->mesh_ptr.lock()))
		{
			mesh_ptr->draw_animated(it->get(), camera, world_matrix);
		}
	}
}

}
