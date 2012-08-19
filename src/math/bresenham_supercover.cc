
#include "bresenham_supercover.h"

namespace math
{

template<class T>
bresenham_supercover<T>::bresenham_supercover(vec_t const &v1, vec_t const &v2):
	x1_(v1.x),
	y1_(v1.y),
	x2_(v2.x),
	y2_(v2.y)
{
	x_ = x1_;
	y_ = y1_;

	dx_ = x2_ - x1_;
	dy_ = y2_ - y1_;

	if (dy_ < 0)
	{
		ystep_ = -1;
		dy_ = -dy_;
	}
	else
	{
		ystep_ = 1;
	}

	if (dx_ < 0)
	{
		xstep_ = -1;
		dx_ = -dx_;
	}
	else
	{
		xstep_ = 1;
	}

	ddx_ = 2 * dx_;
	ddy_ = 2 * dy_;

	if (ddx_ > ddy_) errorprev_ = error_ = dx_;
	else  errorprev_ = error_ = dy_;

	i_ = 0;

	results_[0].set(x1_, y1_);
	firstresult_ = 0;
	nresults_ = 1;

	finished_ = false;
}

template<class T>
bresenham_supercover<T>::~bresenham_supercover()
{
}

template<class T>
boost::tuple<typename bresenham_supercover<T>::vec_t, bool> bresenham_supercover<T>::get()
{
	if (firstresult_ < nresults_)
	{
		return boost::make_tuple(results_[firstresult_++], false);
	}

	firstresult_ = nresults_ = 0;
	next();

	if (firstresult_ < nresults_)
	{
		return boost::make_tuple(results_[firstresult_++], false);
	}

	return boost::make_tuple(vec_t(), finished_);
}

template<class T>
void bresenham_supercover<T>::next()
{
	if (ddx_ >= ddy_)
	{
		if (i_ == dx_)
		{
			finished_ = true;
			return;
		}

		x_ += xstep_;
		error_ += ddy_;

		if (error_ > ddx_)
		{
			y_ += ystep_;
			error_ -= ddx_;

			if (error_ + errorprev_ < ddx_)
			{
				results_[0].set(x_, y_ - ystep_);
				nresults_ = 1;
			}
			else if (error_ + errorprev_ > ddx_)
			{
				results_[0].set(x_ - xstep_, y_);
				nresults_ = 1;
			}
			else
			{
				results_[0].set(x_ - xstep_, y_);
				results_[1].set(x_, y_ - ystep_);
				nresults_ = 2;
			}
		}

		results_[nresults_++].set(x_, y_);
		errorprev_ = error_;
	}
	else
	{
		if (i_ == dy_)
		{
			finished_ = true;
			return;
		}

		y_ += ystep_;
		error_ += ddx_;

		if (error_ > ddy_)
		{
			x_ += xstep_;
			error_ -= ddy_;

			if (error_ + errorprev_ < ddy_)
			{
				results_[0].set(x_ - xstep_, y_);
				nresults_ = 1;
			}
			else if (error_ + errorprev_ > ddy_)
			{
				results_[0].set(x_, y_ - ystep_);
				nresults_ = 1;
			}
			else
			{
				results_[0].set(x_ - xstep_, y_);
				results_[1].set(x_, y_ - ystep_);
				nresults_ = 2;
			}
		}

		results_[nresults_++].set(x_, y_);
		errorprev_ = error_;
	}

	++i_;
}

template class bresenham_supercover<>;

}

