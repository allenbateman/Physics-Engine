#pragma once
#include "Module.h"
#include "Globals.h"

#define GRAVITY -10

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

private:
	bool debug;
};