
#include "directx_error.h"
#include <util/logger.h>
#include "uber_shader.h"
#include "uber_draw_call.h"
#include "static_model.h"
#include "renderer.h"
#include "scheduler.h"
#include "static_mesh.h"

namespace graphic
{

template<class T>
BasicStaticMesh<T>::BasicStaticMesh(Model *model_ptr):
    Mesh(model_ptr),
	initialized_vertex_components_(0),
	always_visible_(false)
{
}

template<class T>
BasicStaticMesh<T>::~BasicStaticMesh()
{
}

template<class T>
math::aabb<3> const &BasicStaticMesh<T>::get_bounds() const
{
	return bounds_;
}

template<class T>
MeshPtr BasicStaticMesh<T>::shallow_copy() const
{
	return MeshPtr(new BasicStaticMesh<T>(*this));
}

template<class T>
BasicStaticModel<T> *BasicStaticMesh<T>::get_model()
{
	return (BasicStaticModel<T> *) model_;
}

template<class T>
void BasicStaticMesh<T>::check()
{
	Mesh::check();

	if (initialized_vertex_components_ != vertex_type::FORMAT)
	{
		LOG_WARN("some vertex components are not initialized, format = " << vertex_type::FORMAT
			<< ", initialized components = " << initialized_vertex_components_);
	}
}

template<class T>
void BasicStaticMesh<T>::draw(Joint *joint_ptr, CameraPtr const &camera, math::matrix<4,4> const &world_matrix)
{
	math::matrix<4,4> transform = joint_ptr->matrix * world_matrix;

	for (triangles_list_type::const_iterator it = triangles_list.begin(); it != triangles_list.end(); it++)
	{
		UberDrawCallPtr draw_call = get_model()->get_renderer()->create_uber_draw_call();

		{
			math::obb<3> bounds(bounds_);
			bounds.transform(transform);
			draw_call->set_obb(bounds);
		}

		draw_call->set_vertex_buffer(vertex_array_->get_vertex_buffer());
		draw_call->set_index_buffer(index_array_->get_index_buffer());
		draw_call->set_material(it->material_ptr);
		draw_call->set_minimal_vertex_index(it->minimal_vertex_index);
		draw_call->set_vertices_count(vertex_array_->get_count());
		draw_call->set_first_index_offset(it->first_index_offset);
		draw_call->set_triangles_count(it->triangles_count);
		draw_call->set_world_matrix(transform);
		draw_call->set_camera(camera);
		draw_call->set_always_visible(always_visible_);
		draw_call->commit();

		get_model()->get_scheduler()->add(draw_call);
	}
}

template<class V>
void BasicStaticMesh<V>::set_vertices(GenericVertex *vertices, GenericVertex::format_type format, size_t count)
{
	LOG_DEBUG("vertices: count = " << count << ", format = " << std::hex << format);

	if (!vertex_array_)
	{
		vertex_array_ = get_model()->get_renderer()->
			Renderer::create_vertex_array<vertex_type>(count, false, true);

		vertex_array_->lock(true);
		memset(vertex_array_->begin(), 0, vertex_array_->get_size());
	}
	else
	{
		vertex_array_->lock(false);
	}

	if (initialized_vertex_components_ == 0 || vertex_type::FORMAT == format)
	{
		vertex_type *p = vertex_array_->begin();
		GenericVertex *vertex = vertices;

		for (size_t i = 0; i < count; i++, p++, vertex++)
		{
			*p = *vertex;
		}
	}
	else
	{
		vertex_type *p = vertex_array_->begin();
		GenericVertex *vertex = vertices;

		for (size_t i = 0; i < count; i++, p++, vertex++)
		{
			p->assign(*vertex, format);
		}
	}

	if (format & GenericVertex::FORMAT_POSITION)
	{
		GenericVertex *vertex = vertices;

		bounds_.lo = bounds_.hi = vertex->position;

		for (size_t i = 0; i < count; i++, vertex++)
		{
			bounds_.extend(vertex->position);
		}

		LOG_DEBUG("bounds = " << bounds_);
	}

	initialized_vertex_components_ |= format;

	vertex_array_->unlock();
}

template<class T>
void BasicStaticMesh<T>::set_indices(unsigned *buffer, size_t count)
{
	LOG_DEBUG("indices: count = " << count);

	index_array_ = get_model()->get_renderer()->
		Renderer::create_index_array<INDEX_U16>(count, false, true);

    index_array_->lock(false);

    index_type *p = index_array_->begin();

    for (size_t i = 0; i < count; i++, p++, buffer++)
    {
        *p = *buffer;
    }

    index_array_->unlock();
}

template<class T>
void BasicStaticMesh<T>::set_always_visible(bool flag)
{
	always_visible_ = flag;
}

template class BasicStaticMesh<VERTEX_PNT>;
template class BasicStaticMesh<VERTEX_PTBNT>;
template class BasicStaticMesh<VERTEX_PNJWT>;
template class BasicStaticMesh<VERTEX_PTBNJWT>;

}
