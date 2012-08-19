#pragma once

#include <iosfwd>
#include <string>
#include <algorithm>
#include <luabind/lua_include.hpp>
#include "scalar.h"

namespace math
{

template<int N,class T=scalar> class vec;
template<class T> class quaternion;
template<class T> class vec<2,T>;
template<class T> class vec<3,T>;
template<class T> class vec<4,T>;

template<class T> inline T dot_product(const vec<2,T> &a,const vec<2,T> &b);
template<class T> inline T dot_product(const vec<3,T> &a,const vec<3,T> &b);
template<class T> inline T dot_product(const vec<4,T> &a,const vec<4,T> &b);
template<int N,class T> inline T dot_product(const vec<N,T> &a,const vec<N,T> &b);

template<class T> inline void cross_product(vec<3,T> &u,const vec<3,T> &v1,const vec<3,T> &v2);

template<class T> inline void minmax(vec<2,T> &v1,vec<2,T> &v2);
template<class T> inline void minmax(vec<3,T> &v1,vec<3,T> &v2);
template<class T> inline void minmax(vec<4,T> &v1,vec<4,T> &v2);

template<int N,class T> inline vec<N,T> normalize(const vec<N,T> &v);

void bind_vec(lua_State *L);

#pragma pack(push, 1)
template<class T>
class vec<2,T> {
public:
	typedef T scalar_t;
	static size_t const ARITY = 2;
	typedef size_t mask_t;

	union {
		struct {
			scalar_t x,y;
		};

		struct {
			scalar_t u,v;
		};

		struct {
			scalar_t s,t;
		};

		struct {
			scalar_t A,B;
		};

		scalar_t i[ARITY];
	};

	vec() {
	}

	vec(const vec &v):
	x(v.x), y(v.y) {
	}

	template<class type>
	vec(const vec<ARITY,type> &v):
		x((scalar_t) v.x), y((scalar_t) v.y)
	{
	}


	vec(scalar_t x0,scalar_t y0):
		x(x0), y(y0)
	{
	}

	void set(scalar_t x0, scalar_t y0)
	{
		x = x0;
		y = y0;
	}

	void set_all(scalar_t a)
	{
		set(a, a);
	}

