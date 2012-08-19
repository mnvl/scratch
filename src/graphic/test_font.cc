
#include <SDL.h>
#include <boost/test/unit_test.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <util/get_sources_path.h>
#include "test_fixture.h"
#include "font.h"
#include "material.h"
#include "vertex_array.h"
#include "index_array.h"
#include "uber_draw_call.h"
#include "d3d9_renderer.h"

BOOST_AUTO_TEST_SUITE(font)

BOOST_FIXTURE_TEST_CASE(make_texture, graphic::TestFixture)
{
	graphic::RendererPtr renderer(new graphic::D3D9Renderer());
	renderer->postconstruct();

	graphic::FontPtr font(new graphic::Font(renderer, "arial.ttf", 72));

	graphic::TexturePtr texture = font->make_texture("bonjour", math::vec<4>(1, 1, 1, 0), math::vec<4>(0, 0.2f, 0.2f, 0));

	graphic::Texture::save_to_file(texture, "rendered_text_texture.bmp");
}

BOOST_FIXTURE_TEST_CASE(make_draw_call, graphic::TestFixture)
{
	graphic::RendererPtr renderer(new graphic::D3D9Renderer());
	renderer->postconstruct();

	graphic::FontPtr font(new graphic::Font(renderer, "arial.ttf", 72));

	boost::shared_ptr<graphic::Camera> camera(new graphic::Camera());
	camera->set_orthographic_projection(renderer->get_screen_width(), renderer->get_screen_height(), -1, 1000);

	math::matrix<4,4> view_matrix;
	view_matrix.translation(-renderer->get_screen_width() / 2, -renderer->get_screen_height() / 2, 0);
	view_matrix.scale(1, -1, 1);
	camera->set_view_matrix(view_matrix);

	graphic::MaterialPtr hacked_material = renderer->create_material();
	hacked_material->set_diffuse_texture(graphic::Texture::load_from_file(renderer,
		util::get_graphic_sources_path() + "/test_directions.png"));

	graphic::DrawCallPtr draw_call = font->make_draw_call(math::vec<3>(0, 0, 0), math::vec<3>(100, 0, 0),
		math::vec<3>(100, 100, 0), math::vec<3>(0, 100, 0), "bonjour", math::vec<4>(1, 1, 1, 0), math::vec<4>(0, 0.2f, 0.2f, 0));
	draw_call->set_camera(camera);
	draw_call->set_always_visible(true);
	draw_call->commit();

	graphic::DrawCallPtr hacked_draw_call = font->make_draw_call(math::vec<3>(0, 100, 0), math::vec<3>(100, 100, 0),
		math::vec<3>(100, 200, 0), math::vec<3>(0, 200, 0), "bonjour", math::vec<4>(1, 1, 1, 0), math::vec<4>(0, 0.2f, 0.2f, 0));
	hacked_draw_call->set_material(hacked_material);
	hacked_draw_call->set_camera(camera);
	hacked_draw_call->commit();

	boost::posix_time::ptime start_time(boost::posix_time::microsec_clock::local_time());
	math::scalar delta;
	math::scalar const DURATION = 5e6;

	while ((delta = math::scalar((boost::posix_time::microsec_clock::local_time() - start_time).total_microseconds())) < DURATION)
	{
		graphic::TestFixture::process_events();

		renderer->begin_frame();

		draw_call->execute();
		hacked_draw_call->execute();

		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_SUITE_END()
