
#include <boost/test/unit_test.hpp>
#include <util/logger.h>
#include "test_fixture.h"
#include "d3d9_renderer.h"

BOOST_AUTO_TEST_SUITE(renderer)

BOOST_FIXTURE_TEST_CASE(basic, graphic::TestFixture)
{
	graphic::D3D9RendererPtr renderer(new graphic::D3D9Renderer());
	renderer->postconstruct();
	renderer->reset_state();

	double t = SDL_GetTicks();

	while (SDL_GetTicks() - t < 1000)
	{
		graphic::TestFixture::process_events();

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_SUITE_END()
