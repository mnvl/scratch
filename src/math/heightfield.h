#pragma once

#include <vector>
#include <luabind/lua_include.hpp>
#include <boost/shared_ptr.hpp>
#include "vec.h"
#include "aabb.h"
#include "line.h"
#include "matrix.h"
#include "collision.h"

namespace math
{

template<class T = scalar>
class heightfield {
public:
    typedef T value_t;
	typedef vec<2, int> cell_t;

    heightfield(matrix<4,4> const &tf, size_t ncols, size_t nrows);
    ~heightfield();

	value_t local_y_at(int col, int row) const { return heights_.at(row * ncols_ + col); }
	value_t &local_y_at(int col, int row) { return heights_.at(row * ncols_ + col); }
	vec<3> local_vertex_at(int col, int row) const { return vec<3>(scalar(col), local_y_at(col, row), scalar(row)); }

	scalar y_under(vec<3> const &position, scalar default_value = 0);
	scalar max_y_in_cell(int col, int row, scalar default_value = 0) const;
	scalar max_y_in_cell(vec<3> const &position, scalar default_value = 0) const;

	size_t ncols() const { return ncols_; }
	size_t nrows() const { return nrows_; }
    matrix<4,4> const &get_local_to_world() const { return local_to_world_; }
    matrix<4,4> const &get_world_to_local() const { return world_to_local_; }
	aabb<3> const &get_local_aabb() const { return local_aabb_; }

	void set_local_to_world(math::matrix<4, 4> const &tf);
	cell_t world_position_to_cell(vec<3> const &position) const;
	vec<3> cell_to_world_position(cell_t const &cell) const;
    contact_info<3> trace(ray<3> const &r, scalar max_distance = 1000.0f) const;
    void load_from_raw_buffer(value_t const *heights);
	void resize(size_t ncols, size_t nrows);
	std::vector<cell_t> build_path(vec<3> const &from, vec<3> const &to, value_t min_value = 0, bool allow_best_heuristic_point = false,
		std::vector<cell_t> const &obstacles = std::vector<cell_t>()) const;

	template<class Archive>
	void serialize(Archive &archive, unsigned const /*file_version*/)
	{
		archive & local_to_world_ & world_to_local_ & ncols_ & nrows_ & heights_ & local_aabb_;
	}

	static void bind(lua_State *L, char const *name);

private:
    matrix<4,4> local_to_world_, world_to_local_;
    size_t ncols_, nrows_;
    std::vector<value_t> heights_;
	aabb<3> local_aabb_;

    size_t nsamples()
    {
        return nrows_ * ncols_;
    }

	void post_load();
};

void bind_heightfield(lua_State *L);

}
