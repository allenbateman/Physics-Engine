#include "Collider.h"

Collider::Collider(SDL_Rect rectangle, Type type, Module* listener) : rect(rectangle), type(type)
{
	shape = Shape::RECTANGLE;
	position.x = rectangle.x;
	position.y = rectangle.y;
	listeners[0] = listener;
}

Collider::Collider(fPoint center, float radius, Type type, Module* listener) : position(center),radius (radius), type(type)
{
	shape = Shape::CIRCLE;
	listeners[0] = listener;
}


void Collider::CircleCollider(fPoint pos, float _radius, Type _type, Module* listener)
{
	shape = Shape::CIRCLE;
	position.x = pos.x;
	position.y = pos.y;
	radius = _radius;
	type = _type;
	listeners[0] = listener;
}

void Collider::RectangleCollider(SDL_Rect _rectangle, Type _type, Module* listener)
{
	shape = Shape::RECTANGLE;
	rect = _rectangle;
	position.x = _rectangle.x;
	position.y = _rectangle.y;
	type = _type;
	listeners[0] = listener;
}

void Collider::ChainCollider(float* vertices, Type type, Module* listener)
{
	//todo
	shape = Shape::CHAIN;
}

void Collider::SetPos(int x, int y)
{
	switch (shape)
	{
		case Shape::RECTANGLE:
			rect.x = x;
			rect.y = y;
			break;
		case Shape::CIRCLE:
			position.x = x;
			position.y = y;
			break;
		default:

			break;
	}

}

bool Collider::CircleRectangleCollision(Collider c1, Collider c2)
{
	//c1 -> being a circle 
	//c2 -> being a rectangle
	return false;
}

bool Collider::CircleCircleCollision(Collider c, Collider c2)
{
	//c1 -> being a Circle
	//c2 -> being a Circle
	return false;
}

bool Collider::RectangleRectangleCollsion(Collider c, Collider c2)
{
	//c1 -> being a rectangle
	//c2 -> being a rectangle
	return false;
}


bool Collider::Intersects(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}

void Collider::AddListener(Module* listener)
{
	for (int i = 0; i < MAX_LISTENERS; ++i)
	{
		if (listeners[i] == nullptr)
		{
			listeners[i] = listener;
			break;
		}

		//Simple security check to avoid adding the same listener twice
		else if (listeners[i] == listener)
			break;
	}
}