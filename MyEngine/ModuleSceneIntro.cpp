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

	currentTime = 0;
	AsteroidCounter = 0;
	InGame = true;
	InParticleSystem = false;
	return ret;
}

update_status ModuleSceneIntro::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		InGame = !InGame;
		InParticleSystem = false;
		App->player->GodMode = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		ResetGame();
	}
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		InParticleSystem = !InParticleSystem;
		InGame = false;
		App->player->GodMode = true;
	}
	return UPDATE_CONTINUE;
}


// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		SpawnAsteriod();
	}
	if (InGame)
	{
		AsteroidHorde();
	}
	if (InParticleSystem)
	{
		ParticleSystem();
	}
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate()
{
	SDL_Rect r;
	if (InGame)
	{
		r.x =20;
		r.y = 20;
		r.w = 25;
		r.h = 25;
		App->renderer->DrawQuad(r, 55, 200, 55, 200);
	}
	else {
		r.x = 20;
		r.y = 20;
		r.w = 25;
		r.h = 25;
		App->renderer->DrawQuad(r, 200, 55, 55, 200);
	}

	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::ResetGame()
{
	currentTime = App->GetTime();
	AsteroidCounter = 0;
	InGame = true;
	App->collisions->OnResetGame();
	App->player->RestartPlayer();
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
			App->player->bulletCounter--;
			break;
		case BOMB:
			if (body1->velocity.x == 0 && body1->velocity.y == 0)
			{
				//explode
				body1->pendingToDelete = true;
				App->player->bulletCounter--;
			}
			if (body2 != nullptr && body2->type == Collider::Type::ENEMY)
			{
				body1->pendingToDelete = true;
				App->player->bulletCounter--;
			}
			break;
		case BOUNCER:
			if (body2 == nullptr  || body2->type == Collider::Type::WALL  )
			{
				body1->bulletProperties.bounceCounter--;
				if (body1->bulletProperties.bounceCounter <= 0)
				{
					body1->pendingToDelete = true;
					App->player->bulletCounter--;
				}
			}
			else if(body2->type == Collider::Type::ENEMY)
			{
				body1->pendingToDelete = true;
				App->player->bulletCounter--;
			}
			break;
		}
		break;
	case Collider::Type::PLAYER:
		App->player->canJump = true;
		if (body2 != nullptr)
		{
			switch (body2->type)
			{
			case Collider::Type::WALL:
				//in ground
				break;
			case Collider::Type::ENEMY:
				if (!App->player->GodMode && InGame)
				{
					ResetGame();
				}
				break;
			case Collider::Type::BULLET:
				//lets say palyer can't kill himself
				break;
			default:
				break;
			}
		}
		else {
			// if body2 is null we know it hitted with a boundery
			// and in game player dies on getting out of the boundaries
			if (!App->player->GodMode && InGame)
			{
				ResetGame();
			}
		}
		break;
	case Collider::Type::ENEMY:
		if (body2 != nullptr)
		{
			switch (body2->type)
			{
			case Collider::Type::WALL:
				body1->pendingToDelete = true;
				AsteroidCounter--;
				break;
			case Collider::Type::BULLET:
				body1->pendingToDelete = true;
				AsteroidCounter--;
				break;
			case Collider::Type::PLAYER:
				if (!App->player->GodMode && InGame)
				{
					ResetGame();
				}
				body1->pendingToDelete = true;
				AsteroidCounter--;
				break;
			default:
				break;
			}
		}
		else {
			//destroy when on bounds
			body1->pendingToDelete = true;
			AsteroidCounter--;
		}

		break;
	default:
		break;
	}
}

void ModuleSceneIntro::SpawnAsteriod()
{
	fPoint spawnPos, targetPos;
	float asteroidSize, initialvelocity;
	asteroidSize = rand() % 10 + 10; //between 10 and 20
	initialvelocity = 0.35f;

	float targetx, targety;
	int n = 40; //target offset
	targetx = rand() % n + (-n) ; //n time more or less the player radius
	targety = rand() % n + (-n) ; //n time more or less the player radius
	targetx += App->player->player->position.x;
	targety += App->player->player->position.y;
	targetPos = { targetx , targety };

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

	AsteroidCounter++;
}

void ModuleSceneIntro::AsteroidHorde()
{
	if (AsteroidCounter < MAX_ASTEROIDS)
	{
		if(App->GetTime()-currentTime > secondsToSpawn)
		{
			for (int i = 0; i < spawnPerSecond; i++)
			{
				SpawnAsteriod();
			}
			currentTime = App->GetTime();
		}
	}
}
void ModuleSceneIntro::ParticleSystem()
{
	if (AsteroidCounter < MAX_ASTEROIDS)
	{
		SpawnAsteriod();
	}
}