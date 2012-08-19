#pragma once

#include <math/vec.h>

namespace graphic
{

struct GenericVertex {
	typedef unsigned format_type;

	static format_type const FORMAT_POSITION = 0x0001u;
	static format_type const FORMAT_TANGENT = 0x0002u;
	static format_type const FORMAT_BINORMAL = 0x0004u;
	static format_type const FORMAT_NORMAL = 0x0008u;
	static format_type const FORMAT_JOINT_INDICES = 0x0010u;
	static format_type const FORMAT_JOINT_WEIGHTS = 0x0020u;
	static format_type const FORMAT_TEXCOORDS = 0x0040u;
	static format_type const FORMAT_TEXTURE_BLEND_WEIGHTS = 0x0080u;

	inline bool operator <(GenericVertex const &rhs) const
	{
		if (math::lexicographical_less(position, rhs.position)) return true;
		if (math::lexicographical_greater(position, rhs.position)) return false;

		if (math::lexicographical_less(tangent, rhs.tangent)) return true;
		if (math::lexicographical_greater(tangent, rhs.tangent)) return false;

		if (math::lexicographical_less(binormal, rhs.binormal)) return true;
		if (math::lexicographical_greater(binormal, rhs.binormal)) return false;

		if (math::lexicographical_less(normal, rhs.normal)) return true;
		if (math::lexicographical_greater(normal, rhs.normal)) return false;

		if (math::lexicographical_less(texcoords, rhs.texcoords)) return true;
		if (math::lexicographical_greater(texcoords, rhs.texcoords)) return false;

		return math::lexicographical_less(texutre_blend_weights, rhs.texutre_blend_weights);
	}

	math::vec<3> position;
	math::vec<3> tangent;
	math::vec<3> binormal;
	math::vec<3> normal;
	math::vec<8, unsigned> joints;
	math::vec<8> weights;
	math::vec<2> texcoords;
	math::vec<4> texutre_blend_weights;
};

}

