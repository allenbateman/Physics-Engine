#include "ModuleCollisions.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleInput.h"
#include "SDL/include/SDL_Scancode.h"

//https://www.toptal.com/game/video-game-physics-part-i-an-introduction-to-rigid-body-dynamics

ModuleCollisions::ModuleCollisions(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	debug = true;

	matrix[Collider::Type::WALL][Collider::Type::WALL] = false;
	matrix[Collider::Type::WALL][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::WALL][Collider::Type::BULLET] = true;

	matrix[Collider::Type::PLAYER][Collider::Type::WALL] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::BULLET] = true;


	matrix[Collider::Type::BULLET][Collider::Type::BULLET] = false;
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

	CheckCollisions();
	ApplyForces();

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleCollisions::Update(float dt)
{
	
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	ApplyMovement(dt);
	
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleCollisions::PostUpdate()
{
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
	LOG("COLLSION!");
	if (body1->shape == Collider::Shape::CIRCLE)
	{
		switch (body2->shape)
		{
		case Collider::Shape::CIRCLE:
			body1->velocity.y =  -body1->velocity.y;

			break;
		case Collider::Shape::RECTANGLE:
			body1->velocity.y = -body1->velocity.y;
			break;
		default:
			break;
		}
	}
	else if (body1->shape == Collider::Shape::RECTANGLE)
	{
		switch (body2->shape)
		{
		case Collider::Shape::CIRCLE:
			body1->velocity.y = -body1->velocity.y;
			break;
		case Collider::Shape::RECTANGLE:
			if (body1->type == Collider::Type::BULLET)
			{

				if (body1->position.y + body1->rect.h >= SCREEN_HEIGHT)
				{
					body1->position.y = SCREEN_HEIGHT - body1->rect.h;
				}
					body1->velocity.y *= -1;
					body1->velocity.x *= -1;
				
			}
			break;
		default:
			break;
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

			if (matrix[c1->type][c2->type] && c1->Intersects(c2))
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
	//Gravity
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			if (colliders[i]->type != Collider::Type::WALL)
			{
				colliders[i]->force += fPoint(0 ,colliders[i]->mass * 0.00005f);
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

			/*	colliders[i]->acceleration.x = colliders[i]->force.x;
				colliders[i]->acceleration.y = colliders[i]->force.y;

				colliders[i]->velocity.x += colliders[i]->acceleration.x * dt;
				colliders[i]->velocity.y += colliders[i]->acceleration.y * dt;*/
			

				colliders[i]->position.x += colliders[i]->velocity.x * dt;
				colliders[i]->position.y += colliders[i]->velocity.y * dt;

				colliders[i]->SetPosition();


				App->renderer->DrawCircle(colliders[i]->position,2, 0, 255, 0, 255);

				if (colliders[i]->position.y > SCREEN_HEIGHT)
					//colliders[i]->position.y = SCREEN_HEIGHT;

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