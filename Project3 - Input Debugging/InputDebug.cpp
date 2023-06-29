#include "InputDebug.h"


enum EventType : size_t
{
	KEYBOARD,
	MOUSE,
	JOYSTICK,
	CONTROLLER,
	TOUCH
};

int mask = 0xFFF0;

int get_event_type(SDL_Event* event) {
	switch (event->type & mask) {
	case 0x300:
		return KEYBOARD;
	case 0x400:
		return MOUSE;
	case 0x600:
		return JOYSTICK;
	case 0x650:
		return CONTROLLER;
	case 0x700:
		return TOUCH;
	}
}

std::string hat_dir(int val)
{
	std::string dir = "";
	if (val == 0)
		return "NONE";
	if (val & 1)
		dir += "UP";
	if ((val >> 2) & 1)
		dir += "DOWN";
	if ((val >> 1) & 1)
		dir += "RIGHT";
	if ((val >> 3) & 1)
		dir += "LEFT";
	return dir;
}

std::string stick(int val)
{
	switch (val) {
	case 0:
		return "LS X";
	case 1:
		return "LS Y";
	case 2:
		return "RS X";
	case 3:
		return "RS Y";
	case 4:
		return "LT";
	case 5:
		return "RT";
	}
}

std::string button(int val)
{
	switch (val) {
	case 0:
		return "A";
	case 1:
		return "B";
	case 2:
		return "X";
	case 3:
		return "Y";
	case 4:
		return "LB";
	case 5:
		return "RB";
	case 6:
		return "SELECT";
	case 7:
		return "START";
	case 8:
		return "LS";
	case 9:
		return "RS";
	}
}


InputDebug::InputDebug()
{
	console = new Console();
	for (int i = 0; i < 6; i++)
		jaxis_values[i] = 0;
}

void InputDebug::render()
{
	console->render();
}

void InputDebug::log(SDL_Event* event)
{
	int index = -1, val, type = get_event_type(event);
	int offset = event->type - (event->type & mask);
	std::string type_name;
	std::string action = "";
	switch (type) {
	case KEYBOARD:
		type_name = "KEYBOARD";
		index = event->key.keysym.scancode;
		switch (offset) {
		case 0:
			action = "key down";
			break;
		case 1:
			action = "key up";
			break;
		}
		break;
	case MOUSE:
		type_name = "MOUSE";
		switch (offset) {
		case 1:
			action = "button down";
			break;
		case 2:
			action = "button up";
			break;
		}
		break;
	case JOYSTICK:
		type_name = "JOYSTICK";
		switch (offset) {
		case 0:
			val = event->jaxis.value;
			if (abs(val - jaxis_values[event->jaxis.axis]) > 20000) {
				jaxis_values[event->jaxis.axis] = val;
				action = stick(event->jaxis.axis) + " axis " + std::to_string(val);
			}
			break;
		case 1:
			index = event->jball.ball;
			action = "ball motion";
			break;
		case 2:
			action = "hat " + hat_dir(event->jhat.value);
			break;
		case 3:
			action = button(event->jbutton.button) + " button down";
			break;
		case 4:
			action = button(event->jbutton.button) + " button up";
			break;
		}
		break;
	case CONTROLLER:
		type_name = "CONTROLLER";
		switch (offset) {
		case 0:
			index = event->caxis.axis;
			if ((event->jaxis.value < -3200) || (event->jaxis.value > 3200))
				action = "axis motion";
			break;
		case 1:
			index = event->cbutton.button;
			action = "button down";
			break;
		case 2:
			index = event->cbutton.button;
			action = "button up";
			break;
		case 6:
			index = event->ctouchpad.touchpad;
			action = "touchpad down";
			break;
		case 7:
			index = event->ctouchpad.touchpad;
			action = "touchpad motion";
			break;
		case 8:
			index = event->ctouchpad.touchpad;
			action = "touchpad up";
			break;
		case 9:
			index = event->csensor.sensor;
			action = "sensor update";
			break;
		}
		break;
	case TOUCH:
		type_name = "TOUCH";
		index = event->tfinger.touchId;
		switch (offset) {
		case 0:
			action = "finger down";
			break;
		case 1:
			action = "finger up";
			break;
		}
		break;
	}
	if (!action.empty() and !type_name.empty())
	{
		if (index >= 0)
			console->print(type_name + ": " + std::to_string(index) + " " + action);
		else
			console->print(type_name + ": " + action);
	}
}