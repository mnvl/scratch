#pragma once

#include <assert.h>
#include <iosfwd>
#include <luabind/lua_include.hpp>
#include "scalar.h"
#include "vec.h"

#if defined(minor)
#undef minor
#endif

namespace math
{

template<int N,int M,class T = scalar> class matrix;
template<class T> class matrix<3,3,T>;
template<class T> class matrix<4,4,T>;
template<class T> class quaternion;

template<class T> inline void mul(vec<2,T> &r,const vec<2,T> &v,const matrix<3,3,T> &m);
template<class T> inline void mul(vec<3,T> &r,const vec<2,T> &v,const matrix<3,3,T> &m);
template<class T> inline void mul(vec<3,T> &r,const vec<3,T> &v,const matrix<3,3,T> &m);
template<class T> inline void mul(vec<3,T> &r,const vec<3,T> &v,const matrix<4,4,T> &m);
template<class T> inline void mul(vec<4,T> &r,const vec<3,T> &v,const matrix<4,4,T> &m);
template<class T> inline void mul(vec<4,T> &r,const vec<4,T> &v,const matrix<4,4,T> &m);

template<int M,int N,int K,class T> inline void
mul(matrix<M,K,T> &result,const matrix<M,N,T> &left,const matrix<N,K,T> &right);

void bind_matrix(lua_State *L);

template<class T>
class matrix<3,3,T> {
public:
	// типы и константы
	static size_t const WIDTH=3,HEIGHT=3;
	typedef T scalar_t;
	typedef vec<WIDTH-1,scalar_t> point_t;
	typedef vec<WIDTH,scalar_t> vec_t;
	typedef quaternion<scalar_t> quaternion_t;
	typedef matrix<WIDTH,HEIGHT,scalar_t> matrix_t;

	// данные
    union {
		struct {
			float _11, _12, _13;
            float _21, _22, _23;
            float _31, _32, _33;
        };

		scalar_t ij[WIDTH][HEIGHT];
	};

	// конструкторы
	void assign(vec_t const &l1, vec_t const &l2, vec_t const &l3);
	void zero();
	void identity();
	void translation(scalar_t x,scalar_t y);
	void translation(const point_t &p);
	void translate(scalar_t x,scalar_t y);
	void translate(const point_t &p);
	void scaling(scalar_t x, scalar_t y);
	void scaling(const point_t &p);
	void scale(scalar_t x, scalar_t y);
	void scale(const point_t &p);
	void rotation(scalar_t alpha,scalar_t beta,scalar_t gamma);
	void rotation(const vec_t &v);
	void rotation(const quaternion_t &q);
	void rotation(vec_t const &axis, scalar_t sine, scalar_t cosine);
	void rotation(vec_t const &axis, scalar_t angle);
	void rotate(scalar_t alpha,scalar_t beta,scalar_t gamma);
	void rotate(const vec_t &v);
	void rotate(const quaternion_t &q);
	void rotate(vec_t const &axis, scalar_t sine, scalar_t cosine);
	void rotate(vec_t const &axis, scalar_t angle);
	void direction(const vec_t &dir);
	void direct(const vec_t &dir);

	bool world_to_axes(vec_t const &tangent, vec_t const &normal, vec_t const &binormal);
	bool axes_to_world(vec_t const &tangent, vec_t const &normal, vec_t const &binormal);

	// вычислить детерминант минора
	// r0, r1 -- индексы столбцов минора
	// c0, c1 -- индексы строк минора
	scalar_t minor(int r0,int r1,int c0,int c1) const {
		return ij[r0][c0] * ij[r1][c1] - ij[r1][c0] * ij[r0][c1];
	}

	void adjoint(matrix_t &M) const {
		M.ij[0][0] =  minor( 1, 2, 1, 2);
		M.ij[0][1] = -minor( 0, 2, 1, 2);
		M.ij[0][2] =  minor( 0, 1, 1, 2);
		M.ij[1][0] = -minor( 1, 2, 0, 2);
		M.ij[1][1] =  minor( 0, 2, 0, 2);
		M.ij[1][2] = -minor( 0, 1, 0, 2);
		M.ij[2][0] =  minor( 1, 2, 0, 1);
		M.ij[2][1] = -minor( 0, 2, 0, 1);
		M.ij[2][2] =  minor( 0, 1, 0, 1);
	}

