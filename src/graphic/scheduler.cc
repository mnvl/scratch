
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include "draw_call.h"
#include "renderer.h"
#include "scheduler.h"

namespace graphic
{

Scheduler::Scheduler(RendererPtr const &renderer):
	renderer_(renderer)
{
	draw_calls_.reserve(4096);
}

Scheduler::~Scheduler()
{
}

void Scheduler::add(DrawCallPtr const &draw_call)
{
	draw_calls_.push_back(draw_call);
}

void Scheduler::flush()
{
	visible_calls_count_ = invisible_calls_count_ = 0;

	std::sort(draw_calls_.begin(), draw_calls_.end(), *boost::lambda::_1 < *boost::lambda::_2);

	for(draw_calls_type::const_iterator it = draw_calls_.begin(); it != draw_calls_.end(); ++it)
	{
		if ((*it)->is_visible())
		{
			++visible_calls_count_;
			(*it)->execute();
		}
		else
		{
			++invisible_calls_count_;
		}
	}

	draw_calls_.clear();
}

}
