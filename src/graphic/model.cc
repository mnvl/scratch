
#include <boost/unordered_map.hpp>
#include <luabind/luabind.hpp>
#include "collada_loader.h"
#include "model.h"

namespace graphic {

Model::Model(RendererPtr const &renderer_ptr, SchedulerPtr const &scheduler_ptr):
	renderer_(renderer_ptr),
	scheduler_(scheduler_ptr)
{
}

Model::Model(Model const &model):
	materials(model.materials),
	renderer_(model.renderer_),
	scheduler_(model.scheduler_)
{
	boost::unordered_map<JointPtr, JointPtr> joints_tt;
	boost::unordered_map<MeshPtr, MeshPtr> meshes_tt;

	joints.reserve(model.joints.size());
	animations.reserve(model.animations.size());
	meshes.reserve(model.meshes.size());

	for (size_t i = 0; i < model.joints.size(); i++)
	{
		JointPtr source = model.joints[i];
		JointPtr joint(new Joint(*source));

		joints.push_back(joint);
		joints_tt.insert(std::make_pair(source, joint));
	}

	for (animations_type::const_iterator it = model.animations.begin(); it != model.animations.end(); it++)
	{
		animations.push_back(AnimationPtr(new Animation(**it)));
	}

	for (meshes_type::const_iterator it = model.meshes.begin(); it != model.meshes.end(); it++)
	{
		MeshPtr source = *it;

		MeshPtr mesh = source->shallow_copy();
		mesh->set_model(this);

		meshes.push_back(mesh);

		meshes_tt.insert(std::make_pair(source, mesh));
	}

	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (JointPtr parent = (*it)->parent.lock())
		{
			(*it)->parent = joints_tt.find(parent)->second;
		}

		for (Joint::childs_type::iterator jt = (*it)->childs.begin(); jt != (*it)->childs.end(); jt++)
		{
			*jt = joints_tt.find(jt->lock())->second;
		}

		if (MeshPtr mesh_ptr = (*it)->mesh_ptr.lock())
		{
			(*it)->mesh_ptr = meshes_tt.find(mesh_ptr)->second;
		}
	}

	for (animations_type::iterator it = animations.begin(); it != animations.end(); it++)
	{
		for (Animation::samplers_type::iterator jt = (*it)->samplers.begin(); jt != (*it)->samplers.end(); jt++)
		{
			if (JointPtr joint = jt->get_joint())
			{
				jt->set_joint(joints_tt.find(joint)->second);
			}
		}
	}
}

Model::~Model()
{
}

math::aabb<3> Model::get_bounds() const
{
	math::aabb<3> bounds;
	bounds.null();

	for (meshes_type::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		bounds.extend((*it)->get_bounds());
	}

	return bounds;
}

JointPtr Model::add_joint()
{
	joints.push_back(JointPtr(new Joint(joints.size())));
	return joints.back();
}

void Model::load_from_collada(std::string const &filename)
{
    ColladaLoader loader(this);
	loader.load(filename);
	loader.build_model();
}

void Model::check()
{
	for (meshes_type::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		(*it)->check();
	}
}

JointPtr Model::get_joint_by_id(std::string const &id)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if ((*it)->id == id) return *it;
	}

	throw std::runtime_error("joint with id = " + id + " not found in model");
}

JointPtr Model::get_joint_by_name(std::string const &name)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if ((*it)->name == name) return *it;
	}

	throw std::runtime_error("joint with name = " + name + " not found");
}

MeshPtr Model::get_mesh_by_id(std::string const &id)
{
	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++) {
		if ((*it)->id == id) return *it;
	}

	throw std::runtime_error("mesh with id = " + id + " not found");
}

MeshPtr Model::get_mesh_by_name(std::string const &name)
{
	for (meshes_type::iterator it = meshes.begin(); it != meshes.end(); it++) {
		if ((*it)->name == name) return *it;
	}

	throw std::runtime_error("mesh with name = " + name + " not found");
}

MaterialPtr Model::get_material_by_name(std::string const &name)
{
	for (materials_type::iterator it = materials.begin(); it != materials.end(); it++) {
		if ((*it)->get_name() == name) return *it;
	}

	throw std::runtime_error("material with name = " + name + " not found");
}

void Model::set_animation_time(math::scalar t)
{
	for (animations_type::iterator it = animations.begin(); it != animations.end(); it++)
	{
		(*it)->set_time(t);
	}

	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++) {
		if (!(*it)->parent.lock()) {
			(*it)->build_frame_transforms();
		}
	}
}

void Model::set_always_visible(bool flag)
{
	for (meshes_type::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		(*it)->set_always_visible(flag);
	}
}

void Model::draw(CameraPtr const &camera, math::matrix<4,4> const &world_matrix)
{
	for (joints_type::iterator it = joints.begin(); it != joints.end(); it++)
	{
		if (MeshPtr mesh_ptr = (*it)->mesh_ptr.lock())
		{
			mesh_ptr->draw(it->get(), camera, world_matrix);
		}
	}
}

void Model::bind(lua_State *L)
{
    using namespace luabind;

    module_(L, "graphic")
    [
        class_<Model, boost::shared_ptr<Model> >("Model")
		.def("get_bounds", &Model::get_bounds)
		.def("load_from_collada", &Model::load_from_collada)
		.def("get_joint_by_id", &Model::get_joint_by_id)
		.def("get_joint_by_name", &Model::get_joint_by_name)
		.def("set_animation_time", &Model::set_animation_time)
		.def("set_always_visible", &Model::set_always_visible)
		.def("draw", &Model::draw)
		.scope
		[
			class_<materials_type>("materials_type")
			.def("size", &materials_type::size)
			.def("at", (MaterialPtr &(materials_type::*)(size_t)) &materials_type::at)
		]
		.def_readwrite("materials", &Model::materials)
    ];
}

}