	scalar_t determinant() const {
		return 	ij[0][0] * minor(1, 2, 1, 2) -
			ij[0][1] * minor(1, 2, 0, 2) +
			ij[0][2] * minor(1, 2, 0, 1);
	}

	scalar_t trace() const {
		return ij[0][0] + ij[1][1] + ij[2][2];
	}

	// вычислить обратную матрицу и записать ее в M
	// возвращает false если детерминант равен нулю
	bool inverse(matrix_t &M) const {
		scalar_t det = determinant();
		if(det == 0) return false;
		adjoint(M);
		M /= det;
		return true;
	}

	// инвертирует матрицу
	// возвращает false если детерминант равен нулю
	bool invert() {
		return matrix_t(*this).inverse(*this);
	}

	// записать в T транспонированную матрицу
	void transpose(matrix_t &tp) const {
        tp.ij[0][0] = ij[0][0];		tp.ij[0][1] = ij[1][0];		tp.ij[0][2] = ij[2][0];
		tp.ij[1][0] = ij[0][1];		tp.ij[1][1] = ij[1][1];		tp.ij[1][2] = ij[2][1];
		tp.ij[2][0] = ij[0][2];		tp.ij[2][1] = ij[1][2];		tp.ij[2][2] = ij[2][2];
	}

	matrix_t &operator *=(scalar_t k) {
		ij[0][0]*=k;	ij[0][1]*=k;	ij[0][2]*=k;
		ij[1][0]*=k;	ij[1][1]*=k;	ij[1][2]*=k;
		ij[2][0]*=k;	ij[2][1]*=k;	ij[2][2]*=k;
		return *this;
	}

	matrix_t &operator *=(const matrix_t &m) {
		matrix_t n=*this;
		mul(*this,n,m);
		return *this;
	}

	matrix_t &operator /=(scalar_t k) {
		return *this *= 1/k;
	}

	template<class Archive>
	void serialize(Archive &archive, unsigned const /*file_version*/)
	{
		archive & ij;
	}
};

// must be binary-compatible with D3DMATRIX, see graphic/renderer.cc
template<class T>
class matrix<4,4,T> {
public:
	// типы и константы
	static size_t const WIDTH=4,HEIGHT=4;
	typedef T scalar_t;
	typedef vec<WIDTH,scalar_t> vec_t;
	typedef vec<WIDTH-1,scalar_t> point_t;
	typedef quaternion<scalar_t> quaternion_t;
	typedef matrix<WIDTH,HEIGHT,scalar_t> matrix_t;

	// данные
	union {
		struct {
			scalar_t _11, _12, _13, _14;
            scalar_t _21, _22, _23, _24;
            scalar_t _31, _32, _33, _34;
            scalar_t _41, _42, _43, _44;
        };

		scalar_t ij[WIDTH][HEIGHT];
	};

	// конструкторы
	matrix() {
	}

	void identity();
	void zero();
	void translation(scalar_t x,scalar_t y,scalar_t z);
	void translation(const point_t &p);
	void translate(scalar_t x,scalar_t y,scalar_t z);
	void translate(const point_t &p);
	void scaling(scalar_t sx,scalar_t sy,scalar_t sz);
	void scaling(const point_t &s);
	void scale(scalar_t sx,scalar_t sy,scalar_t sz);
	void scale(const point_t &s);
	void rotation(scalar_t alpha,scalar_t beta,scalar_t gamma);
	void rotation(const point_t &v);
	void rotation(const quaternion_t &q);
	void rotation(point_t const &axis, scalar_t sine, scalar_t cosine);
	void rotation(point_t const &axis, scalar_t angle);
	void rotate(scalar_t alpha,scalar_t beta,scalar_t gamma);
	void rotate(const point_t &v);
	void rotate(const quaternion_t &q);
	void rotate(point_t const &axis, scalar_t sine, scalar_t cosine);
  	void rotate(point_t const &axis, scalar_t angle);
	void direction(const point_t &dir);
	void direct(const point_t &dir);
	void lookat(const point_t &eye,const point_t &at,const point_t &up);
	void ortho(scalar_t width,scalar_t height,scalar_t z_near,scalar_t z_far);
	void perspective(scalar_t fovy,scalar_t aspect,scalar_t z_near,scalar_t z_far);

