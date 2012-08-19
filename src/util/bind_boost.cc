
#include <boost/filesystem/v3/path.hpp>
#include <boost/filesystem/v3/operations.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

namespace util
{

void bind_filesystem(lua_State* L)
{
	using namespace luabind;
	using namespace boost::filesystem;

	namespace fs = boost::filesystem;

	module(L, "boost")
	[
		namespace_("filesystem")
		[
			class_<fs::path>("path")
			.def(constructor<>())
			.def(constructor<char const *>())
			.def("string", (const std::string (fs::path::*)() const) &fs::path::string)
			.def("filename", &fs::path::filename)
		]
	 ];
}
void bind_boost(lua_State *L)
{
	bind_filesystem(L);
}

}

