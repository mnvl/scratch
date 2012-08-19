#pragma once

#include "material.h"

namespace graphic
{

class D3D9Renderer;

class D3D9Material: public Material {
	friend class D3D9Renderer;

public:
	virtual ~D3D9Material();

	virtual void select();

private:
	D3D9RendererPtr renderer_;

	D3D9Material(D3D9RendererPtr const &renderer_ptr = D3D9RendererPtr());
};

}
