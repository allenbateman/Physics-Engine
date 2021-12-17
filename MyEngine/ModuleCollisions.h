#ifndef __MODULE_COLLISIONS_H__
#define __MODULE_COLLISIONS_H__

#define MAX_COLLIDERS 25
#define GRAVITY 1
#include "Module.h"
#include "Collider.h"

class ModuleCollisions : public Module
{
public:
	// Constructor
	// Fills all collision matrix data
	ModuleCollisions(Application* app, bool start_enabled = true);

	// Destructor
	~ModuleCollisions();
	update_status PreUpdate();
	update_status Update(float dt);
	update_status PostUpdate();

	// Removes all existing colliders
	bool CleanUp();
	void RemovePendingToDeleteColliders();

	void CheckCollisions();

	//Apply forces
	void ApplyForces();
	void ApplyMovement(float dt);

	// Adds a new collider to the list
	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);

	Collider* AddCircleCollider(fPoint center, float radius, Collider::Type type, Module* listener = nullptr);
	Collider* AddRectangleCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);
	//Collider* AddChainCollider(iPoint center,float[] chain, Collider::Type type, Module* listener = nullptr);

	void RemoveCollider(Collider* collider);
	void DebugDraw();
	void OnCollision(Collider* body1, Collider* body2);
	void CheckParticleInBounds();
	bool IsPositive(float value);
	fPoint StopVibration(fPoint v);
	fPoint CapBigVelocities(fPoint v);
private:
	Collider* colliders[MAX_COLLIDERS] = { nullptr };
	bool matrix[Collider::Type::MAX][Collider::Type::MAX];
	bool debug = false;
};

#endif // __MODULE_COLLISIONS_H__