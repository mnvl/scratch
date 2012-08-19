#pragma once

#include <assert.h>
#include "scalar.h"
#include "vec.h"
#include "sphere.h"
#include "line.h"
#include "aabb.h"
#include "obb.h"
#include "contact_info.h"

namespace math
{

static scalar const COLLISION_EPSILON = 0.001f;

template<int N, class S>
bool collide(sphere<N, S> const &s1, sphere<N, S> const &s2, vec<N, S> const &vel, contact_info<2, S> &ci) {
	// |x1 - x2| = R1 + R2
	// x2(t) = x2 + v * t
	// |x1 - x2 - v*t| = R1 + R2
	// (x1 - x2 - v*t, x1 - x2 - v*t) = (R1 + R2)^2
	// (v, v) * t^2 - (v, x1 - x2) * 2 * t + (x1 - x2, x1 - x2) - (R1 + R2)^2 = 0
	// A = (v, v)
	// B = -2 * (v, x1 - x2)
	// C = (x1 - x2, x1 - x2) - (R1 + R2)^2

	S R = s1.radius + s2.radius;
	vec<N, S> delta = s1.centre - s2.centre;

	if (square(R) >= delta.length_sq()) {
		ci.penetration_depth = R - delta.length();

		if (ci.penetration_depth > COLLISION_EPSILON) {
			ci.penetrated = true;
			ci.position = s1.centre - math::normalize(delta) * (s1.radius - ci.penetration_depth);
			ci.normal = math::normalize(-delta);
			ci.time = 0;
			return ci.happened = true;
		}
	}

	ci.penetrated = false;

	S A = vel.length_sq();
	S B = -2 * (vel & delta);
	S C = delta.length_sq() - square(R);

	S discriminant = square(B) - 4 * A * C;
	if(discriminant < 0) return ci.happened = false;

	if(abs(discriminant) < EPSILON) {
		ci.time = -B / (2 * A);

		if (ci.time < 0) return ci.happened = false;
	} else {
		S discriminant_sqrt = sqrt(discriminant);
		S t1 = (-B + discriminant_sqrt) / (2 * A);
		S t2 = (-B - discriminant_sqrt) / (2 * A);

		if(t1>0) {
			if(t2>0) ci.time = std::min(t1,t2);
			else ci.time = t1;
		} else {
			if(t2>0) ci.time = t2;
			else return ci.happened = false;
		}
	}

	ci.position = s2.centre + vel * ci.time;
	ci.normal = math::normalize(ci.position - s1.centre);

	return ci.happened = true;
}

template<int N, class S>
bool collide(line<N, S> const &l, sphere<N, S> const &s, vec<N, S> const &vel, contact_info<2, S> &ci) {
	vec<N, S> p1, p2;

	if (s.query_intersection(l, p1, p2) == 2) {
		ci.position = (p1 + p2) / S(2);
		ci.penetration_depth = s.radius - (s.centre - ci.position).length();

		if (ci.penetration_depth < COLLISION_EPSILON) {
			ci.penetrated = false;
		} else {
			ci.penetrated = true;
			ci.normal = math::normalize(s.centre - ci.position);
			ci.time = 0;

			return ci.happened = true;
		}
	} else {
		ci.penetrated = false;
	}

	vec<N, S> l_dir = math::normalize(l.direction());
	vec<N, S> l_norm(-l_dir.y, l_dir.x);
	S l_vel = vel & l_norm;
	vec<N, S> l_norm_v = l_norm * sign(l_vel);

	ci.position = s.centre + l_norm_v * s.radius;

	if (!ray<N, S>(ci.position, l_norm_v * abs(l_vel)).query_intersection(l.get_ray(), ci.time)) {
		ci.happened = false;
	} else if (ci.time < 0) {
		ci.happened = false;
	} else {
		ci.normal = (((l_norm & l_norm_v) > 0) ? -l_norm : l_norm);
		ci.position += vel * ci.time;

		S pos = (ci.position - l.A) & l_dir;

		if (pos < 0 || pos * pos > l.length_sq()) {
			ci.happened = false;
		} else {
			ci.happened = true;
		}
	}

	contact_info<N, S> ci0;

	collide(sphere<N, S>(l.A, 0), s, vel, ci0);
	if (ci.worse_than(ci0)) {
		ci = ci0;
	}

	collide(sphere<N, S>(l.B, 0), s, vel, ci0);
	if (ci.worse_than(ci0)) {
		ci = ci0;
	}

	return ci.happened;
}

template<class S>
bool collide(obb<2, S> const &o, sphere<2, S> const &s, vec<2, S> const &vel, contact_info<2, S> &ci, bool not_from_inside = true) {
	ci.happened = false;

	if (vel.length_sq() < EPSILON) return false;

	if (not_from_inside && o.contains(s.centre)) {
		for (size_t i = 0; i < obb<2, S>::SIDES_COUNT; i++) {
			vec<2, S> vertex = o.get_vertex(i), normal = o.get_normal(i);

			contact_info<2, S> ci0;
			ci0.penetration_depth = -((s.centre - vertex) & normal) + s.radius;

			if (ci0.penetration_depth > 0) {
				ci0.penetrated = ci0.happened = true;
				ci0.position = s.centre + normal * ci0.penetration_depth;
				ci0.normal = normal;
				ci0.time = 0;

				if (ci.worse_than(ci0)) {
					ci = ci0;
				}
			}
		}
	}

	for (size_t i = 0; i < obb<2, S>::SIDES_COUNT; i++) {
		line<2, S> side = o.get_side(i);

		contact_info<2, S> ci0;
		collide(side, s, vel, ci0);

		if (ci.worse_than(ci0)) {
			ci = ci0;
		}
	}

	return ci.happened;
}

template<int N, class S>
bool collide(aabb<N, S> const &a, sphere<N, S> const &s, vec<N, S> const &vel, contact_info<N, S> &ci, bool not_from_inside = true) {
	return collide(obb<N, S>(a), s, vel, ci, not_from_inside);
}

}

