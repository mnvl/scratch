#pragma once

#include <boost/cstdint.hpp>

namespace rpc
{

#pragma pack(push, 1)
struct Header
{
	boost::uint16_t id;
	boost::uint16_t len;
};
#pragma pack(pop)

}
