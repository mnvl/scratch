
#include <d3d9.h>
#include <vector>
#include <util/logger.h>
#include "uber_shader.h"
#include "uber_draw_call.h"
#include "animated_model.h"
#include "renderer.h"
#include "scheduler.h"
#include "animated_mesh.h"

namespace graphic
{

template<class V>
BasicAnimatedMesh<V>::BasicAnimatedMesh(Model *model_ptr):
    BasicStaticMesh<V>(model_ptr)
{
}

template<class V>
BasicAnimatedMesh<V>::~BasicAnimatedMesh()
{
}

template<class V>
MeshPtr BasicAnimatedMesh<V>::shallow_copy() const
{
	return MeshPtr(new BasicAnimatedMesh(*this));
}

template<class V>
BasicAnimatedModel<V> *BasicAnimatedMesh<V>::get_model()
{
    return (BasicAnimatedModel<V> *) Mesh::get_model();
}

template<class V>
void BasicAnimatedMesh<V>::check()
{
   	if (this->vertex_array_->get_size())
	{
		if (math::abs(this->bind_shape_matrix.determinant()) < math::EPSILON)
		{
			LOG_WARN("bind shape matrix is zero");
		}

		static unsigned const MAXIMAL_MESSAGES_OF_SAME_KIND_TO_ISSUE = 10;

		unsigned vertex_weights_are_zero_issued = 0;
		unsigned vertex_joints_are_zero_issued = 0;

		for (typename BasicStaticMesh<V>::vertex_type const *vertex = this->vertex_array_->begin();
			 vertex != this->vertex_array_->end(); ++vertex)
		{
			if (vertex->joints.is_zero() && vertex_joints_are_zero_issued < MAXIMAL_MESSAGES_OF_SAME_KIND_TO_ISSUE)
			{
				LOG_WARN("vertex joints are zero for vertex with index = " << vertex - this->vertex_array_->begin());
				if (++vertex_joints_are_zero_issued == MAXIMAL_MESSAGES_OF_SAME_KIND_TO_ISSUE) {
					LOG_WARN("(following messages of same kind will be skipped...)");
				}
			}

			if (vertex->weights.is_zero() && vertex_weights_are_zero_issued < MAXIMAL_MESSAGES_OF_SAME_KIND_TO_ISSUE)
			{
				LOG_WARN("vertex weights are zero for vertex index = " << vertex - this->vertex_array_->begin());
				if (++vertex_weights_are_zero_issued == MAXIMAL_MESSAGES_OF_SAME_KIND_TO_ISSUE) {
					LOG_WARN("(following messages of same kind will be skipped...)");
				}
			}
		}
	}
}

template<class V>
void BasicAnimatedMesh<V>::draw(Joint *joint_ptr, CameraPtr const &camera,
	math::matrix<4,4> const &world_matrix)
{
	math::matrix<4,4> transform = world_matrix * joint_ptr->matrix;

    std::vector<math::matrix<4,4> > joint_matrix_array;
    joint_matrix_array.reserve(this->joint_index_transform.size());

    for (typename BasicStaticMesh<V>::joint_index_transform_type::const_iterator it = this->joint_index_transform.begin();
         it != this->joint_index_transform.end(); it++)
    {
		math::matrix<4,4> t = this->bind_shape_matrix *
			this->inverted_bind_matrices[*it] *
			get_model()->joints[*it]->matrix;

        joint_matrix_array.push_back(t);
    }

	math::obb<3> bounds(this->bounds_);
	bounds.transform(transform);

	for (typename Mesh::triangles_list_type::const_iterator it = this->triangles_list.begin();
		 it != this->triangles_list.end(); it++)
	{
		UberDrawCallPtr draw_call = get_model()->get_renderer()->create_uber_draw_call();

		draw_call->set_obb(bounds);
		draw_call->set_vertex_buffer(this->vertex_array_->get_vertex_buffer());
		draw_call->set_index_buffer(this->index_array_->get_index_buffer());
		draw_call->set_material(it->material_ptr);
		draw_call->set_minimal_vertex_index(it->minimal_vertex_index);
		draw_call->set_vertices_count(it->vertices_count);
		draw_call->set_first_index_offset(it->first_index_offset);
		draw_call->set_triangles_count(it->triangles_count);
		draw_call->set_world_matrix(transform);
		draw_call->set_camera(camera);
		draw_call->set_joint_matrix_array(joint_matrix_array);
		draw_call->set_always_visible(this->always_visible_);
		draw_call->commit();

		get_model()->get_scheduler()->add(draw_call);
	}
}

template class BasicAnimatedMesh<VERTEX_PNJWT>;
template class BasicAnimatedMesh<VERTEX_PTBNJWT>;

}
