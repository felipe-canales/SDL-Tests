#include "cmath"
#include "Game.h"
#include "Console.h"

void HSV2RGB(int h, float s, float v, int& r, int& g, int& b);
SDL_Texture* icon;
Console* console;

Game::Game()
{
	delay = 100;
	current_color = 120;
	is_running = false;
}

Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Error to initialize" << std::endl;
		is_running = false;
		return;
	}
	std::cout << "Initialized" << std::endl;
	window = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cout << "Error to create window" << std::endl;
		is_running = false;
		return;
	}
	std::cout << "Window created" << std::endl;
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!window) {
		std::cout << "Error to create renderer" << std::endl;
		is_running = false;
		return;
	}
	std::cout << "Renderer created" << std::endl;
	is_running = true;

	icon = IMG_LoadTexture(renderer, "assets/touhou_suika14.png");

	console = new Console(renderer);

	console->print("START");
	
	SDL_JoystickEventState(SDL_ENABLE);
	SDL_JoystickOpen(0);
}

void Game::handle_events()
{
	SDL_Event event;

	int mask = 0xFFF0;
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_LEFT and delay > 10)
			{
				delay *= 0.9;
				std::cout << "++ speed " << delay << std::endl;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT and delay < 500)
			{
				delay *= 1.12;
				std::cout << "-- speed" << delay << std::endl;
			}
			console->print("KEYBOARD: Key down");
			break;
		case SDL_KEYUP:
			console->print("KEYBOARD: Key up");
		default:
			int offset = event.type - (event.type & mask);
			if ((event.type & mask) == 0x400) { // Mouse
				if (offset == 1)
					console->print("MOUSE: Button down");
				else if (offset == 2)
					console->print("MOUSE: Button up");
				break;
			}
			if ((event.type & mask) == 0x600) { // Joystick
				if (offset == 0 && (event.jaxis.value < -3200) || (event.jaxis.value > 3200))
					console->print("JOYSTICK: Axis motion");
				else if (offset == 1)
					console->print("JOYSTICK: Ball motion");
				else if (offset == 2)
					console->print("JOYSTICK: Hat motion");
				else if (offset == 3)
					console->print("JOYSTICK: Button down");
				else if (offset == 4)
					console->print("JOYSTICK: Button up");
				break;
			}
			if ((event.type & mask) == 0x650) { // Controller
				if (offset == 0)
					console->print("CONTROLLER: Axis motion");
				else if (offset == 1)
					console->print("CONTROLLER: Button down");
				else if (offset == 2)
					console->print("CONTROLLER: Button up");
				else if (offset == 6)
					console->print("CONTROLLER: Touchpad down");
				else if (offset == 7)
					console->print("CONTROLLER: Touchpad motion");
				else if (offset == 8)
					console->print("CONTROLLER: Touchpad up");
				else if (offset == 9)
					console->print("CONTROLLER: Sensor update");
				break;
			}
			if ((event.type & mask) == 0x700) { // Touch
				if (offset == 0)
					console->print("TOUCH: Finger down");
				else if (offset == 1)
					console->print("TOUCH: Finger up");
				else if (offset == 2)
					console->print("TOUCH: Finger motion");
				break;
			}
		}
	}
}

void Game::update()
{
	SDL_Delay(delay);
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	
	int r, g, b;
	int paint = current_color;
	SDL_Rect rect{ 0, 0, 960, 2 };
	SDL_Rect icon_dest{ 950 - 128, 10, 128, 128 };
	for (int i = 0; i < 544; i += 2) {
		// paint half with max value
		HSV2RGB(paint, 1, 1, r, g, b);
		SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
		rect = { 480, i, 480, 2 };
		SDL_RenderDrawRect(renderer, &rect);

		paint = (paint + 1) % 360;
	}

	SDL_RenderCopy(renderer, icon, NULL, &icon_dest);

	console->render();

	SDL_RenderPresent(renderer);

	--current_color;
	if (current_color < 0) current_color += 360;
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

bool Game::running()
{
	return Game::is_running;
}

void HSV2RGB(int h, float s, float v, int& r, int& g, int& b) {
	float M = 255 * v;
	float m = M * (1 - s);
	float sup = fmod(h / 60.0f, 2.0f);
	sup = (sup < 0) ? sup : --sup;
	float z = (M - m) * (1 - abs(sup));
	switch (h / 60) {
	case 0:
		r = M;
		g = z + m;
		b = m;
		break;
	case 1:
		r = z + m;
		g = M;
		b = m;
		break;
	case 2:
		r = m;
		g = M;
		b = z + m;
		break;
	case 3:
		r = m;
		g = z + m;
		b = M;
		break;
	case 4:
		r = z + m;
		g = m;
		b = M;
		break;
	case 5:
		r = M;
		g = m;
		b = z + m;
		break;
	default:
		break;
	}
}