#ifndef InputDebug_h
#define InputDebug_h
#include "Console.h"

class InputDebug
{
public:
	InputDebug();
	void log(SDL_Event* event);
	void render();
private:
	Console* console;
	int jaxis_values[6];
};

#endif