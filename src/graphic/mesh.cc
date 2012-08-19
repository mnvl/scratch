
#include <util/logger.h>
#include "model.h"
#include "uber_shader.h"
#include "renderer.h"
#include "mesh.h"

namespace graphic {

Mesh::Mesh(Model *model_ptr):
	model_(model_ptr),
	shader_(model_ptr->get_renderer() ? model_ptr->get_renderer()->get_uber_shader() : UberShaderPtr())
{
	bind_shape_matrix.zero();
}

Mesh::~Mesh()
{
}

void Mesh::set_model(Model *model)
{
	model_ = model;
}

Model *Mesh::get_model() const
{
	return model_;
}

void Mesh::check()
{
	LOG_INFO("checking mesh id = " << id << ", name = " << name << "...");
}

void Mesh::draw(Joint *joint_ptr, CameraPtr const &camera, math::matrix<4,4> const &world_matrix)
{
}

}


