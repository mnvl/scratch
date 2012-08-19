#pragma once

#include <luabind/lua_include.hpp>
#include "scalar.h"
#include "vec.h"

namespace math
{

template<int N, class T = scalar>
struct contact_info {
	static size_t const ARITY = N;
	typedef T scalar_t;
	typedef vec<ARITY, scalar_t> vec_t;

	bool happened, penetrated;
	vec_t position, normal;
	scalar_t time, penetration_depth;

	bool worse_than(contact_info const &ci) const {
		if (ci.happened == false) return false;
		if (happened == false) return true;
		if (penetrated == false && ci.penetrated == true) return true;
		if (penetrated == true && ci.penetrated == true) return penetration_depth > ci.penetration_depth;
		return time > ci.time;
	}

	static void bind(lua_State *L, char const *name);
};

void bind_contact_info(lua_State *L);

}
