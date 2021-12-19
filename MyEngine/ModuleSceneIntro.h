#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"

#define BOUNCER_TIME 200
#define MAX_ASTEROIDS 70

struct Bouncer
{
	Bouncer() : body(NULL), texture(NULL), hit_timer(0), fx(0)
	{}

	PhysBody* body;
	SDL_Texture* texture;
	Uint32 hit_timer;
	uint fx;
};

enum lightTypes
{
	tiny,
	medium,
	big
};

class ModuleSceneIntro;

struct Light
{
	Light() : body(NULL), texture(NULL), on(false), fx(0)
	{}

	Light(ModuleSceneIntro* physics, int x, int y, lightTypes type);

	lightTypes type;
	PhysBody* body;
	SDL_Texture* texture;
	bool on;
	uint fx;
	int x, y;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate();
	update_status Update(float dt);
	update_status PostUpdate();
	
	bool CleanUp();

	void ResetGame();

	void OnCollision(Collider* body1, Collider* body2);

	void SpawnAsteriod();
	void AsteroidHorde();
	void ParticleSystem();
public:

	SDL_Texture* graphics;
	PhysBody* background;

	//grounds
	Collider* ground1;
	Collider* ground2;
	Collider* ground3;
	Collider* ground4;
	Collider* ground5;

	//wave variables
	int AsteroidCounter = 0;
	int spawnPerSecond = 3;
	float secondsToSpawn = 1;
	float currentTime = 0;

	bool InGame = false;
	bool InParticleSystem = false;
};
