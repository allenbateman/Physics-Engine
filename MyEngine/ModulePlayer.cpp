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

	position = { 250, 250 };
	player =  App->collisions->AddCircleCollider(position, 20 ,Collider::PLAYER,App->collisions);
	player->SetPosition(450, 250);
	player->mass = 1;


	speed = { 0.5f,0.7f };
	force = { 0.01f,0.01f };

	return ret;
}

update_status ModulePlayer::PreUpdate()
{

	//player->velocity.x = 0;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		player->force.x = -force.x;
		//player->velocity.x = -speed.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		player->force.x = force.x;
		//player->velocity.x = speed.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		player->force.y = -force.y;
		//player->velocity.y = -speed.y;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		player->force.y = force.y;
		//player->velocity.y = speed.y;
	}
	

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::Update(float dt)
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		Aim();
	

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{

	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

void ModulePlayer::OnCollision(Collider* body1, Collider* body2)
{

}

void ModulePlayer::Aim()
{
	int xMouse = App->input->GetMouseX();
	int yMouse = App->input->GetMouseY();

	App->renderer->DrawLine(player->position.x, player->position.y, xMouse, yMouse, 0, 100, 255, 255,true);

}


