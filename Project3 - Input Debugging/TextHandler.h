#include "Game.h"
#include <string>

class TextHandler {
public:
	TextHandler(SDL_Renderer* ext_renderer);
	~TextHandler();
	void print(const char* text, int x, int y);
	void print(std::string text, int x, int y);
	int get_line_height();
private:
	SDL_Texture* font;
	SDL_Renderer* renderer;
	void get_rect_of_char(char c, SDL_Rect* dest);
};