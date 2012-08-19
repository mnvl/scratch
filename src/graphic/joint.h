#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <luabind/lua_include.hpp>
#include <math/matrix.h>
#include "forward.h"

namespace graphic {

class Joint {
public:
	struct transform {
		transform(std::string const &s, math::matrix<4,4> const &m):
		sid(s), matrix(m)
		{
		}

		std::string sid;
		math::matrix<4,4> matrix;
	};

	typedef std::vector<JointWeakPtr> childs_type;

	size_t index;
	std::string id, name;
	JointWeakPtr parent;
	childs_type childs;
	std::vector<transform> transforms;
	mutable math::matrix<4,4> matrix;
	boost::weak_ptr<Mesh> mesh_ptr;

	Joint(size_t i):
		index(i)
	{
	}

	math::matrix<4,4> &get_transform_by_sid(std::string const &sid){
		for (std::vector<transform>::iterator it = transforms.begin(); it != transforms.end(); it++)
			if (it->sid == sid) return it->matrix;
		throw std::logic_error("transform sid = " + sid + " not found");
	}

	void build_frame_transforms()
	{
		matrix.identity();

		for (std::vector<transform>::reverse_iterator it = transforms.rbegin();
			it != transforms.rend(); it++)
		{
			matrix *= it->matrix;
		}

		if (JointPtr p = parent.lock()) matrix *= p->matrix;

		for (childs_type::iterator it = childs.begin(); it != childs.end(); it++)
		{
			if (JointPtr child = it->lock()) child->build_frame_transforms();
		}
	}

	static void bind(lua_State *L);
};

}

