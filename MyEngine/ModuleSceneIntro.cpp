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

	ground1 = App->collisions->AddRectangleCollider(rectangle, Collider::WALL, App->collisions);
	ground1->SetCenter();
	ground1->activeGravity = false;

	rectangle.x = 850;
	rectangle.y = 200;
	rectangle.w = 50;
	rectangle.h = 250;

	ground2 = App->collisions->AddRectangleCollider(rectangle, Collider::WALL, App->collisions);
	ground2->SetCenter();
	ground2->activeGravity = false;


	fPoint center;
	center.x = SCREEN_WIDTH * 0.5f;
	center.y = SCREEN_HEIGHT * 0.5f;
	ground3 = App->collisions->AddCircleCollider(center, 75.0f, Collider::WALL, App->collisions);
	ground3->activeGravity = false;


	circleBullet = App->collisions->AddCircleCollider(fPoint{100, 300}, 20,Collider::BULLET,App->collisions);
	circleBullet->mass = 1;
	circleBullet->velocity.x = 0.1f;
	circleBullet->velocity.y = 0.1f;

	circleBullet2 = App->collisions->AddCircleCollider(fPoint{ 500, 300 }, 20, Collider::BULLET, App->collisions);
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

	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{

	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{

	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{

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


	switch (body1->type)
	{
	case Collider::Type::BULLET:
		switch (body1->bulletProperties.type)
		{
		case LASER:
			body1->pendingToDelete = true;
			break;
		case BOMB:
			if (body1->velocity.x == 0)
			{
				//explode
				body1->pendingToDelete = true;
			}
			break;
		case BOUNCER:
			if (body2 == nullptr  || body2->type == Collider::Type::WALL  )
			{
				body1->bulletProperties.bounceCounter--;
				if (body1->bulletProperties.bounceCounter <= 0)
				{
					body1->pendingToDelete = true;
				}
			}
			else if(body2->type == Collider::Type::ENEMY)
			{
				body1->pendingToDelete = true;
			}

			break;
		}
		break;
	case Collider::Type::PLAYER:
		App->player->canJump = true;
		break;
	default:
		break;
	}
}