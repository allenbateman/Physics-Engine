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
	Normal = false;
	Satelite = false;
	Interplanetary = true;

	matrix[Collider::Type::WALL][Collider::Type::WALL] = false;
	matrix[Collider::Type::WALL][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WALL][Collider::Type::BULLET] = true;
	matrix[Collider::Type::WALL][Collider::Type::ENEMY] = true;

	matrix[Collider::Type::PLAYER][Collider::Type::WALL] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::BULLET] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY] = true;


	matrix[Collider::Type::BULLET][Collider::Type::BULLET] = true;
	matrix[Collider::Type::BULLET][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::BULLET][Collider::Type::WALL] = true;
	matrix[Collider::Type::BULLET][Collider::Type::ENEMY] = true;

	matrix[Collider::Type::ENEMY][Collider::Type::WALL] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::BULLET] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY] = false;


}

// Destructor
ModuleCollisions::~ModuleCollisions()
{

}

update_status ModuleCollisions::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		NormalGravity();

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		StateliteGravity();

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		InterplanetaryGravity();

	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		debug = !debug;

	RemovePendingToDeleteColliders();
	//Sumatory forces
	ApplyForces();


	return update_status::UPDATE_CONTINUE;
}

update_status ModuleCollisions::Update(float dt)
{
	
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

	SDL_Rect r;
	if (Normal)
	{
		r.x = 0;
		r.y = 0;
		r.w = 25;
		r.h = 25;
		App->renderer->DrawQuad(r, 213, 235, 52, 150);
	}
	if (Satelite)
	{
		r.x = 25;
		r.y = 0;
		r.w = 25;
		r.h = 25;
		App->renderer->DrawQuad(r, 52, 235, 100, 150);
	}
	if (Interplanetary)
	{
		r.x = 50;
		r.y = 0;
		r.w = 25;
		r.h = 25;
		App->renderer->DrawQuad(r, 52, 216, 235, 150);
	}



	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		Color color = colliders[i]->color;
		switch (colliders[i]->shape) {
		case Collider::Shape::RECTANGLE:
			App->renderer->DrawQuad(colliders[i]->rect, color.r, color.g, color.b, color.a);
			break;
		case Collider::Shape::CIRCLE:
			App->renderer->DrawCircle(colliders[i]->position, colliders[i]->radius, color.r, color.g, color.b, color.a);
			break;
		default:
			break;
		}

	}
}

