#pragma once

#include <set>
#include <vector>
#include <boost/function.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include "vec.h"

namespace math
{

class a_star {
public:
	typedef vec<2, int> point;
	typedef boost::function<scalar (point const &)> scorer_t;

	a_star(point const &start, point const &goal, scorer_t const &scorer);
	~a_star();

	void calculate_path();
	bool have_failed() const { return failed_; }
	std::vector<point> build_path(bool allow_best_heuristic_point = false) const;

private:
	point const start_, goal_;
	scorer_t const scorer_;
	bool failed_;

	struct point_record
	{
		point came_from;
		scalar best_cost;
		scalar heuristic_cost;
		scalar total_cost;
	};

	struct point_hash { size_t operator ()(point const &p) const { return size_t(p.x) * 1011 + size_t(p.y); } };
	struct point_equal { bool operator ()(point const &p1, point const &p2) const { return p1 == p2; } };

	struct weighted_point_less {
		bool operator ()(std::pair<scalar, point> const &l, std::pair<scalar, point> const &r) const {
			if (l.first < r.first) return true;
			if (l.first > r.first) return false;

			return lexicographical_less(l.second, r.second);
		}
	};

	boost::unordered_set<point, point_hash, point_equal> closed_points_;
	boost::unordered_set<point, point_hash, point_equal> open_points_;
	std::multiset< std::pair<scalar, point>, weighted_point_less> weighted_open_points_;
	boost::unordered_map<point, point_record, point_hash, point_equal> point_records_;

	void add_open_point(point const &p, scalar w);
	point pop_cheapest_open_point();
	bool is_point_open(point const &p) const { return open_points_.find(p) != open_points_.end(); }
	void update_open_point_cost(point const &p, scalar prev_w, scalar new_w);

	void start();
	void run();
	void process_next_point();
	void process_point_neighbour(point const &p, point_record const &pr, point const &n);
};

}

