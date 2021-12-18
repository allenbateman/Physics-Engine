#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
	App->renderer->camera.x = App->renderer->camera.y = 0;


	SDL_Rect rectangle;

	rectangle.x = 250;
	rectangle.y = 200;
	rectangle.w = 150;
	rectangle.h = 50;

	Flor1 = App->collisions->AddRectangleCollider(rectangle, Collider::WALL, App->collisions);
	Flor1->SetCenter();

	rectangle.x = 850;
	rectangle.y = 200;
	rectangle.w = 50;
	rectangle.h = 250;

	Flor2 = App->collisions->AddRectangleCollider(rectangle, Collider::WALL, App->collisions);
	Flor2->SetCenter();

	rectangle.x = SCREEN_WIDTH * 0.5f;
	rectangle.y = SCREEN_HEIGHT * 0.5f;
	rectangle.w = 100;
	rectangle.h = 100;

	CenterCube = App->collisions->AddRectangleCollider(rectangle, Collider::WALL, App->collisions);
	CenterCube->SetCenter();

	circleBullet = App->collisions->AddCircleCollider(fPoint{120, 400}, 20,Collider::BULLET,App->collisions);
	circleBullet->mass = 1;
	circleBullet->velocity.x = 0.1f;
	circleBullet->velocity.y = 0.1f;

	circleBullet2 = App->collisions->AddCircleCollider(fPoint{ 420, 400 }, 20, Collider::BULLET, App->collisions);
	circleBullet2->mass = 1;
	circleBullet2->velocity.x = -0.1f;
	circleBullet2->velocity.y = 0.1f;

	return ret;
}

update_status ModuleSceneIntro::PreUpdate()
{
	fPoint addForce;
	addForce.x = 0.001f;
	addForce.y = 0.01;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		squareBullet->force.x += addForce.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		squareBullet->force.x -= addForce.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		squareBullet->force.y -= addForce.y;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		squareBullet->force.y += addForce.y;
	}

	return UPDATE_CONTINUE;
}


// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{

	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::OnCollision(Collider* body1, Collider* body2)
{
	LOG("COLLISION IN SCENE INTRO");

	if (body1->type == Collider::Type::BULLET)
	{
		if (body1->bulletProperties.type == LASER)
		{
			body1->pendingToDelete = true;
		}
	}
}