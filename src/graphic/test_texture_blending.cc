
#include <SDL.h>
#include <boost/test/unit_test.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <util/get_sources_path.h>
#include "directx_error.h"
#include "test_fixture.h"
#include "material.h"
#include "vertex_array.h"
#include "index_array.h"
#include "uber_draw_call.h"
#include "d3d9_renderer.h"

BOOST_AUTO_TEST_SUITE(texture_blending)

BOOST_FIXTURE_TEST_CASE(blending_weights_in_vertex, graphic::TestFixture)
{
	std::string const &src = util::get_graphic_sources_path();

	boost::shared_ptr<graphic::D3D9Renderer> renderer(new graphic::D3D9Renderer());
	renderer->postconstruct();

	graphic::MaterialPtr material = renderer->create_material();
	material->set_diffuse_texture(src + "test_gray.png", 0);
	material->set_diffuse_texture(src + "test_brick_rounded.jpeg", 3);

	boost::shared_ptr<graphic::VertexArrayPNTB> vertex_array = renderer->create_vertex_array<graphic::VERTEX_PNTB>(4);

	{
		graphic::VERTEX_PNTB *v = vertex_array->lock(true);

		v->position.set(0, 1, 0);
		v->normal.set(0, 0, 1);
		v->texcoords.set(0, 1);
		v->texutre_blend_weights.set(1, 0, 0, 0);
		++v;

		v->position.set(0, 0, 0);
		v->normal.set(0, 0, 1);
		v->texcoords.set(0, 0);
		v->texutre_blend_weights.set(0, 1, 0, 0);
		++v;

		v->position.set(1, 0, 0);
		v->normal.set(0, 0, 1);
		v->texcoords.set(1, 0);
		v->texutre_blend_weights.set(0, 0, 1, 0);
		++v;

		v->position.set(1, 1, 0);
		v->normal.set(0, 0, 1);
		v->texcoords.set(1, 1);
		v->texutre_blend_weights.set(0, 0, 0, 1);
		++v;

		vertex_array->unlock();
	}

	boost::shared_ptr<graphic::IndexArray16> index_array = renderer->create_index_array<graphic::INDEX_U16>(6);

	{
		graphic::INDEX_U16 *i = index_array->lock(true);

		*i++ = 0;
		*i++ = 1;
		*i++ = 2;
		*i++ = 2;
		*i++ = 3;
		*i++ = 0;

		index_array->unlock();
	}

	graphic::UberShaderPtr shader = renderer->get_uber_shader();

	boost::shared_ptr<graphic::Camera> camera(new graphic::Camera());
	camera->set_orthographic_projection(2, 2 * math::scalar(renderer->get_screen_height()) / renderer->get_screen_width(), 0, 2);
	camera->set_eye_lookat_up(math::vec<3>(0.5f, 0.5f, 1), math::vec<3>(0.5f, 0.5f, 0), math::vec<3>(0, 1, 0));

	math::matrix<4,4> world_matrix;
	world_matrix.identity();

	graphic::UberDrawCallPtr draw_call = renderer->create_uber_draw_call();

	draw_call->set_always_visible(true);
	draw_call->set_vertex_buffer(vertex_array->get_vertex_buffer());
	draw_call->set_index_buffer(index_array->get_index_buffer());
	draw_call->set_material(material);
	draw_call->set_vertices_count(4);
	draw_call->set_triangles_count(2);
	draw_call->set_camera(camera);
	draw_call->commit();

	boost::posix_time::ptime start_time(boost::posix_time::microsec_clock::local_time());
	math::scalar delta;
	math::scalar const DURATION = 5e6;

	while ((delta = math::scalar((boost::posix_time::microsec_clock::local_time() - start_time).total_microseconds())) < DURATION)
	{
		graphic::TestFixture::process_events();

		renderer->begin_frame();

		draw_call->execute();

		renderer->finish_frame();
	}
}

BOOST_FIXTURE_TEST_CASE(blending_weights_in_texture, graphic::TestFixture)
{
	std::string const &src = util::get_graphic_sources_path();

	boost::shared_ptr<graphic::D3D9Renderer> renderer(new graphic::D3D9Renderer());

	renderer->postconstruct();

	graphic::MaterialPtr material = renderer->create_material();
	material->set_diffuse_texture(src + "test_gray.png", 0);
	material->set_diffuse_texture(src + "test_brick_rounded.jpeg", 2);
	material->set_texture_blending_weights_texture(src + "test_texture_blending_weights.png");

	boost::shared_ptr<graphic::VertexArrayPNT> vertex_array = renderer->create_vertex_array<graphic::VERTEX_PNT>(4);

	{
		graphic::VERTEX_PNT *v = vertex_array->lock(true);

		v->position.set(0, 1, 0);
		v->normal.set(0, 0, 1);
		v->texcoords.set(0, 1);
		++v;

		v->position.set(0, 0, 0);
		v->normal.set(0, 0, 1);
		v->texcoords.set(0, 0);
		++v;

		v->position.set(1, 0, 0);
		v->normal.set(0, 0, 1);
		v->texcoords.set(1, 0);
		++v;

		v->position.set(1, 1, 0);
		v->normal.set(0, 0, 1);
		v->texcoords.set(1, 1);
		++v;

		vertex_array->unlock();
	}

	boost::shared_ptr<graphic::IndexArray16> index_array = renderer->create_index_array<graphic::INDEX_U16>(6);

	{
		graphic::INDEX_U16 *i = index_array->lock(true);

		*i++ = 0;
		*i++ = 1;
		*i++ = 2;
		*i++ = 2;
		*i++ = 3;
		*i++ = 0;

		index_array->unlock();
	}

	graphic::UberShaderPtr shader = renderer->get_uber_shader();

	boost::shared_ptr<graphic::Camera> camera(new graphic::Camera());
	camera->set_orthographic_projection(2, 2 * math::scalar(renderer->get_screen_height()) / renderer->get_screen_width(), 0, 2);
	camera->set_eye_lookat_up(math::vec<3>(0.5f, 0.5f, 1), math::vec<3>(0.5f, 0.5f, 0), math::vec<3>(0, 1, 0));

	math::matrix<4,4> world_matrix;
	world_matrix.identity();

	graphic::UberDrawCallPtr draw_call = renderer->create_uber_draw_call();

	draw_call->set_always_visible(true);
	draw_call->set_vertex_buffer(vertex_array->get_vertex_buffer());
	draw_call->set_index_buffer(index_array->get_index_buffer());
	draw_call->set_material(material);
	draw_call->set_vertices_count(4);
	draw_call->set_triangles_count(2);
	draw_call->set_camera(camera);
	draw_call->commit();

	boost::posix_time::ptime start_time(boost::posix_time::microsec_clock::local_time());
	math::scalar delta;
	math::scalar const DURATION = 5e6;

	while ((delta = math::scalar((boost::posix_time::microsec_clock::local_time() - start_time).total_microseconds())) < DURATION)
	{
		graphic::TestFixture::process_events();

		renderer->begin_frame();

		draw_call->execute();

		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_SUITE_END()
