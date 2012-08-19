
#include <SDL.h>
#include <boost/test/unit_test.hpp>
#include <util/get_sources_path.h>
#include "test_fixture.h"
#include "texture.h"
#include "d3d9_renderer.h"

BOOST_AUTO_TEST_SUITE(window_resize)

BOOST_FIXTURE_TEST_CASE(basic, graphic::TestFixture)
{
	graphic::RendererPtr renderer(new graphic::D3D9Renderer());

	for (int i = 0; i < 10; i++)
	{
		graphic::TestFixture::process_events();

		if (i == 5) MoveWindow((HWND) renderer->get_window_handle(), 100, 100, 900, 700, TRUE);

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_FIXTURE_TEST_CASE(with_postconstruct, graphic::TestFixture)
{
	graphic::RendererPtr renderer(new graphic::D3D9Renderer());
	renderer->postconstruct();

	for (int i = 0; i < 10; i++)
	{
		graphic::TestFixture::process_events();

		if (i == 5) MoveWindow((HWND) renderer->get_window_handle(), 100, 100, 900, 700, TRUE);

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_FIXTURE_TEST_CASE(with_postconstruct_and_vertex_array, graphic::TestFixture)
{
	graphic::RendererPtr renderer(new graphic::D3D9Renderer());
	renderer->postconstruct();

	boost::shared_ptr<graphic::VertexArrayPT> vertex_array1;
	boost::shared_ptr<graphic::VertexArrayPT> vertex_array2;
	boost::shared_ptr<graphic::VertexArrayPT> vertex_array3;
	boost::shared_ptr<graphic::VertexArrayPT> vertex_array4;

	for (int i = 0; i < 20; i++)
	{
		graphic::TestFixture::process_events();

		if (i == 5)
		{
			vertex_array1 = renderer->create_vertex_array<graphic::VERTEX_PT>(100, false, false);
			vertex_array2 = renderer->create_vertex_array<graphic::VERTEX_PT>(100, false, true);
			vertex_array3 = renderer->create_vertex_array<graphic::VERTEX_PT>(100, true, false);
			vertex_array4 = renderer->create_vertex_array<graphic::VERTEX_PT>(100, true, true);
		}

		if (i == 15)
		{
			MoveWindow((HWND) renderer->get_window_handle(), 100, 100, 900, 700, TRUE);
		}

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_FIXTURE_TEST_CASE(with_postconstruct_and_index_array, graphic::TestFixture)
{
	graphic::RendererPtr renderer(new graphic::D3D9Renderer());
	renderer->postconstruct();

	boost::shared_ptr<graphic::IndexArray16> index_array1;
	boost::shared_ptr<graphic::IndexArray16> index_array2;
	boost::shared_ptr<graphic::IndexArray16> index_array3;
	boost::shared_ptr<graphic::IndexArray16> index_array4;

	for (int i = 0; i < 20; i++)
	{
		graphic::TestFixture::process_events();

		if (i == 5)
		{
			index_array1 = renderer->create_index_array<graphic::INDEX_U16>(100, false, false);
			index_array2 = renderer->create_index_array<graphic::INDEX_U16>(100, false, true);
			index_array3 = renderer->create_index_array<graphic::INDEX_U16>(100, true, false);
			index_array4 = renderer->create_index_array<graphic::INDEX_U16>(100, true, true);
		}

		if (i == 15)
		{
			MoveWindow((HWND) renderer->get_window_handle(), 100, 100, 900, 700, TRUE);
		}

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_FIXTURE_TEST_CASE(with_postconstruct_and_texture, graphic::TestFixture)
{
	graphic::RendererPtr renderer(new graphic::D3D9Renderer());
	renderer->postconstruct();

	boost::shared_ptr<graphic::Texture> texture;

	for (int i = 0; i < 20; i++)
	{
		graphic::TestFixture::process_events();

		if (i == 5)
		{
			texture = graphic::Texture::load_from_file(renderer, util::get_graphic_sources_path() + "/test_white.png");
		}

		if (i == 15)
		{
			MoveWindow((HWND) renderer->get_window_handle(), 100, 100, 900, 700, TRUE);
		}

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_SUITE_END()
