#pragma once

#include <math/scalar.h>
#include <math/vec.h>
#include <math/ray.h>
#include <math/line.h>
#include <math/plane.h>
#include <math/aabb.h>
#include <math/sphere.h>
#include <math/matrix.h>

namespace math {

template<int N,class T=scalar> class triangle;

template<class T>
class triangle<3,T> {
public:
    typedef T scalar_t;
    static size_t const ARITY = 3;
    typedef vec<ARITY,scalar_t> vec_t;
    typedef ray<ARITY,scalar_t> ray_t;
    typedef line<ARITY,scalar_t> line_t;
    typedef plane<scalar_t> plane_t;
    typedef aabb<ARITY,scalar_t> aabb_t;
    typedef sphere<ARITY, scalar_t> sphere_t;
    typedef matrix<ARITY+1,ARITY+1,scalar_t> matrix_t;
    typedef triangle<ARITY,scalar_t> triangle_t;

    vec_t A, B, C;

    triangle() {
    }

    triangle(const vec_t &a,const vec_t &b,const vec_t &c):
        A(a),
        B(b),
        C(c)
    {
    }

    void construct(const vec_t &a,const vec_t &b,const vec_t &c) {
        A=a;
        B=b;
        C=c;
    }

    // Graphic Gems III, ed. David Kirk. V.1 Triangles Revisited, pp. 215-218
    // the normal of the triangle
    vec_t get_normal() const {
        return normalize((B-A)^(C-B));
    }
	
    // the AABB of the triangle
    void get_aabb(const aabb_t &locus) const {
        locus.lo = locus.hi = A;
        locus.extend(B);
        locus.extend(C);
    }
	
    // the plane of the triangle
    plane_t get_plane() const {
        return plane_t(A,get_normal());
    }
	
    // the perimeter of the triangle
    scalar_t perimeter() const {
        return (B-A).length()+(C-B).length()+(A-C).length();
    }
	
    // the area of the triangle
    scalar_t area() const {
        return scalar_t(0.5)*((A^B) + (B^C) + (C^A)).length();
    }
	
    // the centre of gravity, i.e. the concurrence of the medians
    vec_t cog() const {
        return (A + B + C) / scalar_t(3);
    }
	
    // the orthocenter, i.e. the concurrence of the altitudes
    vec_t orthocenter() const {
        vec_t a = B - C;
        vec_t b = C - A;
        vec_t c = A - B;
	
        scalar_t d1 = -(b & c);
        scalar_t d2 = -(c & a);
        scalar_t d3 = -(a & b);
	
        scalar_t e1 = d2 * d3;
        scalar_t e2 = d3 * d1;
        scalar_t e3 = d1 * d2;
	
        return (e1 * A + e2 * B + e3 * C) / (e1 + e2 + e3);
    }
	
    // the circumcenter, i.e. the concurrence of the perpendicular
    // bisectors to the sides
    vec_t circumcenter() const {
        return scalar_t(0.5)*(scalar_t(3)*cog() - orthocenter());
    }
	
    // the radius of the circumcircle, i.e. the circle that passes
    // through the three vertices
    scalar_t circumcircle_radius() const {
        vec_t a = B - C;
        vec_t b = C - A;
        vec_t c = A - B;
	
        scalar_t d1 = -(b & c);
        scalar_t d2 = -(c & a);
        scalar_t d3 = -(a & b);
	
        scalar_t e1 = d2 * d3;
        scalar_t e2 = d3 * d1;
        scalar_t e3 = d1 * d2;
	
        scalar_t e = e1 + e2 + e3;
	
        return scalar_t(0.5)*sqrt((a.length_sq()+b.length_sq()+c.length_sq())/e);
    }
	
    sphere_t get_circumcircle() const {
        return sphere_t(circumcenter(), circumcircle_radius());
    }
	
