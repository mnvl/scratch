#pragma once

#include "uber_draw_call.h"

namespace graphic
{

class D3D9UberDrawCall: public UberDrawCall {
public:
	D3D9UberDrawCall(D3D9RendererPtr const &renderer);
	virtual ~D3D9UberDrawCall();

	virtual void execute();

private:
	D3D9RendererPtr const renderer_;
};

}
