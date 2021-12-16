#include "ModuleCollisions.h"

#include "Application.h"
#include <math.h>
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "SDL/include/SDL_Scancode.h"

//https://www.toptal.com/game/video-game-physics-part-i-an-introduction-to-rigid-body-dynamics

ModuleCollisions::ModuleCollisions(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	debug = true;

	matrix[Collider::Type::WALL][Collider::Type::WALL] = false;
	matrix[Collider::Type::WALL][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WALL][Collider::Type::BULLET] = true;

	matrix[Collider::Type::PLAYER][Collider::Type::WALL] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::BULLET] = true;


	matrix[Collider::Type::BULLET][Collider::Type::BULLET] = true;
	matrix[Collider::Type::BULLET][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::BULLET][Collider::Type::WALL] = true;
}

// Destructor
ModuleCollisions::~ModuleCollisions()
{

}

update_status ModuleCollisions::PreUpdate()
{
	RemovePendingToDeleteColliders();

	//Sumatory forces
	ApplyForces();


	return update_status::UPDATE_CONTINUE;
}

update_status ModuleCollisions::Update(float dt)
{
	
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	ApplyMovement(dt);

	CheckCollisions();

	
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleCollisions::PostUpdate()
{
	//set force to zero
	if (debug)
		DebugDraw();

	return update_status::UPDATE_CONTINUE;
}

void ModuleCollisions::DebugDraw()
{
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->shape) {
		case Collider::Shape::RECTANGLE:
			switch (colliders[i]->type)
			{
				case Collider::Type::NONE: // white
					App->renderer->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
					break;
				case Collider::Type::WALL: // blue
					App->renderer->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
					break;
				case Collider::Type::PLAYER: // green
					App->renderer->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
					break;
				case Collider::Type::BULLET: // red
					App->renderer->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
					break;
			}
			break;
		case Collider::Shape::CIRCLE:
			switch (colliders[i]->type)
			{
				case Collider::Type::NONE: // white
					App->renderer->DrawCircle(colliders[i]->position, colliders[i]->radius, 255, 255, alpha);
					break;
				case Collider::Type::WALL: // blue
					App->renderer->DrawCircle(colliders[i]->position, colliders[i]->radius, 0, 0, 255, alpha);
					break;
				case Collider::Type::PLAYER: // green
					App->renderer->DrawCircle(colliders[i]->position, colliders[i]->radius, 0, 255, 0, alpha);
					break;
				case Collider::Type::BULLET: // red
					App->renderer->DrawCircle(colliders[i]->position, colliders[i]->radius, 255, 0, 0, alpha);
					break;
			}
			break;
		default:
			break;
		}

	}
}

void ModuleCollisions::OnCollision(Collider* body1, Collider* body2)
{

	fPoint v1 = { body1->velocity.x - body1->velocity.x, body1->velocity.y - body1->velocity.y };
	fPoint v2 = { body2->velocity.x - body1->velocity.x, body2->velocity.y - body1->velocity.y };
	body2->velocity.x = -v2.x;
	body2->velocity.y = -v2.y;

}

void ModuleCollisions::CheckParticleInBounds()
{

	bool checkLeft = false;
	bool checkRight = false;
	bool checkTop = false;
	bool checkBot = false;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		if (colliders[i]->shape == Collider::Shape::CIRCLE)
		{
			checkLeft = colliders[i]->position.x - colliders[i]->radius > 0;
			checkRight = colliders[i]->position.x + colliders[i]->radius  < SCREEN_WIDTH;
			if (checkLeft && checkRight)
			{
				// in xBounds
			}
			else {
				if (!checkLeft)
					colliders[i]->collInfo->Left = true;
				if (!checkRight)
					colliders[i]->collInfo->Right = true;

				colliders[i]->position.x = colliders[i]->lastPosition.x;
				colliders[i]->velocity.x *= -1;
			}


			checkTop = colliders[i]->position.y - colliders[i]->radius > 0;
			checkBot = colliders[i]->position.y + colliders[i]->radius < SCREEN_HEIGHT;

			if (checkBot && checkTop)
			{
				// in xBounds
			}
			else {
				if (!checkTop)
					colliders[i]->collInfo->Top = true;
				if (!checkBot)
					colliders[i]->collInfo->Bot = true;

				colliders[i]->position.y = colliders[i]->lastPosition.y;
				colliders[i]->velocity.y *= -1;
			}

		}else
		if (colliders[i]->shape == Collider::Shape::RECTANGLE)
		{
			checkLeft = colliders[i]->position.x - (colliders[i]->rect.w * 0.5) > 0;
			checkRight = colliders[i]->position.x + (colliders[i]->rect.w * 0.5) < SCREEN_WIDTH;
			if (checkLeft && checkRight)
			{
				// in xBounds
			}
			else {
				if (!checkLeft)
					colliders[i]->collInfo->Left = true;
				if (!checkRight)
					colliders[i]->collInfo->Right = true;

				colliders[i]->position.x = colliders[i]->lastPosition.x;
				colliders[i]->velocity.x *= -1;
			}

			checkTop = colliders[i]->position.y - (colliders[i]->rect.h * 0.5) > 0;
			checkBot = colliders[i]->position.y + (colliders[i]->rect.h * 0.5) < SCREEN_HEIGHT;

			if (checkBot && checkTop)
			{
				// in yBounds
			}
			else {

				if (!checkBot)
					colliders[i]->collInfo->Bot = true;
				if (!checkTop)
					colliders[i]->collInfo->Top = true;

				colliders[i]->position.y = colliders[i]->lastPosition.y;
				colliders[i]->velocity.y *= -1;


			}
		}
	}

}

