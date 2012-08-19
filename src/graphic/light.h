#pragma once

#include <math/vec.h>

namespace graphic
{

#pragma pack(push, 1)
struct DirectionalLightDescr
{
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR spec;
	D3DXVECTOR3 dir;
};
#pragma pack(pop)

}
