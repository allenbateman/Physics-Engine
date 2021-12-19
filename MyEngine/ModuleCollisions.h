#ifndef __MODULE_COLLISIONS_H__
#define __MODULE_COLLISIONS_H__

#define MAX_COLLIDERS 350
#define GRAVITY 0.0005f//normal gravity
#define SATELITE_GRAVITY 0.0001f // roational gravity
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
	
	//draw
	void DebugDraw();

	// Removes all existing colliders
	bool CleanUp();
	void RemovePendingToDeleteColliders();
	void RemoveCollider(Collider* collider);


	//Apply forces
	void ApplyForces();
	void ApplyMovement(float dt);

	//Spawnners
	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);
	Collider* AddCircleCollider(fPoint center, float radius, Collider::Type type, Module* listener = nullptr);
	Collider* AddRectangleCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);
	Collider* AddBulletCollider(fPoint center, float radius, BulletType bulletType, Module* listener = nullptr);

	//collision Handelers
	void CheckCollisions();
	void OnCollision(Collider* body1, Collider* body2);
	void CheckParticleInBounds();
	void CheckLineCollisionsWithRectangles(fPoint start, fPoint end, fPoint& _intersection);
	bool LineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, fPoint& _intersection);

	//Game Functions
	void OnResetGame();

	//Helpers
	bool IsPositive(float value);
	fPoint StopVibration(fPoint v);
	fPoint CapBigVelocities(fPoint v);

	//gavitational functions
	fPoint GravityRotation();
	void GravitationalForce(Collider* body, int index );

	//gavitational Setters
	bool NormalGravity() { return Normal = !Normal; };
	bool StateliteGravity() { return Satelite = !Satelite; };
	bool InterplanetaryGravity() { return Interplanetary = !Interplanetary; };

	bool debug = false;
private:
	Collider* colliders[MAX_COLLIDERS] = { nullptr };
	bool matrix[Collider::Type::MAX][Collider::Type::MAX];



	bool Normal = false;
	bool Satelite = false;
	bool Interplanetary = true;

	//speed of the moving satelite
	float GravityMovement = 0.1f;
};

#endif // __MODULE_COLLISIONS_H__