    // the incenter, i.e. the concurrence of the angle bisectors
    vec_t incenter() const {
        vec_t a = B - C;
        vec_t b = C - A;
        vec_t c = A - B;
	
        scalar_t ab = a.length();
        scalar_t bc = b.length();
        scalar_t ca = c.length();
	
        return (ab*A+bc*B+ca*C)/(ab+bc+ca);
    }
	
    // the radius of the incircle, i.e. the circle inscribed in the
    // triangle, tangent to the three sides
    scalar_t incircle_radius() const {
        vec_t a = B - C;
        vec_t b = C - A;
        vec_t c = A - B;
	
        scalar_t ab=a.length();
        scalar_t bc=b.length();
        scalar_t ca=c.length();
	
        return area()/(ab+bc+ca);
    }
	
    sphere_t get_incircle() const {
        return sphere_t(incenter(), incircle_radius());
    }
	
    // the excenters and the radii of the excircles, i.e. the circles
    // tangent to one side of the triangle and to the extensions of the
    // other two sides
    vec_t excenter_A() const {
        vec_t a = B - C;
        vec_t b = C - A;
        vec_t c = A - B;
	
        scalar_t ab=a.length();
        scalar_t bc=b.length();
        scalar_t ca=c.length();
	
        return ((ab+bc+ca)*incenter()-ab*A)/(bc+ca);
    }
	
    scalar_t excircle_A_radius() const {
        vec_t b = C - A;
        vec_t c = A - B;
	
        scalar_t bc=b.length();
        scalar_t ca=c.length();
	
        return area()/(bc+ca);
    }
	
    vec_t excenter_B() const {
        vec_t a = B - C;
        vec_t b = C - A;
        vec_t c = A - B;
	
        scalar_t ab=a.length();
        scalar_t bc=b.length();
        scalar_t ca=c.length();
	
        return ((ab+bc+ca)*incenter()-bc*B)/(ab+ca);
    }
	
    scalar_t excircle_B_radius() const {
        vec_t a = B - C;
        vec_t c = A - B;
	
        scalar_t ab=a.length();
        scalar_t ca=c.length();
	
        return area()/(ab+ca);
    }
	
    vec_t excenter_C() const {
        vec_t a = B - C;
        vec_t b = C - A;
        vec_t c = A - B;
	
        scalar_t ab=a.length();
        scalar_t bc=b.length();
        scalar_t ca=c.length();
	
        return ((ab+bc+ca)*incenter()-ca*C)/(ab+bc);
    }
	
    scalar_t excircle_C_radius() const {
        vec_t a = B - C;
        vec_t b = C - A;
			
        scalar_t ab=a.length();
        scalar_t bc=b.length();
	
        return area()/(ab+bc);
    }
	
    // the centre and the radius of the Feuerbach circle (nine-point
    // circle), i.e. the circle that passes through the three feet of
    // the altitudes and the middle points of the sides
    vec_t feuerbach_center() const {
        return scalar_t(0.5)*(circumcenter()+orthocenter());
    }
	
    scalar_t feuerbach_radius() const {
        return scalar_t(0.5)*circumcircle_radius();
    }

    // transform triangle by matrix
    void transform(const matrix_t &tf) {
        mul(A, vec_t(A), tf);
        mul(B, vec_t(B), tf);
        mul(C, vec_t(C), tf);
    }

    bool contains_on_plane(const vec_t &v) const {
		// http://www.blackpawn.com/texts/pointinpoly/default.html
		return ((((B - A) ^ (v - A)) & ((B - A) ^ (C - A))) >= 0)
			&& ((((C - B) ^ (v - B)) & ((C - B) ^ (A - B))) >= 0)
			&& ((((A - C) ^ (v - C)) & ((A - C) ^ (B - C))) >= 0);
    }

	bool contains(vec_t const &v) const {
		return get_plane().contains(v) && contains_on_plane(v);
	}