	vec &operator =(const vec &v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	vec operator -() const
	{
		return vec(-x, -y);
	}

	vec &operator *=(const scalar_t k)
	{
		x *= k;
		y *= k;
		return *this;
	}

	vec &operator /=(const scalar_t k)
	{
		x /= k;
		y /= k;
		return *this;
	}

	vec &operator +=(const vec &v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	vec &operator -=(const vec &v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	vec &operator *=(const vec &v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	vec &operator /=(const vec &v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	vec operator +(vec const &v) const
	{
		return vec(x + v.x, y + v.y);
	}

	vec operator -(vec const &v) const
	{
		return vec(x - v.x, y - v.y);
	}

	vec operator *(vec const &v) const
	{
		return vec(x * v.x, y * v.y);
	}

	vec operator /(vec const &v) const
	{
		return vec(x / v.x, y / v.y);
	}

	vec operator *(scalar_t const k) const
	{
		return vec(x * k, y * k);
	}

	vec operator /(scalar_t const k) const
	{
		return vec(x / k, y / k);
	}

	bool operator <(vec const &rhs) const
	{
		return x < rhs.x && y < rhs.y;
	}

	bool operator <=(vec const &rhs) const
	{
		return x <= rhs.x && y <= rhs.y;
	}

	bool operator >(vec const &rhs) const
	{
		return x > rhs.x && y > rhs.y;
	}

	bool operator >=(vec const &rhs) const
	{
		return x >= rhs.x && y >= rhs.y;
	}

	bool operator ==(vec const &rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	bool operator !=(vec const &rhs) const
	{
		return x != rhs.x || y != rhs.y;
	}

	scalar_t length_sq() const
	{
		return x * x + y * y;
	}

	scalar_t length() const
	{
		return sqrt(length_sq());
	}

	scalar_t length_manhattan() const
	{
		return abs(x) + abs(y);
	}

	void normalize()
	{
		scalar_t l = length();
		if(abs(l) > 0) *this /= l;
	}

	void negate()
	{
		x = -x;
		y = -y;
	}

	bool is_collinear(vec const &v, scalar_t epsilon = EPSILON) const
	{
		return abs(x*v.y - y*v.x) < epsilon;
	}

	void to_polar(scalar_t &modulus, scalar_t &angle) const;

	vec perpendicular() const { return vec(y, -x); }

	template<class Archive>
	void serialize(Archive &archive, unsigned const /*file_version*/)
	{
		archive & x & y;
	}
};
#pragma pack(pop)

#pragma pack(push, 1)
template<class T>
class vec<3,T> {
public:
	typedef T scalar_t;
	static size_t const ARITY = 3;
	typedef vec<2, scalar_t> vec2_t;
	typedef size_t mask_t;

	union {
		struct {
			scalar_t x,y,z;
		};

		struct {
			scalar_t r,g,b;
		};

		struct {
			scalar_t A,B,C;
		};

		scalar_t i[ARITY];
	};

	vec()
	{
	}

	vec(vec const &v):
		x(v.x), y(v.y), z(v.z)
	{
	}

	vec(vec<ARITY - 1, scalar_t> const &v, scalar_t const z0 = 0):
	x(v.x), y(v.y), z(z0)
	{
	}

	vec(scalar_t x0,scalar_t y0,scalar_t z0):
		x(x0), y(y0), z(z0)
	{
	}

	void set(scalar_t x0,scalar_t y0,scalar_t z0)
	{
		x = x0;
		y = y0;
		z = z0;
	}

	void set_all(scalar_t a)
	{
		set(a, a, a);
	}

	vec operator -() const
	{
		return vec(-x, -y, -z);
	}

	vec &operator =(const vec &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	vec &operator *=(const scalar_t k)
	{
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	vec &operator /=(const scalar_t k)
	{
		x /= k;
		y /= k;
		z /= k;
		return *this;
	}

	vec &operator +=(const vec &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	vec &operator -=(const vec &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	vec &operator *=(const vec &v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	vec &operator /=(const vec &v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	vec operator +(vec const &v) const
	{
		return vec(x + v.x, y + v.y, z + v.z);
	}

	vec operator -(vec const &v) const
	{
		return vec(x - v.x, y - v.y, z - v.z);
	}

	vec operator *(vec const &v) const
	{
		return vec(x * v.x, y * v.y, z * v.z);
	}

	vec operator /(vec const &v) const
	{
		return vec(x / v.x, y / v.y, z / v.z);
	}

	vec operator *(scalar_t const k) const
	{
		return vec(x * k, y * k, z * k);
	}

	vec operator /(scalar_t const k) const
	{
		return vec(x / k, y / k, z / k);
	}

	bool operator <(vec const &rhs) const
	{
		return x < rhs.x && y < rhs.y && z < rhs.z;
	}

	bool operator <=(vec const &rhs) const
	{
		return x <= rhs.x && y <= rhs.y && z <= rhs.z;
	}

	bool operator >(vec const &rhs) const
	{
		return x > rhs.x && y > rhs.y && z > rhs.z;
	}

	bool operator >=(vec const &rhs) const
	{
		return x >= rhs.x && y >= rhs.y && z >= rhs.z;
	}

	bool operator ==(vec const &rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	bool operator !=(vec const &rhs) const
	{
		return x != rhs.x || y != rhs.y || z != rhs.z;
	}

	scalar_t length_sq() const
	{
		return x * x + y * y + z * z;
	}

	scalar_t length() const
	{
		return sqrt(length_sq());
	}

	void normalize()
	{
		scalar_t l = length();
		if(abs(l) > 0) *this /= l;
	}

	void negate()
	{
		x = -x;
		y = -y;
	}

	void to_axis_angle(vec &axis, scalar_t &angle) const;

	bool is_collinear(vec const &v, scalar_t epsilon = EPSILON) const
	{
		return (*this ^ v).length_sq() < epsilon;
	}

	vec perpendicular_xy() const;
	vec perpendicular_yz() const;
	vec perpendicular_xz() const;

	static vec const I, J, K, null;

	template<class Archive>
	void serialize(Archive &archive, unsigned const /*file_version*/)
	{
		archive & x & y & z;
	}
};
#pragma pack(pop)

#pragma pack(push, 1)
template<class T>
class vec<4,T> {
public:
	typedef T scalar_t;
	static size_t const ARITY = 4;
	typedef size_t mask_t;

	union {
		struct {
			scalar_t x,y,z,w;
		};

		struct {
			scalar_t r,g,b,a;
		};

		struct {
			scalar_t A,B,C,D;
		};

		scalar_t i[ARITY];
	};

	vec() {
	}

	vec(const vec &v):
		x(v.x), y(v.y), z(v.z), w(v.w)
	{
	}

	vec(scalar_t x0,scalar_t y0,scalar_t z0,scalar_t w0):
		x(x0), y(y0), z(z0), w(w0)
	{
	}

	void set(scalar_t x0,scalar_t y0,scalar_t z0,scalar_t w0) {
		x = x0;
		y = y0;
		z = z0;
		w = w0;
	}

	void set_all(scalar_t a)
	{
		set(a, a, a, a);
	}

	vec &operator =(const vec &v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	vec &operator *=(const scalar_t k)
	{
		x *= k;
		y *= k;
		z *= k;
		w *= k;
		return *this;
	}

	vec &operator /=(const scalar_t k)
	{
		x /= k;
		y /= k;
		z /= k;
		w /= k;
		return *this;
	}

	vec &operator +=(const vec &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	vec &operator -=(const vec &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	vec &operator *=(const vec &v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	vec &operator /=(const vec &v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	vec operator +(vec const &rhs) const
	{
		return vec(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	vec operator -(vec const &rhs) const
	{
		return vec(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	vec operator *(scalar_t const k) const
	{
		return vec(x * k, y * k, z * k, w * k);
	}

	vec operator /(scalar_t const k) const
	{
		return vec(x / k, y / k, z / k, w / k);
	}

	bool operator <(vec const &rhs) const
	{
		return x < rhs.x && y < rhs.y && z < rhs.z && w < rhs.w;
	}

	bool operator <=(vec const &rhs) const
	{
		return x <= rhs.x && y <= rhs.y && z <= rhs.z && w <= rhs.w;
	}

	bool operator >(vec const &rhs) const
	{
		return x > rhs.x && y > rhs.y && z > rhs.z && w > rhs.w;
	}

	bool operator >=(vec const &rhs) const
	{
		return x >= rhs.x && y >= rhs.y && z >= rhs.z && w >= rhs.w;
	}

	bool operator ==(vec const &rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
	}

	bool operator !=(vec const &rhs) const
	{
		return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
	}

	scalar_t length_sq() const
	{
		return x * x + y * y + z * z + w * w;
	}

	scalar_t length() const
	{
		return sqrt(length_sq());
	}

	void normalize()
	{
		scalar_t l=length();
		if(abs(l) > 0) *this /= l;
	}

	void negate()
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}
};
#pragma pack(pop)

#pragma pack(push, 1)
template<int N,class T>
class vec {
public:
	static size_t const ARITY=N;
	typedef T scalar_t;
	typedef int mask_t;

	scalar_t i[ARITY];

	void set_all(scalar_t a)
	{
		for (size_t n = 0; n < ARITY; n++)
			i[n] = a;
	}

	vec &operator *=(const scalar_t k)
	{
		for (size_t n = 0; n < ARITY; n++)
			i[n] *= k;
		return *this;
	}

	vec &operator /=(const scalar_t k)
	{
		for (size_t n = 0; n < ARITY; n++)
			i[n] /= k;
		return *this;
	}

    vec &operator =(const vec &v)
	{
        std::copy(v.i, v.i + ARITY, i);
        return *this;
    }

    template<class X>
    vec &operator =(const vec<ARITY, X> &v)
	{
        for (size_t n = 0; n < ARITY; n++)
            i[n] = (scalar_t) v.i[n];
        return *this;
    }

	vec &operator +=(const vec &v)
	{
		for (size_t n = 0; n < ARITY; n++)
			i[n] += v.i[n];
		return *this;
	}

	vec &operator -=(const vec &v)
	{
		for (size_t n = 0; n < ARITY; n++)
			i[n] -= v.i[n];
		return *this;
	}

	vec &operator *=(const vec &v)
	{
		for (size_t n = 0; n < ARITY; n++)
			i[n] *= v.i[n];
		return *this;
	}

	vec &operator /=(const vec &v)
	{
		for (size_t n = 0; n < ARITY; n++)
			i[n] /= v.i[n];
		return *this;
	}

	bool operator <(vec const &rhs) const
	{
		for (size_t n = 0; n < ARITY; n++)
			if (i[n] > rhs.i[n]) return false;
		return true;
	}

	bool operator >(vec const &rhs) const
	{
		for (size_t n = 0; n > ARITY; n++)
			if (i[n] < rhs.i[n]) return false;
		return true;
	}

	bool is_zero() const
	{
		for (size_t n = 0; n < ARITY; n++)
			if (abs(i[n]) < EPSILON) return true;
		return false;
	}

	scalar_t length_sq() const
	{
		scalar_t l = i[0] * i[0];
		for (size_t n = 1; n < ARITY; n++)
			l += i[n] * i[n];
		return l;
	}

	scalar_t length() const
	{
		return sqrt(length_sq());
	}

	scalar_t manhattan_length() const
	{
		scalar_t l = 0;
		for (size_t n = 1; n < ARITY; n++)
			l += abs(i[n] - i[n - 1]);
		return l;
	}

	void normalize()
	{
		scalar_t l=length();
		if(abs(l) > 0) *this /= l;
	}
};
#pragma pack(pop)

template<class L,class R> inline bool
lexicographical_less(const vec<2,L> &left,const vec<2,R> &right)
{
	if (left.x < right.x) return true;
	if (left.x > right.x) return false;

	return left.y < right.y;
}

template<class L,class R> inline bool
lexicographical_less(const vec<3,L> &left,const vec<3,R> &right)
{
	if (left.x < right.x) return true;
	if (left.x > right.x) return false;

	if (left.y < right.y) return true;
	if (left.y > right.y) return false;

	return left.z < right.z;
}

template<class L,class R> inline bool
lexicographical_less(const vec<4,L> &left,const vec<4,R> &right)
{
	if (left.x < right.x) return true;
	if (left.x > right.x) return false;

	if (left.y < right.y) return true;
	if (left.y > right.y) return false;

	if (left.z < right.z) return true;
	if (left.z > right.z) return false;

	return left.w < right.w;
}

template<int N,class L,class R> inline bool
lexicographical_greater(const vec<N,L> &left,const vec<N,R> &right)
{
	return lexicographical_less(right, left);
}

template<class T> T
dot_product(const vec<2,T> &a,const vec<2,T> &b)
{
	return a.i[0]*b.i[0] + a.i[1]*b.i[1];
}

template<class T> T
dot_product(const vec<3,T> &a,const vec<3,T> &b)
{
	return a.i[0]*b.i[0] + a.i[1]*b.i[1] + a.i[2]*b.i[2];
}

template<class T> T
dot_product(const vec<4,T> &a,const vec<4,T> &b)
{
	return a.i[0]*b.i[0] + a.i[1]*b.i[1] + a.i[2]*b.i[2] + a.i[3]*b.i[3];
}

template<int N,class T> T
dot_product(const vec<N,T> &a,const vec<N,T> &b)
{
	T sum = a.i[0] * b.i[0];
	for(int i=1; i<N; i++)
		sum += a.i[i] * b.i[i];
	return sum;
}

template<class T> void
cross_product(vec<3,T> &u,const vec<3,T> &v1,const vec<3,T> &v2)
{
	u.x = v1.y * v2.z - v1.z * v2.y;
	u.y = v1.z * v2.x - v1.x * v2.z;
	u.z = v1.x * v2.y - v1.y * v2.x;
}

template<class T> inline void
minmax(vec<2,T> &v1,vec<2,T> &v2)
{
	if(v1.x>v2.x) std::swap(v1.x,v2.x);
	if(v1.y>v2.y) std::swap(v1.y,v2.y);
}

template<class T> inline void
minmax(vec<3,T> &v1,vec<3,T> &v2)
{
	if(v1.x>v2.x) std::swap(v1.x,v2.x);
	if(v1.y>v2.y) std::swap(v1.y,v2.y);
	if(v1.z>v2.z) std::swap(v1.z,v2.z);
}

template<class T> inline void
minmax(vec<4,T> &v1,vec<4,T> &v2)
{
	if(v1.x>v2.x) std::swap(v1.x,v2.x);
	if(v1.y>v2.y) std::swap(v1.y,v2.y);
	if(v1.z>v2.z) std::swap(v1.z,v2.z);
	if(v1.w>v2.w) std::swap(v1.w,v2.w);
}

template<int N,class T> vec<N,T>
normalize(const vec<N,T> &v)
{
	vec<N,T> t(v);
	t.normalize();
	return t;
}

}

template<int N,class T> inline math::vec<N,T>
operator *(T lhs, math::vec<N,T> const &rhs)
{
	return rhs * lhs;
}

template<int N,class T> inline T
operator &(math::vec<N,T> const &lhs,math::vec<N,T> const &rhs)
{
	return math::dot_product(lhs, rhs);
}

template<class T> inline math::vec<3,T>
operator ^(const math::vec<3,T> &lhs,const math::vec<3,T> &rhs)
{
	math::vec<3,T> res;
	math::cross_product(res, lhs, rhs);
	return res;
}

template<class Ch, class ChT, class T>
std::basic_istream<Ch, ChT> &operator >>(std::basic_istream<Ch, ChT> &is, math::vec<2,T> const &v)
{
	return is >> v.x >> v.y;
}

template<class Ch, class ChT, class T>
std::basic_ostream<Ch, ChT> &operator <<(std::basic_ostream<Ch, ChT> &os, math::vec<2,T> const &v)
{
	return os << v.x << ' ' << v.y;
}

template<class Ch, class ChT, class T>
std::basic_istream<Ch, ChT> &operator >>(std::basic_istream<Ch, ChT> &is, math::vec<3,T> const &v)
{
	return is >> v.x >> v.y >> v.z;
}

template<class Ch, class ChT, class T>
std::basic_ostream<Ch, ChT> &operator <<(std::basic_ostream<Ch, ChT> &os, math::vec<3,T> const &v)
{
	return os << v.x << ' ' << v.y << ' ' << v.z;
}

template<class Ch, class ChT, class T>
std::basic_istream<Ch, ChT> &operator >>(std::basic_istream<Ch, ChT> &is, math::vec<4,T> const &v)
{
	return is >> v.x >> v.y >> v.z >> v.w;
}

template<class Ch, class ChT, class T>
std::basic_ostream<Ch, ChT> &operator <<(std::basic_ostream<Ch, ChT> &os, math::vec<4,T> const &v)
{
	return os << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
}
