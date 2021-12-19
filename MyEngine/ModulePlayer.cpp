#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleCollisions.h"
#include "PhysBody.h"


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{

}

// Load assets
bool ModulePlayer::Start()
{
	bool ret = true;

	LOG("Loading player");
	player = App->collisions->AddCircleCollider({0,0}, 20, Collider::PLAYER, App->collisions);
	player->SetPosition(spawnPos.x, spawnPos.y);
	player->mass = 1;
	player->friction = 0;
	player->coeficientOfRestitution = 0;
	player->Bounce = false;
	player->activeGravity = true;
	canJump = true;
	currentMovementType = SPEED;
	speed = { 0.1f,0.1f };
	force = { 0.01f,0.01f };

	currentWeapon = BLASTER;

	weaponsTexture = App->textures->Load("../Assets/Sprites/SpacialWeapons.png");

	blaster.x = 0;
	blaster.y = 0;
	blaster.w = 174;
	blaster.h = 54;
	pivotBlaster = { 50,14};

	cannon.x = 0;
	cannon.y = 114;
	cannon.w = 174;
	cannon.h = 54;
	pivotCannon = { 24,134 };

	bounceShooter.x = 0;
	bounceShooter.y = 54;
	bounceShooter.w = 153;
	bounceShooter.h = 54;
	pivotBouncer = {64,80};


	return ret;
}

update_status ModulePlayer::PreUpdate()
{

	//player indestructible
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		GodMode = !GodMode;
	}

	//type of movement
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		currentMovementType = SPEED;

	}else if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
	{
		currentMovementType = FORCES;
	}

	switch (currentMovementType)
	{
	case SPEED:
		MoveBySpeed();
		break;
	case FORCES:
		MoveByForces();
		break;
	}

	//type of weapon
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		currentWeapon = BLASTER;
	}
	else if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		currentWeapon = CANNON;
	}
	else if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		currentWeapon = BOUNCER_SHOOTER;
	}

	//Aim and Shoot
	Aim();
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && canShoot)
	{
		if (bulletCounter < MAX_BULLETS)
		{
			Shoot();
			bulletCounter++;
		}
	}
	CalcualteAngle();
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::Update(float dt)
{
	if(App->collisions->debug)
		RenderWeapons();

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
	canShoot = true;
	SDL_Rect rect;
	rect.x = 100;
	rect.y = 100;
	rect.w = 174;
	rect.h = 168;
	App->renderer->Blit(weaponsTexture,0,0,&rect);
	RenderWeapons();
	return UPDATE_CONTINUE;
}

bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(weaponsTexture);
	return true;
}

void ModulePlayer::OnCollision(Collider* body1, Collider* body2)
{
	// we dont do that here
}

void ModulePlayer::Shoot()
{
	if (canShoot)
	{
		switch (currentWeapon) {
		case BLASTER:
			SpawnLaser();
			break;
		case CANNON:
			SpawnBomb();
			break;
		case BOUNCER_SHOOTER:
			SpawnBouncer();
			break;
		}

	}
}

void ModulePlayer::Aim()
{
	fPoint mousePos, endPos;
	mousePos.x = App->input->GetMouseX();
	mousePos.y = App->input->GetMouseY();

	App->collisions->CheckLineCollisionsWithRectangles(player->position, mousePos, endPos);
	if (endPos.x == 0 && endPos.y == 0)
		endPos = mousePos;
	App->renderer->DrawLine(player->position.x, player->position.y, endPos.x, endPos.y, 0, 100, 255, 150,true);
	App->renderer->DrawCircle(endPos, 2, 125, 255, 125, 200);

}

void ModulePlayer::CalcualteAngle()
{
	fPoint mousePos;
	mousePos.x = App->input->GetMouseX();
	mousePos.y = App->input->GetMouseY();

	fPoint dir;
	dir.x = mousePos.x - player->position.x;
	dir.y = mousePos.y - player->position.y;
	float module = sqrt(dir.x * dir.x + dir.y * dir.y);

	angle = acos(dir.x / module);
	angle = (RADTODEG * angle);

	if (dir.y < 0)
		angle = 360 - angle;

	LOG("ANGLE:%f", angle);

	if ((dir.x / module)>0)
		direction = SDL_FLIP_NONE;
	else
		direction = SDL_FLIP_VERTICAL;
}

void ModulePlayer::MoveByForces()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		player->force.x = -force.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		player->force.x = force.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && canJump)
	{
		player->force.y = -force.y;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && canJump)
	{
		player->force.y = force.y;
	}
}

void ModulePlayer::MoveBySpeed()
{

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		player->velocity.x = -speed.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		player->velocity.x = speed.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && canJump)
	{
		player->velocity.y = -speed.y;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && canJump)
	{
		player->velocity.y = speed.y;
	}
}