void ModuleCollisions::OnCollision(Collider* body1, Collider* body2)
{
	if (body1->type != Collider::WALL)
	{
		//calculate resultant velocity
		fPoint v2 = { body2->velocity.x - body1->velocity.x, body2->velocity.y - body1->velocity.y };

		v2.x *= body2->friction;

		v2.y *= body2->coeficientOfRestitution;


		//control high velocity
		v2 = CapBigVelocities(v2);

		//set velocity to zero if verly small
		v2 = StopVibration(body2->velocity);


		body2->velocity = { -v2.x, -v2.y };

	}
	else {

		if (body2->collInfo->Right && body2->velocity.x > 0 )
		{
			//reset x position before solving collision
			body2->position.x = body2->lastPosition.x;
			body2->velocity.x *= body2->friction;
			body2->velocity.x *= -1;
		}
		if (body2->collInfo->Left && body2->velocity.x < 0)
		{
			body2->position.x = body2->lastPosition.x;
			body2->velocity.x *= body2->friction;
			body2->velocity.x *= -1;
		}

		if (body2->collInfo->Bot && body2->velocity.y > 0)
		{
			body2->position.y = body2->lastPosition.y;
			body2->velocity.x *= body2->friction;
			body2->velocity.y *= body2->coeficientOfRestitution;
			body2->velocity.y *= -1;
		}
		if (body2->collInfo->Top && body2->velocity.y < 0)
		{
			body2->position.y = body2->lastPosition.y;
			body2->velocity.x *= body2->friction;
			body2->velocity.y *= body2->coeficientOfRestitution;
			body2->velocity.y *= -1;
		}
		//set velocity to zero if verly small
		body2->velocity = StopVibration(body2->velocity);
	}

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

		colliders[i]->velocity = StopVibration(colliders[i]->velocity);

		//Circle collision solver 
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

				if (colliders[i]->Bounce)
				{
					colliders[i]->velocity.x *= colliders[i]->friction;
					colliders[i]->velocity.x *= -1;
				}
				else {
					colliders[i]->velocity.x = 0;
				}
				colliders[i]->collInfo->Collided = true;

				//call the collsion solver from module scene
				if(colliders[i]->listeners[1] != nullptr)
					colliders[i]->listeners[1]->OnCollision(colliders[i], nullptr);
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

				if (colliders[i]->Bounce)
				{
					colliders[i]->velocity.x *= colliders[i]->friction;
					colliders[i]->velocity.y *= colliders[i]->coeficientOfRestitution;
					colliders[i]->velocity.y *= -1;
				}
				else {
					colliders[i]->velocity.y = 0;
				}
				colliders[i]->collInfo->Collided = true;
				//call the collsion solver from module scene
				if (colliders[i]->listeners[1] != nullptr)
					colliders[i]->listeners[1]->OnCollision(colliders[i], nullptr);
			}

		}else //Rectangle collision solver 
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
				if (colliders[i]->Bounce)
				{
					colliders[i]->velocity.x *= colliders[i]->friction;
					colliders[i]->velocity.x *= -1;
				}
				else {
					colliders[i]->velocity.x = 0;
				}
				colliders[i]->collInfo->Collided = true;

				//call the collsion solver from module scene
				if (colliders[i]->listeners[1] != nullptr)
					colliders[i]->listeners[1]->OnCollision(colliders[i], nullptr);
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
				if (colliders[i]->Bounce)
				{
					colliders[i]->velocity.x *= colliders[i]->friction;
					colliders[i]->velocity.y *= colliders[i]->coeficientOfRestitution;
					colliders[i]->velocity.y *= -1;
				}
				else {
					colliders[i]->velocity.y = 0;
				}
				colliders[i]->collInfo->Collided = true;

				//call the collsion solver from module scene
				if (colliders[i]->listeners[1] != nullptr)
					colliders[i]->listeners[1]->OnCollision(colliders[i], nullptr);
			}
		}
	}

}

void ModuleCollisions::CheckLineCollisionsWithRectangles(fPoint start, fPoint end, fPoint& _intersection)
{
	fPoint closestIntersection, currentintersection;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr || colliders[i]->shape != Collider::Shape::RECTANGLE)
			continue;
		SDL_Rect r;
		r = colliders[i]->rect;
		// left 
		if (LineLine(start.x, start.y, end.x, end.y, r.x, r.y, r.x, r.y + r.h, currentintersection))
		{
			App->renderer->DrawCircle(currentintersection, 2, 255, 0, 255, 255);
			closestIntersection = currentintersection;
	
		}
		//right 
		if (LineLine(start.x, start.y, end.x, end.y, r.x + r.w, r.y, r.x + r.w, r.y + r.h, currentintersection))
		{
			App->renderer->DrawCircle(currentintersection, 2, 255, 0, 255, 255);
			if (currentintersection.DistanceTo(start) < closestIntersection.DistanceTo(start))
				closestIntersection = currentintersection;
		}
		//top
		if (LineLine(start.x, start.y, end.x, end.y, r.x, r.y, r.x + r.w, r.y, currentintersection))
		{
			App->renderer->DrawCircle(currentintersection, 2, 255, 0, 255, 255);
			if (currentintersection.DistanceTo(start) < closestIntersection.DistanceTo(start))
				closestIntersection = currentintersection;
		}
		//bottom
		if (LineLine(start.x, start.y, end.x, end.y, r.x, r.y + r.h, r.x + r.w, r.y + r.h, currentintersection))
		{
			App->renderer->DrawCircle(currentintersection, 2, 255, 0, 255, 255);
			if (currentintersection.DistanceTo(start) < closestIntersection.DistanceTo(start))
				closestIntersection = currentintersection;
		}
	}
	_intersection = closestIntersection;
}

bool ModuleCollisions::LineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, fPoint &_intersection)
{
		// calculate the direction of the lines
		float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
		float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

		// if uA and uB are between 0-1, lines are colliding
		if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {

			fPoint intersection;
			intersection.x = x1 + (uA * (x2 - x1));
			intersection.y = y1 + (uA * (y2 - y1));
			_intersection = intersection;
			return true;
		}
		return false;
}

