#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Module.h"
#include "Dummy.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "ModuleCollisions.h"
#include "PerfTimer.h"
#include "Defs.h"
#include "Timer.h"


class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModulePlayer* player;
	ModuleSceneIntro* scene_intro;
	ModulePhysics* physics;
	ModuleCollisions* collisions;
	int frames;
private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool PrepareUpdate();
	bool FinishUpdate();
	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);

	PerfTimer ptimer;
	PerfTimer frameDuration;
	uint64 frameCount = 0;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	float dt = 0.0f;
	bool FPSCapTo30 = false;
	int maxFrameRate;
};