void ModuleCollisions::CheckCollisions()
{

	//between bounds
	CheckParticleInBounds();


	//between particles
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (matrix[c1->type][c2->type] && c1->Intersects(c2)->Collided)
			{
				for (uint i = 0; i < MAX_LISTENERS; ++i)
					if (c1->listeners[i] != nullptr) c1->listeners[i]->OnCollision(c1, c2);

				for (uint i = 0; i < MAX_LISTENERS; ++i)
					if (c2->listeners[i] != nullptr) c2->listeners[i]->OnCollision(c2, c1);
			}
		}
	}

}

void ModuleCollisions::ApplyForces()
{
	
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			if (colliders[i]->type != Collider::Type::WALL)
			{

				colliders[i]->force = fPoint(0, 0);

				//Gravity
				if (colliders[i]->velocity.y != 0.00000f)
					colliders[i]->activeGravity = true;

				if(colliders[i]->activeGravity)
				   colliders[i]->force += fPoint(0 , 0.0005f);

				//Wind
				colliders[i]->force += fPoint(0,0);

			}
		}
	}
}

void ModuleCollisions::ApplyMovement(float dt)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			if (colliders[i]->type != Collider::Type::WALL)
			{
				

				colliders[i]->lastPosition = colliders[i]->position;
				colliders[i]->SetCenter();

				colliders[i]->acceleration.x = colliders[i]->mass * colliders[i]->force.x;
				colliders[i]->acceleration.y = colliders[i]->mass * colliders[i]->force.y;

				colliders[i]->velocity.x += colliders[i]->acceleration.x * dt;
				colliders[i]->velocity.y += colliders[i]->acceleration.y * dt;

				if (colliders[i]->velocity.x > MAX_VELOCITY)  colliders[i]->velocity.x = MAX_VELOCITY;
				else if (colliders[i]->velocity.x < -MAX_VELOCITY) colliders[i]->velocity.x = -MAX_VELOCITY;
				else if (colliders[i]->velocity.y > MAX_VELOCITY)  colliders[i]->velocity.y = MAX_VELOCITY;
				else if (colliders[i]->velocity.y > -MAX_VELOCITY) colliders[i]->velocity.y = -MAX_VELOCITY;

				colliders[i]->position.x += colliders[i]->velocity.x * dt;
				colliders[i]->position.y += colliders[i]->velocity.y * dt;

				colliders[i]->deltaPosition = { colliders[i]->lastPosition.x - colliders[i]->position.x, colliders[i]->lastPosition.y - colliders[i]->position.y };
				
				colliders[i]->SetCenter();


				//LOG("Delata y [%i]: %f", i, colliders[i]->deltaPosition.y);
				//LOG("FORCE[%i]:x:%f y:%f", i, colliders[i]->force.x, colliders[i]->force.y);
				//LOG("ACCELERATION[%i]:x:%f y:%f", i, colliders[i]->acceleration.x, colliders[i]->acceleration.y);
				//LOG("VELOCITY[%i]:x:%f y:%f", i, colliders[i]->velocity.x, colliders[i]->velocity.y);

			

				App->renderer->DrawCircle(colliders[i]->position,2, 125, 255, 0, 255);

				if(colliders[i]->type == Collider::Type::PLAYER)
					LOG("POSITION[%i]:x:%f y:%f", i, colliders[i]->position.x, colliders[i]->position.y);

				if (i == -1)
				{
					
					LOG("ACCELERATION[%i]:x:%f y:%f", i, colliders[i]->acceleration.x, colliders[i]->acceleration.y);
					LOG("VELOCITY[%i]:x:%f y:%f", i, colliders[i]->velocity.x, colliders[i]->velocity.y);
					LOG("POSITION[%i]:x:%f y:%f", i, colliders[i]->position.x, colliders[i]->position.y);
				}
			}
		}
	}
}

Collider* ModuleCollisions::AddRectangleCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}

Collider* ModuleCollisions::AddCircleCollider(fPoint center,float radius, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(center, radius, type, listener);
			break;
		}
	}

	return ret;
}

void ModuleCollisions::RemovePendingToDeleteColliders()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			if (colliders[i]->pendingToDelete == true) {
				delete colliders[i];
				colliders[i] = nullptr;
			}
		}
	}
}

void ModuleCollisions::RemoveCollider(Collider* collider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
}


// Called before quitting
bool ModuleCollisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}
