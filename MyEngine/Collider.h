#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "SDL/include/SDL_Rect.h"
#include "p2Point.h"
#define MAX_LISTENERS 5

class Module;

enum CollisionRecived
{


	NONE,
	TOP,
	BOT,
	LEFT,
	RIGHT,
	HORIZONTAL,
	VERTICAL
};

struct CollisionInfo {
	CollisionRecived collision;
	bool vertical = false;
	bool horizontal = false;
	bool Collided = false;
};


struct Collider
{
	enum Type
	{
		NONE = -1,
		WALL,
		PLAYER,
		BULLET,
		MAX
	};
	enum Shape {
		RECTANGLE,
		CIRCLE,
		CHAIN
	};

	//Methods
	Collider(SDL_Rect rectangle, Type type, Module* listener = nullptr, float _mass = 1);
	Collider(fPoint positon,float radius, Type type, Module* listener = nullptr,float _mass = 1);

	void CircleCollider(fPoint pos,float _radius, Type _type, Module* listener = nullptr,  float _mass = 1);

	void RectangleCollider(SDL_Rect _rctangle, Type _type, Module* listener = nullptr, float _mass = 1);

	void ChainCollider(float* _vertices, Type _type, Module* listener = nullptr, float _mass = 1);

	void SetPos(int x, int y);

	CollisionInfo* CircleRectangleCollision( const Collider* other) const;
	CollisionInfo* RectangleCircleCollision( const Collider* other) const;

	CollisionInfo* CircleCircleCollision(const Collider* other) const;

	CollisionInfo* RectangleRectangleCollsion(const Collider* other)const;

	void SetPosition();

	//fPoint GetPos() { return fPoint {rect.x,rect.y}; }
	
	// const-> so it will not affect the object variables
	CollisionInfo* Intersects( const Collider* other) const;

	void AddListener(Module* listener);



	//modyfiers

	void ResetForce() { force = fPoint(0, 0); }
	void ApplyGravity() { force.y += 0.000005f; }
	void ApplyHorizontalFriction() {force.x -= horizontalFriction;}
	void ApplyVerticalFriction() {force.x -= verticalFriction;}
	void ApplyExternalForce(fPoint externalForce) { force += externalForce; }
	//Variables

	//square
	SDL_Rect rect;
	
	//circle
	float radius;

	fPoint position;
	fPoint lastPosition;
	fPoint deltaPosition;
	fPoint velocity;
	fPoint acceleration;
	fPoint force;

	CollisionInfo* collInfo;

	bool pendingToDelete = false;
	float mass;
	Type type;
	Shape shape;
	Module* listeners[MAX_LISTENERS] = { nullptr };

	float horizontalFriction = 0.01;
	float verticalFriction = 0.01;

	bool activeGravity = true;
};


#endif // !__COLLIDER_H__

