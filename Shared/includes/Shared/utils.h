#pragma once

#include "library.h"

struct SDL_Color;

namespace utils {
	SHARED_API double modulo(double value, double m);
	SHARED_API SDL_Color HSL2RGB(double hue, double S, double L);
}