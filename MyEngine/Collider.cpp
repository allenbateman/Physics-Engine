#include "Collider.h"
#include "math.h"

Collider::Collider(SDL_Rect rectangle, Type type, Module* listener, float _mass) : rect(rectangle), type(type)
{
	shape = Shape::RECTANGLE;
	position.x = rectangle.x;
	position.y = rectangle.y;
	lastPosition = position;
	deltaPosition = { 0,0 };
	listeners[0] = listener;
	mass = _mass;
	collInfo = new CollisionInfo();

}

Collider::Collider(fPoint center, float radius, Type type, Module* listener, float _mass) : position(center),radius (radius), type(type)
{
	shape = Shape::CIRCLE;
	listeners[0] = listener;
	mass = _mass;
	collInfo = new CollisionInfo();


}


void Collider::CircleCollider(fPoint pos, float _radius, Type _type, Module* listener, float _mass)
{
	shape = Shape::CIRCLE;
	position.x = pos.x;
	position.y = pos.y;
	lastPosition = position;
	deltaPosition = { 0,0 };
	radius = _radius;
	type = _type;
	listeners[0] = listener;
	mass = _mass;
	collInfo = new CollisionInfo();

}

void Collider::RectangleCollider(SDL_Rect _rectangle, Type _type, Module* listener, float _mass)
{
	shape = Shape::RECTANGLE;
	rect = _rectangle;
	position.x = _rectangle.x - _rectangle.w * 0.5f;
	position.y = _rectangle.y - _rectangle.h * 0.5f;
	mass = _mass;
	type = _type;
	listeners[0] = listener;
	mass = _mass;
	collInfo = new CollisionInfo();


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

CollisionInfo* Collider::CircleRectangleCollision(const Collider* other) const
{
	//c1 -> being a circle 
	//c2 -> being a rectangle
	CollisionInfo* info = new CollisionInfo();
	info->Collided = false;
	info->collision =CollisionRecived::NONE;

	return info;
}

CollisionInfo* Collider::RectangleCircleCollision(const Collider* other) const
{
	CollisionInfo* info = new CollisionInfo();
	info->Collided = false;
	info->collision = CollisionRecived::NONE;


	return info;
}

CollisionInfo* Collider::CircleCircleCollision(const Collider* other) const
{
	//c1 -> being a Circle
	//c2 -> being a Circle
	CollisionInfo* info = new CollisionInfo();
	info->Collided = false;
	info->collision = CollisionRecived::NONE;


	 //((other->position.x - position.x)* (other->position.x - position.x) + (other->position.y - position.y) * (other->position.y - position.y) <= (radius + other->radius)* (radius + other->radius));
	 return info;
}

CollisionInfo* Collider::RectangleRectangleCollsion(const Collider* other) const
{
	//c1 -> being a rectangle
	//c2 -> being a rectangle
	//if (type == Type::WALL && other->type == Type::BULLET)
	//	LOG(" this->WALL - other->BULLET collison");
	//if (type == Type::BULLET && other->type == Type::BULLET)
	//	LOG("this->BULLET - other->BULLET collison");

	//side collsions
	other->collInfo->Collided = false;
	other->collInfo->horizontal = false;
	other->collInfo->horizontal = false;
	other->collInfo->collision = CollisionRecived::NONE;





	if (other->rect.x + other->rect.w + other->velocity.x > rect.x &&
		other->rect.x + other->velocity.x < rect.x + rect.w &&
		other->rect.y + other->rect.h  > rect.y &&
		other->rect.y < rect.y + rect.h)
	{
		other->collInfo->Collided = true;
		other->collInfo->horizontal = true;
	}

	if (other->rect.x + other->rect.w > rect.x &&
		other->rect.x < rect.x + rect.w &&
		other->rect.y + other->rect.h + other->velocity.y >rect.y &&
		other->rect.y + other->velocity.y < rect.y + rect.h)
	{
		other->collInfo->Collided = true;
		other->collInfo->vertical = true;
	}

	//if (other->rect.x + other->rect.w + other->velocity.x > rect.x &&
	//	other->rect.x + other->velocity.x < rect.x + rect.w &&
	//	other->rect.y + other->rect.h  > rect.y &&
	//	other->rect.y + other->velocity.y < rect.h)
	//{
	//	other->collInfo->Collided = true;
	//	other->collInfo->horizontal = true;
	//}
	//	 
	//if (other->rect.x + other->rect.w > rect.x &&
	//	other->rect.x < rect.x + rect.w && 
	//	other->rect.y + other->rect.h + other->velocity.y >rect.y &&
	//	other->rect.y + other->velocity.y < rect.y + rect.h)
	//{
	//	other->collInfo->Collided = true;
	//	other->collInfo->vertical = true;
	//}


	return other->collInfo;
}

void Collider::SetPosition()
{
	switch(shape)
	{
		case RECTANGLE:
			rect.x = position.x - rect.w * 0.5f;
			rect.y = position.y - rect.h * 0.5f;
			break;
		case CIRCLE:
			//Nothing since the center is alredy the pos
			break;
		default :
			break;
	}
}

CollisionInfo* Collider::Intersects(const	Collider* other) const
{

	switch (type)
	{
		case Shape::CIRCLE:
			switch (other->shape)
			{
				case  Shape::CIRCLE:
					LOG("circle -> circle ");
					return CircleCircleCollision(other);
					break;
				case  Shape::RECTANGLE:
					LOG("circle -> rect ");
					return CircleRectangleCollision(other);
					break;
				default:
					break;
			}
			break;
		case Shape::RECTANGLE:
			switch (other->shape)
			{
				case  Shape::CIRCLE:
					LOG("rect -> circle ");
					return RectangleCircleCollision(other);
					break;
				case  Shape::RECTANGLE:
					//LOG("rect -> rect ");
					return RectangleRectangleCollsion(other);
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