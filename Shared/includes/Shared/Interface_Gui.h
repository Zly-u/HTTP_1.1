#pragma once

#include "library.h"

class SHARED_API Interface_Gui {
public:
	virtual void Init(struct SDL_Renderer* renderer) = 0;
	virtual void SetupDocking() = 0;
	virtual void Draw() = 0;
	virtual void Render() = 0;
	virtual void CleanUp() = 0;

	virtual const struct ImColor& GetClearColor() const = 0;
};
