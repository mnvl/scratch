#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>
#include <luabind/lua_include.hpp>
#include "joint.h"
#include "animation.h"
#include "mesh.h"
#include "material.h"

namespace graphic
{

class Camera;
class Renderer;
class Scheduler;

class Model {
public:
	typedef std::vector<JointPtr> joints_type;
	typedef std::vector<AnimationPtr> animations_type;
	typedef std::vector<MeshPtr> meshes_type;
	typedef std::vector<MaterialPtr> materials_type;

	joints_type joints;
	meshes_type meshes;
	animations_type animations;
	materials_type materials;

	Model(RendererPtr const &renderer_ptr, SchedulerPtr const &scheduler_ptr);
	Model(Model const &model); // makes shallow copy of meshes and materials, and full copy of joints and animations
	virtual ~Model();

	RendererPtr const &get_renderer() const { return renderer_; }
	SchedulerPtr const &get_scheduler() const { return scheduler_; }

	virtual math::aabb<3> get_bounds() const;

	JointPtr get_joint_by_id(std::string const &name);
	JointPtr get_joint_by_name(std::string const &name);
	MeshPtr get_mesh_by_id(std::string const &id);
	MeshPtr get_mesh_by_name(std::string const &name);
	MaterialPtr get_material_by_name(std::string const &name);

	virtual JointPtr add_joint();
	virtual MeshPtr add_mesh() = 0;

    void load_from_collada(std::string const &filename);

	// checks that model have been loaded correctly
	virtual void check();

	// animations blending could be easily implemented adding another function that will clear
	// all Joint::matrix variables to null matrix (all elements == zero) and specifying animation
	// and blend factor in this function
    virtual void set_animation_time(math::scalar t);

	virtual void set_always_visible(bool flag);

    virtual void draw(CameraPtr const &camera, math::matrix<4,4> const &world_matrix);

    static void bind(lua_State *L);

protected:
	RendererPtr const renderer_;
	SchedulerPtr const scheduler_;
};

}
