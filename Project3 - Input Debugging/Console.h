#include "TextHandler.h"
#include <list>

class Console {
public:
	Console(SDL_Renderer* ext_renderer);
	~Console();
	void print(std::string text);
	void render();
private:
	TextHandler* text_handler;
	std::list<std::string> history;
};