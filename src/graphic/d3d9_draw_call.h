#pragma once

#include "draw_call.h"

namespace graphic
{

class D3D9DrawCall: public DrawCall {
public:
	D3D9DrawCall(D3D9RendererPtr const &renderer);
	virtual ~D3D9DrawCall();

	virtual void execute();

private:
	D3D9RendererPtr renderer_;
};

}
