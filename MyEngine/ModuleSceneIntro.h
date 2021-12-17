#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"

#define BOUNCER_TIME 200

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
	
	bool CleanUp();

	void OnCollision(Collider* body1, Collider* body2);

public:

	SDL_Texture* graphics;
	PhysBody* background;


	Collider* squareBullet;
	Collider* squareBullet2;
	Collider* circleBullet;
	Collider* circleBullet2;


	//FLORS
	Collider* Flor1;
	Collider* Flor2;
	Collider* CenterCube;

	Bouncer bouncer1;
	Bouncer bouncer2;

	Bouncer side_bouncer1;
	Bouncer side_bouncer2;

};