void ModulePlayer::SpawnLaser()
{
	canShoot = false;
	fPoint mousePos, endPos;
	mousePos.x = App->input->GetMouseX();
	mousePos.y = App->input->GetMouseY();

	fPoint direction;
	direction.x = mousePos.x - player->position.x;
	direction.y = mousePos.y - player->position.y;

	float directionLength = sqrt(direction.x * direction.x + direction.y * direction.y);
	fPoint vDirectionNormalized = { direction.x / directionLength, direction.y / directionLength };

	float spawnDistance = player->radius + 15;

	fPoint spawnPos = { player->position.x + vDirectionNormalized.x * spawnDistance, player->position.y + vDirectionNormalized.y * spawnDistance };

	Collider* bullet;
	bullet = App->collisions->AddBulletCollider(spawnPos, 5,LASER, App->collisions);
	bullet->velocity = { vDirectionNormalized.x * bullet->bulletProperties.velocity, vDirectionNormalized.y * bullet->bulletProperties.velocity };
	bullet->listeners[1] = App->scene_intro;
	bullet->activeGravity = false;
}

void ModulePlayer::SpawnBomb()
{
	canShoot = false;
	fPoint mousePos, endPos;
	mousePos.x = App->input->GetMouseX();
	mousePos.y = App->input->GetMouseY();

	fPoint direction;
	direction.x = mousePos.x - player->position.x;
	direction.y = mousePos.y - player->position.y;

	float directionLength = sqrt(direction.x * direction.x + direction.y * direction.y);
	fPoint vDirectionNormalized = { direction.x / directionLength, direction.y / directionLength };

	float spawnDistance = player->radius + 15;

	fPoint spawnPos = { player->position.x + vDirectionNormalized.x * spawnDistance, player->position.y + vDirectionNormalized.y * spawnDistance };

	Collider* bullet;
	bullet = App->collisions->AddBulletCollider(spawnPos, 15,BOMB, App->collisions);
	bullet->velocity = { vDirectionNormalized.x * bullet->bulletProperties.velocity, vDirectionNormalized.y * bullet->bulletProperties.velocity };
	bullet->listeners[1] = App->scene_intro;
}

void ModulePlayer::SpawnBouncer()
{
	canShoot = false;
	fPoint mousePos, endPos;
	mousePos.x = App->input->GetMouseX();
	mousePos.y = App->input->GetMouseY();

	fPoint direction;
	direction.x = mousePos.x - player->position.x;
	direction.y = mousePos.y - player->position.y;

	float directionLength = sqrt(direction.x * direction.x + direction.y * direction.y);
	fPoint vDirectionNormalized = { direction.x / directionLength, direction.y / directionLength };

	float spawnDistance = player->radius + 15;

	fPoint spawnPos = { player->position.x + vDirectionNormalized.x * spawnDistance, player->position.y + vDirectionNormalized.y * spawnDistance };

	Collider* bullet;
	bullet = App->collisions->AddBulletCollider(spawnPos, 10, BOUNCER, App->collisions);
	bullet->velocity = { vDirectionNormalized.x * bullet->bulletProperties.velocity, vDirectionNormalized.y * bullet->bulletProperties.velocity };
	bullet->listeners[1] = App->scene_intro;
}

void ModulePlayer::RestartPlayer()
{
	player->SetPosition(spawnPos.x, spawnPos.y);
	GodMode = false;
	PlayerAlive = true;
	bulletCounter = 0;
}

void ModulePlayer::RenderWeapons()
{
	if (weaponsTexture != nullptr)
	{
		switch (currentWeapon)
		{
		case BLASTER:
			App->renderer->Blit(weaponsTexture, player->position.x - pivotBlaster.x, player->position.y - pivotBlaster.y, &blaster, direction, false,0.6f, 1,angle,pivotBlaster);
			break;
		case CANNON:
			//App->renderer->Blit(weaponsTexture, player->position.x - pivotCannon.x, player->position.y - pivotCannon.y, &cannon, direction, false, 0.6f, 1, angle, pivotCannon);
			App->renderer->Blit(weaponsTexture, player->position.x - pivotBlaster.x, player->position.y - pivotBlaster.y, &blaster, direction, false, 0.6f, 1, angle, pivotBlaster);
			break;
		case BOUNCER_SHOOTER:
			//App->renderer->Blit(weaponsTexture, player->position.x - pivotBouncer.x, player->position.y - pivotBouncer.y, &bounceShooter, direction, false, 0.6f, 1, angle,pivotBouncer);
			App->renderer->Blit(weaponsTexture, player->position.x - pivotBlaster.x, player->position.y - pivotBlaster.y, &blaster, direction, false, 0.6f, 1, angle, pivotBlaster);
			break;
		default:
			break;
		}
	}
}