void ModuleCollisions::OnResetGame()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			if (colliders[i]->type == Collider::Type::BULLET || colliders[i]->type == Collider::Type::ENEMY)
				colliders[i]->pendingToDelete = true;
		}
	}
}

fPoint ModuleCollisions::GravityRotation()
{
	float cosine = cos(GravityMovement * App->GetTime());
	float sine = sin(GravityMovement * App->GetTime());
	fPoint gravity = {(float) (SATELITE_GRAVITY * cosine),(float)(SATELITE_GRAVITY * sine) };
	return gravity;
}

void ModuleCollisions::GravitationalForce(Collider* particle, int index)
{
	float gravityForce;
	Collider* ground;
	for (uint i = 0; i < MAX_COLLIDERS; i++)
	{
		
		if (colliders[i] == nullptr || colliders[i]->type != Collider::Type::WALL)
			continue;

		ground = colliders[i];

		//direction particle to the ground
		fPoint direction = { ground->position.x - particle->position.x, ground->position.y - particle->position.y };
		float directionLength = sqrt(direction.x * direction.x + direction.y * direction.y);
		fPoint vDirectionNormalized = { direction.x / directionLength, direction.y / directionLength };
		gravityForce = (GRAVITY * (particle->mass * ground->mass)) / (directionLength * directionLength);
		particle->force += {gravityForce * vDirectionNormalized.x, gravityForce* vDirectionNormalized.y};

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
	Collider* particle;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		particle = colliders[i];
		//reset all forces
		particle->force = fPoint(0, 0);

		//Gravity
		if(particle->activeGravity)
		{	
			//Gravity as the bot screen sdie is ground
			if(Normal)
				colliders[i]->force += {0,0.0005f};

			//thisfunction rotates de force of gravity as if a big palnet would rotate arround the map
			if (Satelite)
				particle->force += GravityRotation();

			//this function calculates gravitational force between particles and "grounds" aka walls
			if (Interplanetary)
				GravitationalForce(particle,i);
		}

		//Wind
		particle->force += fPoint(0,0);
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

				colliders[i]->position.x += colliders[i]->velocity.x * dt;
				colliders[i]->position.y += colliders[i]->velocity.y * dt;

				colliders[i]->deltaPosition = { colliders[i]->lastPosition.x - colliders[i]->position.x, colliders[i]->lastPosition.y - colliders[i]->position.y };
				
				colliders[i]->SetCenter();

				if (i == -1)//for debug porpuses
				{
					LOG("ACCELERATION[%i]:x:%f y:%f", i, colliders[i]->acceleration.x, colliders[i]->acceleration.y);
					LOG("VELOCITY[%i]:x:%f y:%f", i, colliders[i]->velocity.x, colliders[i]->velocity.y);
					LOG("POSITION[%i]:x:%f y:%f", i, colliders[i]->position.x, colliders[i]->position.y);
				}
			}

			//draw center of the objects for debug purposes and also look more pro
			if (colliders[i]->type == Collider::Type::WALL)
			{
				App->renderer->DrawCircle(colliders[i]->position, 2, 255, 125, 0, 255);
			}
			else {
				App->renderer->DrawCircle(colliders[i]->position, 2, 125, 255, 0, 255);
			}
		}
	}
}

bool ModuleCollisions::IsPositive(float value)
{
	if (value > 0)
		return true;
	else
		return false;
}

fPoint ModuleCollisions::StopVibration(fPoint v)
{
	if (v.x <0.005f && v.x > -0.005f)
		v.x = 0;
	if (v.y <0.005f && v.y > -0.005f)
		v.y = 0;

	return v;
}

fPoint ModuleCollisions::CapBigVelocities(fPoint v)
{
	//Control Big Velocities
	if (v.x > MAX_VELOCITY)
		v.x = MAX_VELOCITY;

	if (v.x < -MAX_VELOCITY)
		v.x = -MAX_VELOCITY;

	if (v.y > MAX_VELOCITY)
		v.y = MAX_VELOCITY;

	if (v.y < -MAX_VELOCITY)
		v.y = -MAX_VELOCITY;

	return v;
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

Collider* ModuleCollisions::AddBulletCollider(fPoint center, float radius, BulletType bulletType, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(center, radius, bulletType, listener);
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
