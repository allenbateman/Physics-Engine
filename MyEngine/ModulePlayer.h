#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status PreUpdate();
	update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();

	void OnCollision(Collider* body1, Collider* body2);

	void Aim();

	Collider* player;
	p2Point<float> speed;
	p2Point<float> force;
	
	p2Point<float> position;
	p2Point<float> lasPosition;
	bool canJumnp = true;

	// A set of animations
	SDL_Texture* texture = nullptr;
	Animation* currentAnimation = nullptr;
	Animation moveAnim, idle, jumpAnim,fallAnim;
	SDL_RendererFlip direction;
	
};