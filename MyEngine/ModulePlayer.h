#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"

#define MAX_BULLETS 30

class ModulePlayer : public Module
{
public:
	enum CurrentWeapon {
		CANNON,
		BLASTER,
		BOUNCER_SHOOTER
	};
	enum MovementType {
		FORCES,
		SPEED,
	};
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status PreUpdate();
	update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();

	void OnCollision(Collider* body1, Collider* body2);

	void Shoot();
	void Aim();
	void CalcualteAngle();

	void MoveByForces();
	void MoveBySpeed();

	void SpawnLaser();
	void SpawnBomb();
	void SpawnBouncer();

	void RestartPlayer();

	void RenderWeapons();

	Collider* player;
	p2Point<float> speed;
	p2Point<float> force;
	p2Point<float> spawnPos = { 482, 280 };
	int bulletCounter = 0;

	bool canJump = true;
	bool canShoot = true;
	bool PlayerAlive = true;
	bool GodMode = false;

	MovementType currentMovementType = SPEED;
	CurrentWeapon currentWeapon = BLASTER;

	// A set of animations
	SDL_Texture* texture = nullptr;
	SDL_Texture* weaponsTexture = nullptr;
	SDL_Rect blaster;
	SDL_Rect cannon;
	SDL_Rect bounceShooter;
	SDL_Point pivotBlaster,pivotCannon,pivotBouncer;

	Animation* currentAnimation = nullptr;
	Animation moveAnim, idle, jumpAnim,fallAnim;
	SDL_RendererFlip direction;
	float angle;
	
};