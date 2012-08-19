
#include <luabind/luabind.hpp>
#include "quaternion.h"
#include "matrix.h"

namespace math
{

template<class T> void
matrix<3,3,T>::assign(vec_t const &l1, vec_t const &l2, vec_t const &l3)
{
	ij[0][0]=l1.x;	ij[0][1]=l1.y;	ij[0][2]=l1.z;
	ij[1][0]=l2.x;	ij[1][1]=l2.y;	ij[1][2]=l2.z;
	ij[2][0]=l3.x;	ij[2][1]=l3.z;	ij[2][2]=l3.z;
}

template<class T> void
matrix<3,3,T>::identity() {
	ij[0][0]=1;	ij[0][1]=0;	ij[0][2]=0;
	ij[1][0]=0;	ij[1][1]=1;	ij[1][2]=0;
	ij[2][0]=0;	ij[2][1]=0;	ij[2][2]=1;
}

template<class T> void
matrix<3,3,T>::zero() {
	ij[0][0]=0;	ij[0][1]=0;	ij[0][2]=0;
	ij[1][0]=0;	ij[1][1]=0;	ij[1][2]=0;
	ij[2][0]=0;	ij[2][1]=0;	ij[2][2]=0;
}

template<class T> void
matrix<3,3,T>::translation(scalar_t x,scalar_t y) {
	ij[0][0]=1;	ij[0][1]=0;	ij[0][2]=0;
	ij[1][0]=0;	ij[1][1]=1;	ij[1][2]=0;
	ij[2][0]=x;	ij[2][1]=y;	ij[2][2]=1;
}

template<class T> void
matrix<3,3,T>::translation(const point_t &p) {
	translation(p.x,p.y);
}

template<class T> void
matrix<3,3,T>::translate(scalar_t x,scalar_t y) {
	matrix_t temp;
	temp.translation(x,y);
	*this *= temp;
}

template<class T> void
matrix<3,3,T>::translate(const point_t &p) {
	translate(p.x,p.y);
}

template<class T> void
matrix<3,3,T>::scaling(scalar_t x, scalar_t y) {
	ij[0][0]=x;	ij[0][1]=0;	ij[0][2]=0;
	ij[1][0]=0;	ij[1][1]=y;	ij[1][2]=0;
	ij[2][0]=0;	ij[2][1]=0;	ij[2][2]=1;
}

template<class T> void
matrix<3,3,T>::scaling(const point_t &p) {
	scaling(p.x, p.y);
}

template<class T> void
matrix<3,3,T>::scale(scalar_t x, scalar_t y) {
	matrix_t temp;
	temp.scaling(x,y);
	*this *= temp;
}

template<class T> void
matrix<3,3,T>::scale(const point_t &p) {
	scale(p.x, p.y);
}

template<class T> void
matrix<3,3,T>::rotation(scalar_t alpha,scalar_t beta,scalar_t gamma) {
	scalar_t sa = sin(alpha),
		    ca = cos(alpha),
		    sb = sin(beta),
		    cb = cos(beta),
		    sg = sin(gamma),
		    cg = cos(gamma),
		    sa_sb = sa * sb,
		    ca_sb = ca * sb;

	ij[0][0] = cb*cg;
	ij[0][1] = cb*sg;
	ij[0][2] = -sb;

	ij[1][0] = sa_sb*cg - ca*sg;
	ij[1][1] = sa_sb*sg + ca*cg;
	ij[1][2] = sa*cb;

	ij[2][0] = ca_sb*cg + sa*sg;
	ij[2][1] = ca_sb*sg - sa*cg;
	ij[2][2] = ca*cb;
}

template<class T> void
matrix<3,3,T>::rotation(const vec_t &v) {
	rotation(v.A,v.B,v.C);
}

template<class T> void
matrix<3,3,T>::rotation(const quaternion<T> &q) {
	scalar_t wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
	scalar_t s  = 2 / q.norm();
	x2 = q.x * s;    y2 = q.y * s;    z2 = q.z * s;
	xx = q.x * x2;   xy = q.x * y2;   xz = q.x * z2;
	yy = q.y * y2;   yz = q.y * z2;   zz = q.z * z2;
	wx = q.w * x2;   wy = q.w * y2;   wz = q.w * z2;

	ij[0][0] = 1 - (yy + zz);
	ij[1][0] = xy - wz;
	ij[2][0] = xz + wy;

	ij[0][1] = xy + wz;
	ij[1][1] = 1 - (xx + zz);
	ij[2][1] = yz - wx;

	ij[0][2] = xz - wy;
	ij[1][2] = yz + wx;
	ij[2][2] = 1 - (xx + yy);
}

template<class T> void
matrix<3,3,T>::rotation(vec_t const &axis, scalar_t angle) {
	rotation(axis, sin(angle), cos(angle));
}

template<class T> void
matrix<3,3,T>::rotation(vec_t const &axis, scalar_t sine, scalar_t cosine) {
	scalar_t length = axis.length();
	scalar_t a = axis.x / length;
	scalar_t b = axis.y / length;
	scalar_t c = axis.z / length;
	scalar_t aa = a * a;
	scalar_t bb = b * b;
	scalar_t cc = c * c;
	scalar_t omcos = 1 - cosine;

	ij[0][0] = aa + (1 - aa) * cosine;
	ij[1][1] = bb + (1 - bb) * cosine;
	ij[2][2] = cc + (1 - cc) * cosine;
	ij[0][1] = a * b * omcos + c * sine;
	ij[0][2] = a * c * omcos - b * sine;
	ij[1][0] = a * b * omcos - c * sine;
	ij[1][2] = b * c * omcos + a * sine;
	ij[2][0] = a * c * omcos + b * sine;
	ij[2][1] = b * c * omcos - a * sine;
}

template<class T> void
matrix<3,3,T>::rotate(scalar_t alpha,scalar_t beta,scalar_t gamma) {
	matrix_t self = *this, temp;
	temp.rotation(alpha,beta,gamma);
	mul(*this, self, temp);
}

template<class T> void
matrix<3,3,T>::rotate(const vec_t &v) {
	rotate(v.A,v.B,v.C);
}

template<class T> void
matrix<3,3,T>::rotate(const quaternion_t &q) {
	matrix_t self = *this, temp;
	self.rotation(q);
	mul(*this, self, temp);
}

template<class T> void
matrix<3,3,T>::rotate(vec_t const &axis, scalar_t angle) {
	matrix_t self = *this, temp;
	temp.rotation(axis, angle);
	mul(*this, self, temp);
}

template<class T> void
matrix<3,3,T>::rotate(vec_t const &axis, scalar_t sine, scalar_t cosine) {
	matrix_t self = *this, temp;
	temp.rotation(axis, sine, cosine);
	mul(*this, self, temp);
}

template<class T> void
matrix<3,3,T>::direction(const vec_t &dir) {
	vec_t x_axis = dir;
	x_axis.normalize();

	vec_t y_axis;
	y_axis.set(0,1,0);

	vec_t z_axis = x_axis ^ y_axis;
	z_axis.normalize();

	y_axis = z_axis ^ x_axis;
	y_axis.normalize();

	ij[0][0] = x_axis.x;	ij[0][1] = x_axis.y;	ij[0][2] = x_axis.z;
	ij[1][0] = y_axis.x;	ij[1][1] = y_axis.y;	ij[1][2] = y_axis.z;
	ij[2][0] = z_axis.x;	ij[2][1] = z_axis.y;	ij[2][2] = z_axis.z;
}

template<class T> void
matrix<3,3,T>::direct(const vec_t &dir) {
	matrix_t self = *this, temp;
	temp.direction(dir);
	mul(*this, self, temp);
}

template<class T> bool
matrix<3,3,T>::world_to_axes(vec_t const &tangent, vec_t const &normal, vec_t const &binormal)
{
	scalar_t tt = tangent & tangent;
	scalar_t nt = normal & tangent;
	scalar_t bt = binormal & tangent;
	scalar_t nn = normal & normal;
	scalar_t bn = binormal & normal;
	scalar_t bb = binormal & binormal;

	matrix A;
	A.ij[0][0] = tt; A.ij[0][1] = nt; A.ij[0][2] = bt;
	A.ij[1][0] = nt; A.ij[1][1] = nn; A.ij[1][2] = bn;
	A.ij[2][0] = bt; A.ij[2][1] = bn; A.ij[2][2] = bb;

	matrix B;
	if (!A.inverse(B)) return false;

	matrix C;
	C.ij[0][0] = tangent.x; C.ij[0][1] = normal.x; C.ij[0][2] = binormal.x;
	C.ij[1][0] = tangent.y; C.ij[1][1] = normal.y; C.ij[1][2] = binormal.y;
	C.ij[2][0] = tangent.z; C.ij[2][1] = normal.z; C.ij[2][2] = binormal.z;

	*this = C * B;

	return true;
}

template<class T> bool
matrix<3,3,T>::axes_to_world(vec_t const &tangent, vec_t const &normal, vec_t const &binormal)
{
	matrix world_to_axes;
	world_to_axes.world_to_axes(tangent, normal, binormal);
	return world_to_axes.inverse(*this);
}

template<class T> void
matrix<4,4,T>::identity() {
	ij[0][0]=1;	ij[0][1]=0;	ij[0][2]=0;	ij[0][3]=0;
	ij[1][0]=0;	ij[1][1]=1;	ij[1][2]=0;	ij[1][3]=0;
	ij[2][0]=0;	ij[2][1]=0;	ij[2][2]=1;	ij[2][3]=0;
	ij[3][0]=0;	ij[3][1]=0;	ij[3][2]=0;	ij[3][3]=1;
}

template<class T> void
matrix<4,4,T>::zero() {
	ij[0][0]=0;	ij[0][1]=0;	ij[0][2]=0;	ij[0][3]=0;
	ij[1][0]=0;	ij[1][1]=0;	ij[1][2]=0;	ij[1][3]=0;
	ij[2][0]=0;	ij[2][1]=0;	ij[2][2]=0;	ij[2][3]=0;
	ij[3][0]=0;	ij[3][1]=0;	ij[3][2]=0;	ij[3][3]=0;
}

template<class T> void
matrix<4,4,T>::translation(scalar_t x,scalar_t y,scalar_t z) {
	ij[0][0]=1;		ij[0][1]=0;		ij[0][2]=0;		ij[0][3]=0;
	ij[1][0]=0;		ij[1][1]=1;		ij[1][2]=0;		ij[1][3]=0;
	ij[2][0]=0;		ij[2][1]=0;		ij[2][2]=1;		ij[2][3]=0;
	ij[3][0]=x;		ij[3][1]=y;		ij[3][2]=z;		ij[3][3]=1;
}

template<class T> void
matrix<4,4,T>::translation(const point_t &p) {
	translation(p.x,p.y,p.z);
}

template<class T> void
matrix<4,4,T>::translate(scalar_t x,scalar_t y,scalar_t z) {
	matrix_t m;
	m.translation(x,y,z);
	*this *= m;
}

template<class T> void
matrix<4,4,T>::translate(const point_t &p) {
	translate(p.x,p.y,p.z);
}

template<class T> void
matrix<4,4,T>::scaling(scalar_t sx,scalar_t sy,scalar_t sz) {
	ij[0][0]=sx;	ij[0][1]=0;	ij[0][2]=0;	ij[0][3]=0;
	ij[1][0]=0;	ij[1][1]=sy;	ij[1][2]=0;	ij[1][3]=0;
	ij[2][0]=0;	ij[2][1]=0;	ij[2][2]=sz;	ij[2][3]=0;
	ij[3][0]=0;	ij[3][1]=0;	ij[3][2]=0;	ij[3][3]=1;
}

template<class T> void
matrix<4,4,T>::scaling(const point_t &s) {
	scaling(s.x,s.y,s.z);
}

template<class T> void
matrix<4,4,T>::scale(scalar_t sx,scalar_t sy,scalar_t sz) {
	matrix_t m;
	m.scaling(sx,sy,sz);
	*this *= m;
}

template<class T> void
matrix<4,4,T>::scale(const point_t &s) {
	scale(s.x,s.y,s.z);
}

template<class T> void
matrix<4,4,T>::rotation(scalar_t alpha,scalar_t beta,scalar_t gamma) {
	scalar_t sa = sin(alpha),
		    ca = cos(alpha),
		    sb = sin(beta),
		    cb = cos(beta),
		    sg = sin(gamma),
		    cg = cos(gamma),
		    sa_sb = sa * sb,
		    ca_sb = ca * sb;

	ij[0][0] = cb*cg;
	ij[0][1] = cb*sg;
	ij[0][2] = -sb;
	ij[0][3] = 0;

	ij[1][0] = sa_sb*cg - ca*sg;
	ij[1][1] = sa_sb*sg + ca*cg;
	ij[1][2] = sa*cb;
	ij[1][3] = 0;

	ij[2][0] = ca_sb*cg + sa*sg;
	ij[2][1] = ca_sb*sg - sa*cg;
	ij[2][2] = ca*cb;
	ij[2][3] = 0;

	ij[3][0] = 0;
	ij[3][1] = 0;
	ij[3][2] = 0;
	ij[3][3] = 1;
}

template<class T> void
matrix<4,4,T>::rotation(const point_t &v) {

	rotation(v.x, v.y, v.z);

}

template<class T> void
matrix<4,4,T>::rotation(const quaternion_t &q) {
	scalar_t wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
	scalar_t s  = 2 / q.norm();

	x2 = q.x * s;    y2 = q.y * s;    z2 = q.z * s;
	xx = q.x * x2;   xy = q.x * y2;   xz = q.x * z2;
	yy = q.y * y2;   yz = q.y * z2;   zz = q.z * z2;
	wx = q.w * x2;   wy = q.w * y2;   wz = q.w * z2;

	ij[0][0] = 1 - (yy + zz);
	ij[1][0] = xy - wz;
	ij[2][0] = xz + wy;
	ij[3][0] = 0;


	ij[0][1] = xy + wz;
	ij[1][1] = 1 - (xx + zz);
	ij[2][1] = yz - wx;
	ij[3][1] = 0;

	ij[0][2] = xz - wy;
	ij[1][2] = yz + wx;
	ij[2][2] = 1 - (xx + yy);
	ij[3][2] = 0;


	ij[0][3] = 0;
	ij[1][3] = 0;
	ij[2][3] = 0;
	ij[3][3] = 1;
}

template<class T> void
matrix<4,4,T>::rotation(const point_t &axis, scalar_t sine, scalar_t cosine) {
	scalar_t length = axis.length();
	scalar_t a = axis.x / length;
	scalar_t b = axis.y / length;
	scalar_t c = axis.z / length;
	scalar_t aa = a * a;
	scalar_t bb = b * b;
	scalar_t cc = c * c;
	scalar_t omcos = 1 - cosine;

	ij[0][0] = aa + (1 - aa) * cosine;
	ij[1][1] = bb + (1 - bb) * cosine;
	ij[2][2] = cc + (1 - cc) * cosine;
	ij[0][1] = a * b * omcos + c * sine;
	ij[0][2] = a * c * omcos - b * sine;
	ij[1][0] = a * b * omcos - c * sine;
	ij[1][2] = b * c * omcos + a * sine;
	ij[2][0] = a * c * omcos + b * sine;
	ij[2][1] = b * c * omcos - a * sine;
	ij[0][3] = ij[1][3] = ij[2][3] = ij[3][0] = ij[3][1] = ij[3][2] = 0;
	ij[3][3] = 1;
}

template<class T> void
matrix<4,4,T>::rotation(const point_t &axis, scalar_t angle) {
	rotation(axis, sin(angle), cos(angle));
}

template<class T> void
matrix<4,4,T>::rotate(scalar_t alpha,scalar_t beta,scalar_t gamma) {
	matrix_t self = *this, temp;
	temp.rotation(alpha,beta,gamma);
	mul(*this,self,temp);
}

template<class T> void
matrix<4,4,T>::rotate(const point_t &v) {
	rotate(v.A,v.B,v.C);
}

template<class T> void
matrix<4,4,T>::rotate(const quaternion_t &q) {
	matrix_t self = *this, temp;
	temp.rotation(q);
	mul(*this,self,temp);
}

template<class T> void
matrix<4,4,T>::rotate(point_t const &axis, scalar_t sine, scalar_t cosine) {
	matrix_t self = *this, temp;
	temp.rotation(axis, sine, cosine);
	mul(*this, self, temp);
}

template<class T> void
matrix<4,4,T>::rotate(point_t const &axis, scalar_t angle) {
	matrix_t self = *this, temp;
	temp.rotation(axis, angle);
	mul(*this, self, temp);
}

template<class T> void
matrix<4,4,T>::direction(const point_t &dir) {
	point_t x_axis = dir;
	x_axis.normalize();

	point_t y_axis;
	y_axis.set(0,1,0);

	point_t z_axis = x_axis ^ y_axis;
	z_axis.normalize();

	y_axis = z_axis ^ x_axis;
	y_axis.normalize();

	ij[0][0] = x_axis.x;	ij[0][1] = x_axis.y;	ij[0][2] = x_axis.z;	ij[0][3] = 0;
	ij[1][0] = y_axis.x;	ij[1][1] = y_axis.y;	ij[1][2] = y_axis.z;	ij[1][3] = 0;
	ij[2][0] = z_axis.x;	ij[2][1] = z_axis.y;	ij[2][2] = z_axis.z;	ij[2][3] = 0;
	ij[3][0] = 0;		    ij[3][1] = 0;		    ij[3][2] = 0;		    ij[3][3] = 1;
}

template<class T> void
matrix<4,4,T>::direct(const point_t &dir) {
	matrix_t self = *this, temp;
	temp.direction(dir);
	mul(*this, self, temp);
}

template<class T> void
matrix<4,4,T>::lookat(const point_t &eye,const point_t &at,const point_t &up)
{
	point_t x_axis,y_axis,z_axis;

	(z_axis=at-eye).normalize();
	(x_axis=up^z_axis).normalize();
	y_axis=z_axis^x_axis;

	ij[0][0]=x_axis.x;	ij[0][1]=y_axis.x;	ij[0][2]=z_axis.x;	ij[0][3]=0;
	ij[1][0]=x_axis.y;	ij[1][1]=y_axis.y;	ij[1][2]=z_axis.y;	ij[1][3]=0;
	ij[2][0]=x_axis.z;	ij[2][1]=y_axis.z;	ij[2][2]=z_axis.z;	ij[2][3]=0;

	ij[3][0]=-(x_axis&eye);
	ij[3][1]=-(y_axis&eye);
	ij[3][2]=-(z_axis&eye);
	ij[3][3]=1;
}

template<class T> void
matrix<4,4,T>::ortho(scalar_t width,scalar_t height,scalar_t z_near,scalar_t z_far)
{
	ij[0][0] = 2/width;
	ij[0][1] = 0;
	ij[0][2] = 0;
	ij[0][3] = 0;

	ij[1][0] = 0;
	ij[1][1] = 2/height;
	ij[1][2] = 0;
	ij[1][3] = 0;

	ij[2][0] = 0;
	ij[2][1] = 0;
	ij[2][2] = 1 / (z_far - z_near);
	ij[2][3] = 0;

	ij[3][0] = 0;
	ij[3][1] = 0;
	ij[3][2] = z_near / (z_near - z_far);
	ij[3][3] = 1;
}

template<class T> void
matrix<4,4,T>::perspective(scalar_t fovy,scalar_t aspect,scalar_t z_near,scalar_t z_far)
{
	scalar_t h = 1 / tan(fovy/2);
	scalar_t w = h / aspect;

	ij[0][0] = w;
	ij[0][1] = 0;
	ij[0][2] = 0;
	ij[0][3] = 0;

	ij[1][0] = 0;
	ij[1][1] = h;
	ij[1][2] = 0;
	ij[1][3] = 0;

	ij[2][0] = 0;
	ij[2][1] = 0;
	ij[2][2] = z_far/(z_far-z_near);
	ij[2][3] = 1;

	ij[3][0] = 0;
	ij[3][1] = 0;
	ij[3][2] = -z_near*z_far/(z_far-z_near);
	ij[3][3] = 0;
}

template class matrix<3,3>;
template class matrix<4,4>;

void bind_matrix(lua_State *L)
{
    using namespace luabind;

    module(L, "math")
    [
        class_<matrix<3,3> >("matrix3x3")
        .def(constructor<>())
		.def(constructor<matrix<3,3> const &>())
        .def("rotation", (void (matrix<3,3>::*)(vec<3> const &)) &matrix<3,3>::rotation)
        .def("rotate", (void (matrix<3,3>::*)(vec<3> const &)) &matrix<3,3>::rotate)
		.def("assign", &matrix<3,3>::operator =),

        class_<matrix<4,4> >("matrix4x4")
        .def(constructor<>())
		.def(constructor<matrix<4,4> const &>())
        .def("identity", &matrix<4,4>::identity)
        .def("translation", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::translation)
        .def("translate", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::translate)
        .def("rotation", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::rotation)
        .def("rotate", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::rotate)
        .def("scaling", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::scaling)
        .def("scale", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::scale)
        .def("direction", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::direction)
        .def("direct", (void (matrix<4,4>::*)(vec<3> const &)) &matrix<4,4>::direct)
		.def("assign", &matrix<4,4>::operator =),

		def("mul", (void (*)(vec<3> &, vec<3> const &, matrix<3,3> const &)) mul),
		def("mul", (void (*)(vec<3> &, vec<3> const &, matrix<4,4> const &)) mul),
		def("mul", (void (*)(vec<4> &, vec<4> const &, matrix<4,4> const &)) mul),
		def("mul", (void (*)(matrix<4,4> &, matrix<4,4> const &, matrix<4,4> const &)) mul)
    ];
}

}
