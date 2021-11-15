#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysBody.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	idle.PushBack({7, 325, 20,27 });
	idle.loop = false;


	jumpAnim.PushBack({ 135, 325, 20,27 });
	jumpAnim.PushBack({ 167 , 323, 20,27 });
	jumpAnim.PushBack({ 197, 327, 22,28 });
	jumpAnim.PushBack({ 231, 329, 20,26 });
	jumpAnim.loop = true;
	jumpAnim.speed = 0.05f;


	moveAnim.PushBack({ 7, 325, 20,27 });
	moveAnim.PushBack({ 39 , 325, 20,27 });
	moveAnim.PushBack({ 69, 327, 22,25 });
	moveAnim.PushBack({ 103, 326, 20,26 });
	moveAnim.loop = true;
	moveAnim.speed = 0.05f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	bool ret = true;

	LOG("Loading player");
	texture = App->textures->Load("../Assets/Sprites/Characters.png");
	if (texture == nullptr)
		ret = false;

	currentAnimation = &moveAnim;

	position.x = 250;
	position.y = 250;
	
	speed.x = 0.2;
	speed.y = 0.8;

	return ret;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	lasPosition = position;

	//gravity
	position.y += 0.6;

	//input

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_STATE::KEY_REPEAT)
	{
		position.x += speed.x;
		direction = SDL_FLIP_NONE;
		moveAnim.Reset();
		currentAnimation = &moveAnim;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_STATE::KEY_REPEAT)
	{
		position.x -= speed.x;
		direction = SDL_FLIP_HORIZONTAL;
		moveAnim.Reset();
		currentAnimation = &moveAnim;
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_STATE::KEY_REPEAT && canJumnp)
	{
		canJumnp = false;
		position.y -= speed.y*5;
		moveAnim.Reset();
		currentAnimation = &jumpAnim;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_STATE::KEY_REPEAT)
	{
		position.y += speed.y;
	}

	if (   App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_STATE::KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_STATE::KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_STATE::KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_UP) == KEY_STATE::KEY_IDLE)
	{
		currentAnimation = &idle;
	}

	


	if (position.y > SCREEN_HEIGHT - 25 - 20)
		position.y = lasPosition.y;

	App->renderer->DrawLine(0,SCREEN_HEIGHT-25, SCREEN_WIDTH,SCREEN_HEIGHT-25,50,255,125,255,false);

	App->renderer->DrawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT - 25 - 15, 20, 125, 125, 0);

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->renderer->Blit(texture, position.x-App->renderer->camera.x, position.y - App->renderer->camera.y, &rect,direction,false,0.6,1,0);





	return UPDATE_CONTINUE;
}



