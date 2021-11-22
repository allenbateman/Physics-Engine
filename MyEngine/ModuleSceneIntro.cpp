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
	App->collisions->AddRectangleCollider(rect, Collider::Type::WALL, this); // Bottom
	rect.x = 0;
	rect.y = -10;
	rect.w = SCREEN_WIDTH;
	rect.h = 15;
	App->collisions->AddRectangleCollider(rect, Collider::Type::WALL, this); // Top
	rect.x = -10;
	rect.y = 0;
	rect.w = 15;
	rect.h = SCREEN_HEIGHT;
	App->collisions->AddRectangleCollider(rect, Collider::Type::WALL, this); // Left
	rect.x = SCREEN_WIDTH-5;
	rect.y = 0;
	rect.w = 10;
	rect.h = SCREEN_HEIGHT;
	App->collisions->AddRectangleCollider(rect, Collider::Type::WALL, this); // Right





	SDL_Rect rectangle;
	rectangle.x = 250;
	rectangle.y = 250;
	rectangle.w = 50;
	rectangle.h = 50;
	App->collisions->AddRectangleCollider(rectangle, Collider::WALL, this);
	App->collisions->AddCircleCollider(fPoint(450, 450), 25, Collider::BULLET, this);




	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	

	return UPDATE_CONTINUE;
}