	// вычислить детерминант минора
	// r0, r1, r2 -- индексы столбцов минора
	// c0, c1, c2 -- индексы строк минора
	scalar_t minor(int r0,int r1,int r2,int c0,int c1,int c2) const {
		return
			ij[r0][c0] * (ij[r1][c1] * ij[r2][c2] - ij[r2][c1] * ij[r1][c2]) -
			ij[r0][c1] * (ij[r1][c0] * ij[r2][c2] - ij[r2][c0] * ij[r1][c2]) +
			ij[r0][c2] * (ij[r1][c0] * ij[r2][c1] - ij[r2][c0] * ij[r1][c1]);
	}

	void adjoint(matrix_t &M) const {
		M.ij[0][0] =  minor( 1, 2, 3, 1, 2, 3);
		M.ij[0][1] = -minor( 0, 2, 3, 1, 2, 3);
		M.ij[0][2] =  minor( 0, 1, 3, 1, 2, 3);
		M.ij[0][3] = -minor( 0, 1, 2, 1, 2, 3);
		M.ij[1][0] = -minor( 1, 2, 3, 0, 2, 3);
		M.ij[1][1] =  minor( 0, 2, 3, 0, 2, 3);
		M.ij[1][2] = -minor( 0, 1, 3, 0, 2, 3);
		M.ij[1][3] =  minor( 0, 1, 2, 0, 2, 3);
		M.ij[2][0] =  minor( 1, 2, 3, 0, 1, 3);
		M.ij[2][1] = -minor( 0, 2, 3, 0, 1, 3);
		M.ij[2][2] =  minor( 0, 1, 3, 0, 1, 3);
		M.ij[2][3] = -minor( 0, 1, 2, 0, 1, 3);
		M.ij[3][0] = -minor( 1, 2, 3, 0, 1, 2);
		M.ij[3][1] =  minor( 0, 2, 3, 0, 1, 2);
		M.ij[3][2] = -minor( 0, 1, 3, 0, 1, 2);
		M.ij[3][3] =  minor( 0, 1, 2, 0, 1, 2);
	}

	scalar_t determinant() const {
		return
			ij[0][0] * minor(1, 2, 3, 1, 2, 3) -
			ij[0][1] * minor(1, 2, 3, 0, 2, 3) +
			ij[0][2] * minor(1, 2, 3, 0, 1, 3) -
			ij[0][3] * minor(1, 2, 3, 0, 1, 2);
	}

	// вычислить обратную матрицу и записать ее в M
	// возвращает false если детерминант равен нулю
	bool inverse(matrix_t &M) const {
		scalar_t det = determinant();
		if(det == 0) return false;
		adjoint(M);
		M /= det;
		return true;
	}

	// инвертирует матрицу
	// возвращает false если детерминант равен нулю
	bool invert() {
		return matrix_t(*this).inverse(*this);
	}

	// записать в T транспонированную матрицу
	void transpose(matrix_t &tp) const {
       	tp.ij[0][0] = ij[0][0];	tp.ij[0][1] = ij[1][0];	tp.ij[0][2] = ij[2][0]; tp.ij[0][3] = ij[3][0];
		tp.ij[1][0] = ij[0][1];	tp.ij[1][1] = ij[1][1];	tp.ij[1][2] = ij[2][1]; tp.ij[1][3] = ij[3][1];
		tp.ij[2][0] = ij[0][2];	tp.ij[2][1] = ij[1][2];	tp.ij[2][2] = ij[2][2]; tp.ij[2][3] = ij[3][2];
		tp.ij[3][0] = ij[0][3];	tp.ij[3][1] = ij[1][3];	tp.ij[3][2] = ij[2][3]; tp.ij[3][3] = ij[3][3];
	}

	void transpose_inplace() {
		matrix_t tp;
		tp = *this;
		tp.transpose(*this);
	}

	matrix_t &operator *=(scalar_t k) {
		ij[0][0]*=k;	ij[0][1]*=k;	ij[0][2]*=k;	ij[0][3]*=k;
		ij[1][0]*=k;	ij[1][1]*=k;	ij[1][2]*=k;	ij[1][3]*=k;
		ij[2][0]*=k;	ij[2][1]*=k;	ij[2][2]*=k;	ij[2][3]*=k;
		ij[3][0]*=k;	ij[3][1]*=k;	ij[3][2]*=k;	ij[3][3]*=k;
		return *this;
	}

	matrix_t &operator /=(scalar_t k) {
		return *this *= 1/k;
	}

	matrix_t &operator *=(const matrix_t &m) {
		matrix_t n=*this;
		mul(*this,n,m);
		return *this;
	}

