#pragma once

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <luabind/lua_include.hpp>
#include "forward.h"

namespace graphic
{

class Scheduler: public boost::noncopyable {
public:
	Scheduler(RendererPtr const &renderer);
	~Scheduler();

	void add(DrawCallPtr const &draw_call);
	void flush();

	size_t get_visible_calls_count() const { return visible_calls_count_; }
	size_t get_invisible_calls_count() const { return invisible_calls_count_; }

	static void bind(lua_State *L);

private:
	typedef std::vector<DrawCallPtr> draw_calls_type;

	size_t visible_calls_count_, invisible_calls_count_;

	RendererPtr const &renderer_;
	draw_calls_type draw_calls_;
};

}
