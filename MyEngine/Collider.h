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
	Collider(fPoint positon,float radius, Type type, Module* listener = nullptr);

	void CircleCollider(fPoint pos,float _radius, Type _type, Module* listener = nullptr);

	void RectangleCollider(SDL_Rect _rctangle, Type _type, Module* listener = nullptr);

	void ChainCollider(float* _vertices, Type _type, Module* listener = nullptr);

	void SetPos(int x, int y);

	bool CircleRectangleCollision(Collider c1, Collider c2);

	bool CircleCircleCollision(Collider c1, Collider c2);

	bool RectangleRectangleCollsion(Collider c1, Collider c2);

	iPoint GetPos() { return iPoint {rect.x,rect.y}; }
	
	bool Intersects(const SDL_Rect& r) const;

	void AddListener(Module* listener);

	//Variables

	//square
	SDL_Rect rect;
	
	//circle
	float radius;
	fPoint position;
	bool pendingToDelete = false;
	Type type;
	Shape shape;
	Module* listeners[MAX_LISTENERS] = { nullptr };
};


#endif // !__COLLIDER_H__

