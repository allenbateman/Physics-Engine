#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "SDL/include/SDL_Rect.h"
#include "p2Point.h"
#define MAX_LISTENERS 5
#define MAX_VELOCITY 1.5


class Module;

struct CollisionInfo {
	bool Left = false;
	bool Right = false;
	bool Top = false;
	bool Bot = false;
	bool Collided = false;
	bool Bounds = false;
};
struct Color {
	float r, g, b, a;
};
enum BulletType
{
	BOMB,		//explodes on impact
	LASER,		//pure elastic vs metal
	BOUNCER		//bounces 3 times and explodes on the last hit
};
struct BulletProperties {
	class Application;

	float lifeTime;
	int bounceCounter;
	int damage;
	float velocity;
	BulletType type;
//	SDL_Texture* texture;
};
struct Collider
{
	enum Shape {
		RECTANGLE,
		CIRCLE,
		CHAIN
	};
	enum Type
	{
		NONE = -1,
		WALL,
		PLAYER,
		BULLET,
		ENEMY,
		MAX
	};

	//Constructors
	Collider(SDL_Rect rectangle, Type type, Module* listener = nullptr, float _mass = 1);
	Collider(fPoint center,float radius, Type type, Module* listener = nullptr,float _mass = 1);
	Collider(fPoint center,float radius, BulletType bulletType, Module* listener = nullptr,float _mass = 1);

	//Funcitons to create shapes
	void CircleCollider(fPoint pos,float _radius, Type _type, Module* listener = nullptr,  float _mass = 1);
	void RectangleCollider(SDL_Rect _rctangle, Type _type, Module* listener = nullptr, float _mass = 1);
	void ChainCollider(float* _vertices, Type _type, Module* listener = nullptr, float _mass = 1);

	//To call the desired Collision Solver
	CollisionInfo* Intersects(const Collider* other) const;
	//Collision Solvers
	CollisionInfo* CircleRectangleCollision( const Collider* other) const;
	CollisionInfo* RectangleCircleCollision( const Collider* other) const;
	CollisionInfo* CircleCircleCollision(const Collider* other) const;
	CollisionInfo* RectangleRectangleCollsion(const Collider* other)const;

	void AddListener(Module* listener);
	void SetPosition(int x, int y);
	void SetCenter();

	//Force Modifiers
	void ResetForce() { force = fPoint(0, 0); }
	void ApplyGravity() { force.y += 0.000005f; }
	void ApplyHorizontalFriction() {force.x -= friction;}
	void ApplyVerticalFriction() {force.x -= coeficientOfRestitution;}
	void ApplyExternalForce(fPoint externalForce) { force += externalForce; }
	
	//Variables
	//square
	SDL_Rect rect;
	
	//circle
	float radius;

	//Sum of all forces
	fPoint force;

	//Movement Components
	fPoint acceleration;
	fPoint velocity;
	fPoint position;
	fPoint lastPosition;
	fPoint deltaPosition;

	//collision info 
	CollisionInfo* collInfo;
	bool pendingToDelete = false;

	//particle properties
	Shape shape;
	Type type;
	Module* listeners[MAX_LISTENERS] = { nullptr };

	float mass;
	float friction = 0.9;
	float coeficientOfRestitution = 0.9;

	bool activeGravity;
	bool Bounce;

	Color color;

	//if collider is a bullet
	BulletProperties bulletProperties;
};


#endif // !__COLLIDER_H__

