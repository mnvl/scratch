
#include <util/logger.h>
#include "a_star.h"

namespace math
{

a_star::a_star(point const &start, point const &goal, scorer_t const &scorer):
	start_(start),
	goal_(goal),
	scorer_(scorer),
	failed_(false)
{
}

a_star::~a_star()
{
}

void a_star::calculate_path()
{
	start();
	run();
}

std::vector<a_star::point> a_star::build_path(bool allow_best_heuristic_point) const
{
	std::vector<point> result;
	point current_point = goal_;

	if (failed_)
	{
		if (allow_best_heuristic_point)
		{
			scalar best_cost = -1;
			point best_point;

			for (boost::unordered_map<point, point_record, point_hash>::const_iterator it = point_records_.begin();
				 it != point_records_.end(); ++it)
			{
				if (it->second.heuristic_cost < 0) continue;

				if (best_cost < 0 || it->second.heuristic_cost < best_cost)
				{
					best_cost = it->second.heuristic_cost;
					best_point = it->first;
				}
			}

			if (best_cost < 0) return result;

			current_point = best_point;
		}
		else
		{
			return result;
		}
	}

	result.push_back(current_point);

	while (current_point != start_)
	{
		current_point = point_records_.find(current_point)->second.came_from;
		result.push_back(current_point);
	}

	std::reverse(result.begin(), result.end());

	return result;
}

void a_star::add_open_point(point const &p, scalar w)
{
	open_points_.insert(p);
	weighted_open_points_.insert(std::make_pair(w, p));
}

a_star::point a_star::pop_cheapest_open_point()
{
	point p = weighted_open_points_.begin()->second;

	open_points_.erase(p);
	weighted_open_points_.erase(weighted_open_points_.begin());

	return p;
}

void a_star::update_open_point_cost(point const &p, scalar prev_w, scalar new_w)
{
	weighted_open_points_.erase(weighted_open_points_.find(std::make_pair(prev_w, p)));
	weighted_open_points_.insert(std::make_pair(new_w, p));
}

void a_star::start()
{
	point_record &pr = point_records_[start_];
	pr.best_cost = 0;
	pr.heuristic_cost = scorer_(start_);
	pr.total_cost = pr.heuristic_cost;

	if (pr.heuristic_cost < 0) return;

	add_open_point(start_, pr.total_cost);
}

void a_star::run()
{
	while (!weighted_open_points_.empty())
	{
		if (is_point_open(goal_)) return;
		process_next_point();
	}

	failed_ = true;
}

void a_star::process_next_point()
{
	point p = pop_cheapest_open_point();

	closed_points_.insert(p);

	point_record const &pr = point_records_.find(p)->second;

	process_point_neighbour(p, pr, p + point(-1, -1));
	process_point_neighbour(p, pr, p + point(-1,  0));
	process_point_neighbour(p, pr, p + point(-1,  1));
	process_point_neighbour(p, pr, p + point( 0, -1));
	process_point_neighbour(p, pr, p + point( 0,  1));
	process_point_neighbour(p, pr, p + point( 1, -1));
	process_point_neighbour(p, pr, p + point( 1,  0));
	process_point_neighbour(p, pr, p + point( 1,  1));
}

void a_star::process_point_neighbour(point const &p, point_record const &pr, point const &n)
{
	if (closed_points_.find(n) != closed_points_.end()) return;

	scalar tentative_best_cost = pr.best_cost + 1;

	point_record &nr = point_records_[n];

	if (!is_point_open(n))
	{
		nr.came_from = p;
		nr.best_cost = tentative_best_cost;
		nr.heuristic_cost = scorer_(n);

		if (nr.heuristic_cost < 0)
		{
			nr.total_cost = -nr.best_cost;
			closed_points_.insert(n);
			return;
		}

		nr.total_cost = nr.best_cost + nr.heuristic_cost;

		add_open_point(n, nr.total_cost);
	}
	else if (nr.best_cost > tentative_best_cost)
	{
		nr.came_from = p;
		nr.best_cost = tentative_best_cost;
		scalar total_cost = nr.best_cost + nr.heuristic_cost;
		update_open_point_cost(n, nr.total_cost, total_cost);
		nr.total_cost = total_cost;
	}
}

}
