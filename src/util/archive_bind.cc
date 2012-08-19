
#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "archive.h"

namespace util
{

void ArchiveReader::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "util")
	[
		class_<ArchiveReader, ArchiveReaderPtr>("ArchiveReader")
		.def(constructor<>())
		.def("open", &ArchiveReader::open)
		.def("read_string", &ArchiveReader::read_string)
		.def("close", &ArchiveReader::close)
	];
}

void ArchiveWriter::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "util")
	[
		class_<ArchiveWriter, ArchiveWriterPtr>("ArchiveWriter")
		.def(constructor<>())
		.def("create", &ArchiveWriter::create)
		.def("write_data", (void (ArchiveWriter::*)(std::string const &, std::string const &)) &ArchiveWriter::write_data)
		.def("write_file", (void (ArchiveWriter::*)(std::string const &, std::string const &)) &ArchiveWriter::write_file)
		.def("close", &ArchiveWriter::close)
	];
}

}
