#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "SDL/include/SDL_Rect.h"
#include "p2Point.h"
#define MAX_LISTENERS 5

class Module;

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

	bool CircleRectangleCollision( const Collider* other) const;
	bool RectangleCircleCollision( const Collider* other) const;

	bool CircleCircleCollision(const Collider* other) const;

	bool RectangleRectangleCollsion(const Collider* other) const;

	void SetPosition();

	//fPoint GetPos() { return fPoint {rect.x,rect.y}; }
	
	// const-> so it will not affect the object variables
	bool Intersects( const Collider* other) const;

	void AddListener(Module* listener);

	//Variables

	//square
	SDL_Rect rect;
	
	//circle
	float radius;

	fPoint position;
	fPoint velocity;
	fPoint acceleration;
	fPoint force;


	bool pendingToDelete = false;
	float mass;
	Type type;
	Shape shape;
	Module* listeners[MAX_LISTENERS] = { nullptr };
};


#endif // !__COLLIDER_H__

