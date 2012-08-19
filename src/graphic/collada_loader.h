#pragma once

#include <set>
#include <map>
#include <vector>
#include <stdexcept>
#include <boost/scoped_ptr.hpp>
#include <xercesc/dom/DOM.hpp>
#include <luabind/lua_include.hpp>
#include <math/vec.h>
#include <graphic/model.h>

namespace graphic {

class Renderer;

class ColladaLoader {
public:
	class load_error: public std::runtime_error {
	public:
		load_error(std::string const &message):
		runtime_error(message)
		{
		}
	};

	ColladaLoader(Model *model_ptr);
	~ColladaLoader();

	void load(std::string const &filename);
	void build_model();

	static void bind(lua_State *L);

private:
	Model *model_;

	class Impl;
	boost::scoped_ptr<Impl> impl_;
};

}