	point_t projected_column(size_t column_index) const {
		return point_t(ij[column_index][0], ij[column_index][1], ij[column_index][2]);
	}

	vec_t column(size_t column_index) const {
		return vec_t(ij[column_index][0], ij[column_index][1], ij[column_index][2],
			ij[column_index][3]);
	}

	point_t projected_row(size_t row_index) const {
		return point_t(ij[0][row_index], ij[1][row_index], ij[2][row_index]);
	}

	vec_t row(size_t row_index) const {
		return vec_t(ij[0][row_index], ij[1][row_index], ij[2][row_index],
			ij[3][row_index]);
	}

	template<class Archive>
	void serialize(Archive &archive, unsigned const /*file_version*/)
	{
		archive & ij;
	}
};

template<class T> void
mul(vec<2,T> &result,const vec<2,T> &v,const matrix<3,3,T> &m) {
	vec<3,T> r;
	mul(r,v,m);
	result.x = r.x/r.z;
	result.y = r.y/r.z;
}

template<class T> void
mul(vec<3,T> &result,const vec<2,T> &v,const matrix<3,3,T> &m) {
	assert(&result != &v);

	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0];
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1];
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2];
}

template<class T> void
mul(vec<3,T> &result,const vec<3,T> &v,const matrix<3,3,T> &m) {
	assert(&result != &v);

	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0]*v.z;
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1]*v.z;
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2]*v.z;
}

template<class T> void
mul(vec<3,T> &result,const vec<3,T> &v,const matrix<4,4,T> &m) {
	assert(&result != &v);

	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0]*v.z+m.ij[3][0];
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1]*v.z+m.ij[3][1];
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2]*v.z+m.ij[3][2];
}

template<class T> void
mul(vec<4,T> &result,const vec<3,T> &v,const matrix<4,4,T> &m) {
	assert(&result != &v);

	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0]*v.z+m.ij[3][0];
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1]*v.z+m.ij[3][1];
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2]*v.z+m.ij[3][2];
	result.w=m.ij[0][3]*v.x+m.ij[1][3]*v.y+m.ij[2][3]*v.z+m.ij[3][3];
}

template<class T> void
mul(vec<4,T> &result,const vec<4,T> &v,const matrix<4,4,T> &m) {
	assert(&result != &v);

	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0]*v.z+m.ij[3][0]*v.w;
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1]*v.z+m.ij[3][1]*v.w;
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2]*v.z+m.ij[3][2]*v.w;
	result.w=m.ij[0][3]*v.x+m.ij[1][3]*v.y+m.ij[2][3]*v.z+m.ij[3][3]*v.w;
}

template<int M,int N,int K,class T> void
mul(matrix<M,K,T> &result,const matrix<M,N,T> &left,const matrix<N,K,T> &right) {
	assert(&result != &left);
	assert(&result != &right);

	int i,j,k;
	for(i=0; i<M; i++) {
		for(j=0; j<K; j++) {
			result.ij[i][j]=left.ij[i][0]*right.ij[0][j];
			for(k=1; k<N; k++) result.ij[i][j]+=left.ij[i][k]*right.ij[k][j];
		}
	}
}

template <int M, int N, class T> inline bool
equal(matrix<M, N, T> const &lhs, matrix<M, N, T> const &rhs, T epsilon = EPSILON) {
	for(size_t i = 0; i < M; i++) {
		for(size_t j = 0; j < N; j++) {
			if (abs(lhs.ij[i][j] - rhs.ij[i][j]) > epsilon) return false;
		}
	}

	return true;
}

}

template<int M,int N,class T> inline math::vec<N-1,T>
operator *(const math::vec<N-1,T> &left, const math::matrix<M,N,T> &right) {
	math::vec<N-1,T> result;
	mul(result,left,right);
	return result;
}

template<int M,int N,class T> inline math::vec<N,T>
operator *(const math::vec<N,T> &left, const math::matrix<M,N,T> &right) {
	math::vec<N,T> result;
	mul(result,left,right);
	return result;
}

template<int M,int N,int K,class T> inline math::matrix<N,K,T>
operator *(const math::matrix<M,N,T> &left,const math::matrix<N,K,T> &right) {
	math::matrix<N,K,T> result;
	mul(result,left,right);
	return result;
}

template<int M,int N,class T> inline math::vec<N,T> &
operator *=(math::vec<N,T> &left, const math::matrix<M,N,T> &right) {
	mul(left,math::vec<N,T>(left),right);
	return left;
}