    bool trace(const ray_t &r,scalar_t &t,vec_t &v,scalar_t t_min = 0, scalar_t t_max = 1) const {
        return get_plane().trace(r,t,v,t_min,t_max) && contains_on_plane(v);
    }

    bool trace(const ray_t &r,scalar_t t_min = 0,scalar_t t_max = 1) const {
        scalar_t t;
        vec_t v;
        return trace(r,t,v,t_min,t_max);
    }

    bool test_intersection(const triangle_t &t) const {
        line_t ln;
        return query_intersection(t, ln);
    }

    bool query_intersection(const triangle_t &t,line_t &ln) const {
        ray_t r;
        if(!get_plane().query_intersection(t.get_plane(),r)) return false;

        vec_t v[2];
        size_t v_size = 0;
        line_t a(A,B), b(B,C), c(C,A);
	
        if(a.query_intersection(r,v[v_size])) v_size++;
        if(b.query_intersection(r,v[v_size])) v_size++;
        if(v_size<2 && c.query_intersection(r,v[v_size])) v_size++; 
	
        if(v_size<2) return false;
	
        ln.construct(v[0],v[1]);
        return true;
    }
	
    // найти время через которое столкнутся два движущихся треугольника
    // и точку столкновения
    // tri1 -- данный треугольник
    // vel0 -- скорость настоящего треугольника
    // vel1 -- скорость треугольника tri1
    // t -- время через которое треугольники столкнутся
    // cp -- точка столкновения
    // cn -- нормаль столкновения
    // t_min, t_max -- минимальное и максимальное время расчета
    bool collide(const triangle_t &tri1,const vec_t &vel0,const vec_t &vel1,
        scalar_t &t,vec_t &cp,vec_t &cn,scalar_t t_min = 0, scalar t_max = 1) const
    {
        vec_t vel = vel1 - vel0, n0 = get_normal(), n1 = tri1.get_normal(), cp0;
        scalar_t a_n0 = -vel & n0, a_n1 = vel & n1, t0;
	
        t = MAX_SCALAR;
	
        if(abs(a_n1) > 0) {
            t0 = ((A - tri1.A) & n1) / a_n1;
            cp0 = A - vel * t0;
            if(t_min <= t0 && t0 <= t_max && tri1.contains(cp0)) {
                t = t0;
                cp = A + vel0 * t;
                cn = n1;
            }
	
            t0 = ((B - tri1.A) & n1) / a_n1;
            cp0 = B - vel * t0;
            if(t_min <= t0 && t0 <= t_max && t0 < t && tri1.contains(cp0)) {
                t = t0;
                cp = B + vel0 * t;
                cn = n1;
            }
	
            t0 = ((C - tri1.A) & n1) / a_n1;
            cp0 = C - vel * t0;
            if(t_min <= t0 && t0 <= t_max && t0 < t && tri1.contains(cp0)) {
                t = t0;
                cp = C + vel0 * t;
                cn = n1;
            }
        }
	
        if(abs(a_n0) > 0) {
            t0 = ((tri1.A - A) & n0) / a_n0;
            cp0 = tri1.A + vel * t0;
            if(t_min <= t0 && t0 <= t_max && t0 < t && contains(cp0)) {
                t = t0;
                cp = tri1.A + vel0 * t;
                cn = n0;
            }
	
            t0 = ((tri1.B - A) & n0) / a_n0;
            cp0 = tri1.B + vel * t0;
            if(t_min <= t0 && t0 <= t_max && t0 < t && contains(cp0)) {
                t = t0;
                cp = tri1.B + vel0 * t;
                cn = n0;
            }
	
            t0 = ((tri1.C - A) & n0) / a_n0;
            cp0 = tri1.C + vel * t0;
            if(t_min <= t0 && t0 <= t_max && t0 < t && contains(cp0)) {
                t = t0;
                cp = tri1.C + vel0 * t;
                cn = n0;
            }
        }

        return t != MAX_SCALAR;
    }
};

}

