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
	https://happycoding.io/tutorials/processing/collision-detection

	if (body2->type == Collider::Type::BULLET || body2->type == Collider::Type::PLAYER)
	{

		//if velocity is very small stop the particle
		float dtvx = fabs(body2->velocity.x);
		float acbax = fabs(body2->acceleration.x);
		if (dtvx <= 0.05f)
		{
			body2->acceleration.x = 0;
			body2->velocity.x = 0;
		}
		//if velocity is very small stop the particle
		float dtvy = fabs(body2->velocity.y);
		if (dtvy < 0.05f)
		{
			body2->activeGravity = false;
			body2->acceleration.y = 0;
			body2->velocity.y = 0;
		}


		if (body2->collInfo->horizontal)
		{	
			body2->velocity.x *= -1;			
		}
		if (body2->collInfo->vertical)
		{
			body2->velocity.y *= -1;
		}

		body2->collInfo->horizontal = false;
		body2->collInfo->vertical = false;
		body2->collInfo->Collided = false;

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

void ModuleCollisions::CheckCollisions()
{

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
	//Other Forces...

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
				colliders[i]->SetPosition();

				colliders[i]->acceleration.x = colliders[i]->mass * colliders[i]->force.x;
				colliders[i]->acceleration.y = colliders[i]->mass * colliders[i]->force.y;

				colliders[i]->velocity.x += colliders[i]->acceleration.x * dt;
				colliders[i]->velocity.y += colliders[i]->acceleration.y * dt;

				colliders[i]->position.x += colliders[i]->velocity.x * dt;
				colliders[i]->position.y += colliders[i]->velocity.y * dt;

				colliders[i]->deltaPosition = { colliders[i]->lastPosition.x - colliders[i]->position.x, colliders[i]->lastPosition.y - colliders[i]->position.y };



				//LOG("Delata y [%i]: %f", i, colliders[i]->deltaPosition.y);
				//LOG("FORCE[%i]:x:%f y:%f", i, colliders[i]->force.x, colliders[i]->force.y);
				LOG("ACCELERATION[%i]:x:%f y:%f", i, colliders[i]->acceleration.x, colliders[i]->acceleration.y);
				//LOG("VELOCITY[%i]:x:%f y:%f", i, colliders[i]->velocity.x, colliders[i]->velocity.y);

				if (colliders[i]->rect.y + colliders[i]->rect.h  >  App->scene_intro->botWall->rect.y )
				{
					colliders[i]->position.y = App->scene_intro->botWall->position.y - (colliders[i]->rect.h*0.5f);
				}
				if (colliders[i]->rect.y < App->scene_intro->topWall->rect.y + App->scene_intro->topWall->rect.h)
				{
					colliders[i]->position.y = App->scene_intro->topWall->rect.y + App->scene_intro->topWall->rect.h + (colliders[i]->rect.h * 0.5f);
				}
				if (colliders[i]->rect.x < App->scene_intro->leftWall->rect.x + App->scene_intro->leftWall->rect.w)
				{
					colliders[i]->position.x = App->scene_intro->leftWall->rect.x + App->scene_intro->leftWall->rect.w + (colliders[i]->rect.w * 0.5f);
				}
				if (colliders[i]->rect.x + colliders[i]->rect.w > App->scene_intro->rightWall->rect.x)
				{
					colliders[i]->position.x = App->scene_intro->rightWall->rect.x - (colliders[i]->rect.w * 0.5f);
				}

				colliders[i]->SetPosition();

				App->renderer->DrawCircle(colliders[i]->position,2, 0, 255, 0, 255);


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