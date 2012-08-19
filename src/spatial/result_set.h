#pragma once

#include <set>
#include <boost/shared_ptr.hpp>
#include <luabind/lua_include.hpp>
#include "shape.h"

namespace spatial
{

typedef boost::shared_ptr<class ResultSet> ResultSetPtr;

class ResultSet {
public:
	typedef std::set<ShapePtr> shapes_t;

	void insert(ShapePtr const &shape)
	{
		shapes_.insert(shape);
	}

	bool have(ShapePtr const &shape)
	{
		return shapes_.find(shape) != shapes_.end();
	}

	shapes_t &get_shapes()
	{
		return shapes_;
	}

	static void bind(lua_State *L);

private:
	shapes_t shapes_;
};

}
