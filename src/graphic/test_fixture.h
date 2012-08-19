#pragma once

#include <SDL.h>

namespace graphic
{

struct TestFixture {
	TestFixture() {
		SDL_Init(0);
		SDL_SetVideoMode(400, 300, 0, 0);
	}

	~TestFixture() {
		SDL_Quit();
	}

	static void process_events() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
		}
	}
};

}
