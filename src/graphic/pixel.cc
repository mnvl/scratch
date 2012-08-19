
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include "pixel.h"

namespace graphic
{

static PixelFormat pixel_format_X8R8G8B8 = {
	math::vec<4, unsigned>(0x0000FF0000u, 0x0000FF00u, 0x000000FFu, 0xFF000000u),
	D3DFMT_X8R8G8B8,
	32
};

static PixelFormat pixel_format_A8R8G8B8 = {
	math::vec<4, unsigned>(0x0000FF0000u, 0x0000FF00u, 0x000000FFu, 0xFF000000u),
	D3DFMT_A8R8G8B8,
	32
};

PixelFormat const &PixelFormat::get(unsigned pixel_format) {
	switch (pixel_format) {
	case X8R8G8B8:
		return pixel_format_X8R8G8B8;

	case A8R8G8B8:
		return pixel_format_A8R8G8B8;

	default:
		throw std::runtime_error("unknown pixel format " +
			boost::lexical_cast<std::string>(pixel_format));
	}
}

}
