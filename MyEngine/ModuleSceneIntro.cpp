#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"
#include "Collider.h"

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

	rectangle.x = 180;
	rectangle.y = 180;
	rectangle.w = 50;
	rectangle.h = 50;

	ground1 = App->collisions->AddRectangleCollider(rectangle, Collider::WALL, App->collisions);
	ground1->SetCenter();
	ground1->activeGravity = false;

	rectangle.x = 180;
	rectangle.y = 520;
	rectangle.w = 50;
	rectangle.h = 50;

	ground2 = App->collisions->AddRectangleCollider(rectangle, Collider::WALL, App->collisions);
	ground2->SetCenter();
	ground2->activeGravity = false;

	rectangle.x = 482;
	rectangle.y = 350;
	rectangle.w = 50;
	rectangle.h = 50;
	ground3 = App->collisions->AddRectangleCollider(rectangle, Collider::WALL, App->collisions);
	ground3->activeGravity = false;
	ground3->SetCenter();

	rectangle.x = 760;
	rectangle.y = 520 ;
	rectangle.w = 50;
	rectangle.h = 50;
	ground4 = App->collisions->AddRectangleCollider(rectangle, Collider::WALL, App->collisions);
	ground4->activeGravity = false;
	ground4->SetCenter();

	rectangle.x = 760;
	rectangle.y = 180;
	rectangle.w = 50;
	rectangle.h = 50;
	ground5 = App->collisions->AddRectangleCollider(rectangle, Collider::WALL, App->collisions);
	ground5->activeGravity = false;
	ground5->SetCenter();


	return ret;
}

update_status ModuleSceneIntro::PreUpdate()
{

	return UPDATE_CONTINUE;
}


// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		SpawnAsteriod();
	}
	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::OnCollision(Collider* body1, Collider* body2) //carefull this bdoy2 may be nullptr
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
	case Collider::Type::ENEMY:
		if (body2 != nullptr)
		{
			switch (body2->type)
			{
			case Collider::Type::WALL:
				body1->pendingToDelete = true;
				break;
			default:
				break;
			}
			break;
		}
	default:
		break;
	}
}

void ModuleSceneIntro::SpawnAsteriod()
{
	fPoint spawnPos, targetPos;
	float asteroidSize, initialvelocity;
	asteroidSize = rand() % 10 + 10; //between 10 and 20
	//initialvelocity = static_cast <float> (rand()) / static_cast <float> (1) + 0.1f;
	initialvelocity = 0.4f;

	float targetx, targety;
	targetx = rand() % 10 + (-10) ; //3 time more or less the player radius
	targety = rand() % 10 + (-10) ; //3 time more or less the player radius
	targetx += App->player->player->position.x;
	targety += App->player->player->position.y;
	targetPos = { targetx , targety };
	//
	//targetPos.x = App->player->player->position.x;
	//targetPos.y = App->player->player->position.y;

	// chose a side from screen to spawn
	int side;
	float x, y;
	side = rand() % 4 + 1;
	float spawnOffset;
	spawnOffset = 10 + asteroidSize;
	switch (side) {
	case 1://top
		x = rand() % SCREEN_WIDTH;
		spawnPos = { x, spawnOffset };
		break;
	case 2://right
		y = rand() % SCREEN_HEIGHT;
		spawnPos = { SCREEN_WIDTH - spawnOffset, y };
		break;
	case 3://bot
		x = rand() % SCREEN_WIDTH;
		spawnPos = { x,SCREEN_HEIGHT - spawnOffset };
		break;
	case 4://left
		y = rand() % SCREEN_HEIGHT;
		spawnPos = { spawnOffset ,y };
		break;
	}


	fPoint direction;
	direction.x = targetPos.x - spawnPos.x;
	direction.y = targetPos.y - spawnPos.y;

	float directionLength = sqrt(direction.x * direction.x + direction.y * direction.y);
	fPoint vDirectionNormalized = { direction.x / directionLength, direction.y / directionLength };

	Collider* asteroid;
	asteroid = App->collisions->AddCircleCollider(spawnPos, asteroidSize, Collider::Type::ENEMY, App->collisions);
	asteroid->velocity = { vDirectionNormalized.x * initialvelocity, vDirectionNormalized.y * initialvelocity };
	asteroid->listeners[1] = App->scene_intro;
	asteroid->activeGravity = true; 
	//asteroid->coeficientOfRestitution = 0.9;
	//asteroid->friction = 0.9;
}

void ModuleSceneIntro::AsteroidHorde()
{
}
