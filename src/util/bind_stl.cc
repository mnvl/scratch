
#include <math/vec.h>
#include <vector>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

namespace util
{

template <class T>
void bind_vector(lua_State* L, char const *name)
{
	using namespace luabind;

	module(L, "stl")
	[
		class_< std::vector<T> >(name)
		.def(constructor<>())
		.def("size", &std::vector<T>::size)
		.def("push_back", &std::vector<T>::push_back)
		.def("at", (T const &(std::vector<T>::*)(size_t) const) &std::vector<T>::at)
	 ];
}

void bind_stl(lua_State *L)
{
	bind_vector< math::vec<2, int> >(L, "vector_vec2i");
}

}
