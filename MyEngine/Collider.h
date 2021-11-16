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
		BALL,
		PLAYER,
		BULLET,
		BOUNDS,
		PARTICLE,
		MAX
	};
	enum Shape {
		RECTANGLE,
		CIRCLE,
		CHAIN
	};

	//Methods
	Collider(SDL_Rect rectangle, Type type, Module* listener = nullptr);

	void CircleCollider(iPoint pos,float radius, Type type, Module* listener = nullptr);

	void RectangleCollider(SDL_Rect rctangle, Type type, Module* listener = nullptr);

	void ChainCollider(float* vertices, Type type, Module* listener = nullptr);

	void SetPos(int x, int y);

	bool CircleRectangleCollision();
	bool CircleCircleCollision();
	bool RectangleRectangleCollsion();

	iPoint GetPos() { return iPoint {rect.x,rect.y}; }
	
	bool Intersects(const SDL_Rect& r) const;

	void AddListener(Module* listener);

	//Variables
	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Shape shape;
	Module* listeners[MAX_LISTENERS] = { nullptr };
};


#endif // !__COLLIDER_H__

