#include "Collider.h"

Collider::Collider(SDL_Rect rectangle, Type type, Module* listener, float _mass) : rect(rectangle), type(type)
{
	shape = Shape::RECTANGLE;
	position.x = rectangle.x;
	position.y = rectangle.y;
	listeners[0] = listener;
	mass = _mass;
}

Collider::Collider(fPoint center, float radius, Type type, Module* listener, float _mass) : position(center),radius (radius), type(type)
{
	shape = Shape::CIRCLE;
	listeners[0] = listener;
	mass = _mass;

}


void Collider::CircleCollider(fPoint pos, float _radius, Type _type, Module* listener, float _mass)
{
	shape = Shape::CIRCLE;
	position.x = pos.x;
	position.y = pos.y;
	radius = _radius;
	type = _type;
	listeners[0] = listener;
	mass = _mass;

}

void Collider::RectangleCollider(SDL_Rect _rectangle, Type _type, Module* listener, float _mass)
{
	shape = Shape::RECTANGLE;
	rect = _rectangle;
	position.x = _rectangle.x;
	position.y = _rectangle.y;
	mass = _mass;
	type = _type;
	listeners[0] = listener;
	mass = _mass;

}

void Collider::ChainCollider(float* vertices, Type type, Module* listener, float _mass)
{
	//todo
	mass = _mass;

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

bool Collider::CircleRectangleCollision(Collider current, Collider other)
{
	//c1 -> being a circle 
	//c2 -> being a rectangle
	return false;
}

bool Collider::CircleCircleCollision(Collider current, Collider other)
{
	//c1 -> being a Circle
	//c2 -> being a Circle
	return false;
}

bool Collider::RectangleRectangleCollsion(Collider current, Collider other)
{
	//c1 -> being a rectangle
	//c2 -> being a rectangle
	return (current.rect.x < other.rect.x + other.rect.w &&
		current.rect.x + current.rect.w > other.rect.x &&
		current.rect.y < other.rect.y + other.rect.h &&
		current.rect.h + current.rect.y > other.rect.y);
}


bool Collider::Intersects(const	Collider* other) const
{
	switch (type)
	{
		case Shape::CIRCLE:
			switch (other->shape)
			{
				case  Shape::CIRCLE:
					break;
				case  Shape::RECTANGLE:
					return (rect.x < other->rect.x + other->rect.w &&
						rect.x + rect.w > other->rect.x &&
						rect.y < other->rect.y + other->rect.h &&
						rect.h + rect.y > other->rect.y);
					break;
				default:
					break;
			}
			break;
		case Shape::RECTANGLE:
			switch (other->shape)
			{
				case  Shape::CIRCLE:
					break;
				case  Shape::RECTANGLE:
					return (rect.x < other->rect.x + other->rect.w &&
						rect.x + rect.w > other->rect.x &&
						rect.y < other->rect.y + other->rect.h &&
						rect.h + rect.y > other->rect.y);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

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