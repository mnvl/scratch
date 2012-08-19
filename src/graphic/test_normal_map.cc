
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
#include "uber_shader.h"
#include "d3d9_shader.h"
#include "d3d9_renderer.h"

BOOST_AUTO_TEST_SUITE(normal_map)

BOOST_FIXTURE_TEST_CASE(basic, graphic::TestFixture)
{
	std::string const &src = util::get_graphic_sources_path();

	boost::shared_ptr<graphic::D3D9Renderer> renderer(new graphic::D3D9Renderer());

	renderer->postconstruct();

	graphic::MaterialPtr material = renderer->create_material();
	material->set_diffuse_texture(src + "test_gray.png");
	material->set_normal_map_texture(src + "test_normal_map.jpeg");

	boost::shared_ptr<graphic::VertexArrayPTBNT> vertex_array = renderer->create_vertex_array<graphic::VERTEX_PTBNT>(4);

	{
		graphic::VERTEX_PTBNT *v = vertex_array->lock(true);

		v->position  .set(0, 1, 0);
		v->tangent   .set(1, 0, 0);
		v->binormal  .set(0, 1, 0);
		v->normal    .set(0, 0, 1);
		v->texcoords .set(0, 1);
		++v;

		v->position  .set(0, 0, 0);
		v->tangent   .set(1, 0, 0);
		v->binormal  .set(0, 1, 0);
		v->normal    .set(0, 0, 1);
		v->texcoords .set(0, 0);
		++v;

		v->position  .set(1, 0, 0);
		v->tangent   .set(1, 0, 0);
		v->binormal  .set(0, 1, 0);
		v->normal    .set(0, 0, 1);
		v->texcoords .set(1, 0);
		++v;

		v->position  .set(1, 1, 0);
		v->tangent   .set(1, 0, 0);
		v->binormal  .set(0, 1, 0);
		v->normal    .set(0, 0, 1);
		v->texcoords .set(1, 1);
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
	shader->set_lighting_enabled(true);

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

	graphic::UberShader::POINT_LIGHTS lights;
	memset(&lights, 0, sizeof(lights));

	lights.items[0].position = math::vec<3>(0, 0, 10);
	lights.items[0].color = math::vec<3>(1, 1, 1);
	lights.items[0].inv_attenuation = 1.0f;

	lights.items[1].color = math::vec<3>(1, 1, 1);
	lights.items[1].inv_attenuation = 0.90f;

#pragma pack(push, 1)
	struct POINT_VERTEX
	{
		math::vec<3> position;
		math::vec<4> color;
	};
#pragma pack(pop)

	POINT_VERTEX point;
	point.color.set(1.0f, 1.0f, 0, 1.0f);

	graphic::UberShader::InstancePtr shader_instance = shader->get_instance(graphic::VERTEX_PTBNT::FORMAT, material);

	boost::shared_ptr<graphic::D3D9Shader> d3d9_shader =
		boost::static_pointer_cast<graphic::D3D9Shader>(shader_instance->get_shader());

	boost::posix_time::ptime start_time(boost::posix_time::microsec_clock::local_time());
	math::scalar delta;
	math::scalar const DURATION = 5e6;

	HRESULT hr;

	while ((delta = math::scalar((boost::posix_time::microsec_clock::local_time() - start_time).total_microseconds())) < DURATION)
	{
		graphic::TestFixture::process_events();

		math::scalar angle = delta / DURATION * math::PI * 4;

		point.position.set(-cos(angle) * 0.5f + 0.5f, 0.5f, sin(angle) * 0.5f);
		lights.items[1].position = point.position;
		shader_instance->point_lights->set(&lights, sizeof(lights));

		renderer->begin_frame();

		draw_call->execute();

		hr = renderer->get_device()->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
		if (FAILED(hr)) throw graphic::directx_error(hr);

		float point_size = 32.0f;
		hr = renderer->get_device()->SetRenderState(D3DRS_POINTSIZE, *((DWORD*) &point_size));
		if (FAILED(hr)) throw graphic::directx_error(hr);

		hr = renderer->get_device()->DrawPrimitiveUP(D3DPT_POINTLIST, 1, &point, sizeof(POINT_VERTEX));
		if (FAILED(hr)) throw graphic::directx_error(hr);

		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_SUITE_END()
