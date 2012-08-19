
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <graphic/sampler.h>

namespace graphic {

void Sampler::add_sample(math::scalar time, math::matrix<4,4> const &m)
{
	sample s;
	s.time = time;
	s.transform = m;
	s.translation.set(m.ij[3][0], m.ij[3][1], m.ij[3][2]);
	s.rotation.set_unit(m);

	if (samples_.size()) {
		samples_.back().rotation_interpolator.setup(samples_.back().rotation, s.rotation);
	}

	samples_.push_back(s);
}

math::matrix<4,4> Sampler::get(math::scalar time) const {
	if (time <= samples_.front().time) return samples_.front().transform;
	if (time >= samples_.back().time) return samples_.back().transform;

	using namespace boost::lambda;
	samples_type::const_iterator it = std::lower_bound(samples_.begin(), samples_.end(), sample(time), &_1 ->* &sample::time < &_2 ->* &sample::time);

	math::scalar k = (time - (it - 1)->time) / (it->time - (it - 1)->time);
	math::scalar inv_k = 1.0f - k;

	math::matrix<4,4> m;
	m.rotation((it - 1)->rotation_interpolator.interpolate(k));
	m.translate((it - 1)->translation * inv_k + it->translation * k);
	return m;
}

}

