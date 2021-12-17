#pragma once
#include "Module.h"
#include "Globals.h"

#define NUM_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(Application* app, bool start_enabled = true);
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

	KEY_STATE GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}


private:
	KEY_STATE* keyboard;
	KEY_STATE mouseButtons[NUM_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
};