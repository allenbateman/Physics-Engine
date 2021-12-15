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

	SDL_Rect rect;
	rect.x = 0;
	rect.y = SCREEN_HEIGHT-5;
	rect.w = SCREEN_WIDTH;
	rect.h = 10;
	botWall = App->collisions->AddRectangleCollider(rect, Collider::Type::WALL, App->collisions); // Bottom
	rect.x = 0;
	rect.y = -10;
	rect.w = SCREEN_WIDTH;
	rect.h = 15;
	topWall = App->collisions->AddRectangleCollider(rect, Collider::Type::WALL, App->collisions); // Top
	rect.x = -10;
	rect.y = 0;
	rect.w = 15;
	rect.h = SCREEN_HEIGHT;
	leftWall = App->collisions->AddRectangleCollider(rect, Collider::Type::WALL, App->collisions); // Left
	rect.x = SCREEN_WIDTH-5;
	rect.y = 0;
	rect.w = 10;
	rect.h = SCREEN_HEIGHT;
	rightWall = App->collisions->AddRectangleCollider(rect, Collider::Type::WALL, App->collisions); // Right

	SDL_Rect rectangle;
	rectangle.x = 250;
	rectangle.y = 250;
	rectangle.w = 50;
	rectangle.h = 50;

	squareBullet = App->collisions->AddRectangleCollider(rectangle, Collider::BULLET, App->collisions);
	squareBullet->SetPosition();
	squareBullet->velocity.y = 0.0f;
	squareBullet->velocity.x = 0.0f;
	squareBullet->mass = 2;
	rectangle.x = 350;
	rectangle.y = 350;
	rectangle.w = 20;
	rectangle.h = 20;
	squareBullet2 = App->collisions->AddRectangleCollider(rectangle, Collider::PLAYER, App->collisions);
	squareBullet2->SetPosition();
	squareBullet2->velocity.y = 0.0f;
	squareBullet2->velocity.x = 0.0f;



	return ret;
}

update_status ModuleSceneIntro::PreUpdate()
{
	fPoint addForce;
	addForce.x = 0.001f;
	addForce.y = 0.01;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		squareBullet->force.x += addForce.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		squareBullet->force.x -= addForce.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		squareBullet->force.y -= addForce.y;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
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
}