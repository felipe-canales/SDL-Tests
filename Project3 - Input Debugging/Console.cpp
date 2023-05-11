#include "Console.h"

const int HISTORY_MAX_SIZE = 100;

Console::Console(SDL_Renderer* ext_renderer)
{
	text_handler = new TextHandler(ext_renderer);
}

Console::~Console()
{
	delete text_handler;
}

void Console::print(std::string text)
{
	history.push_front(text);
	if (history.size() > HISTORY_MAX_SIZE)
		history.pop_back();
}

void Console::render()
{
	int cursor = 0;
	int spacing = text_handler->get_line_height();
	for (auto line : history) {
		text_handler->print(line, spacing, (++cursor * spacing));
	}
}
