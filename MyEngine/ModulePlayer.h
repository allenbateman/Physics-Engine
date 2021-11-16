#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"

class PhysBody;

struct Object
{
	SDL_Texture* graphic;
	p2Point<float> position;
	float radius = 10;
	PhysBody* body;
	uint fx;

	void Move(float playerX) {
		
		float dist = playerX - position.x;
		if (dist < 50 && dist >10)
			radius = 10 * dist;
	}

	Object() : graphic(NULL), body(NULL)
	{}
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	//update_status PostUpdate();
	bool CleanUp();

	p2Point<float> speed;
	float counter =0;
	bool canJumnp = true;
	
	p2Point<float> position;
	p2Point<float> lasPosition;
	// The player spritesheet loaded into an SDL_Texture
	SDL_Texture* texture = nullptr;
	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation moveAnim, idle, jumpAnim,fallAnim;

	SDL_RendererFlip direction;
public:

	
};