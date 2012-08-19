#pragma once

#include <boost/tuple/tuple.hpp>
#include "vec.h"

namespace math
{

// based on http://lifc.univ-fcomte.fr/~ededu/projects/bresenham/
template<class T=int>
class bresenham_supercover {
public:
	typedef T int_t;
	typedef vec<2, int_t> vec_t;

	bresenham_supercover(vec_t const &v1, vec_t const &v2);
	~bresenham_supercover();

	boost::tuple<vec_t, bool> get();

private:
	int_t i_;
	int_t x1_, y1_, x2_, y2_;
	int_t xstep_, ystep_;
	int_t error_, errorprev_;
	int_t x_, y_;
	int_t dx_, dy_, ddx_, ddy_;
	vec_t results_[3];
	size_t firstresult_, nresults_;
	bool finished_;

	void next();
};

}

