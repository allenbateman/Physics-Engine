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
	collInfo = new CollisionInfo();
	mass = _mass;
	lastPosition = position;
}


void Collider::CircleCollider(fPoint pos, float _radius, Type _type, Module* listener, float _mass)
{
	shape = Shape::CIRCLE;
	position.x = pos.x;
	position.y = pos.y;
	radius = _radius;
	type = _type;	
	mass = _mass;
	listeners[0] = listener;
	collInfo = new CollisionInfo();

	rect.w = radius * 2;
	rect.h = radius * 2;

}

void Collider::RectangleCollider(SDL_Rect _rectangle, Type _type, Module* listener, float _mass)
{
	shape = Shape::RECTANGLE;
	rect = _rectangle;
	position.x = _rectangle.x - _rectangle.w * 0.5f;
	position.y = _rectangle.y - _rectangle.h * 0.5f;
	type = _type;
	mass = _mass;
	listeners[0] = listener;
	collInfo = new CollisionInfo();


}

void Collider::ChainCollider(float* vertices, Type type, Module* listener, float _mass)
{
	//todo
	mass = _mass;

	shape = Shape::CHAIN;
}

CollisionInfo* Collider::CircleRectangleCollision(const Collider* other) const
{
	//other being a circle
	CollisionInfo* info = new CollisionInfo();
	info->Collided = false;

	float testX = position.x;
	float testY = position.y;

	if (position.x < other->rect.x)							//left?
		testX = other->rect.x; 
	else if (position.x > other->rect.x + other->rect.w)	//right?
		testX = other->rect.x + other->rect.w;

	if (position.y < other->rect.y)							//top?
		testY = other->rect.y;
	else if (position.y > other->rect.y + other->rect.h)	//bot?
		testY = other->rect.y + other->rect.h;


	float distX = position.x - testX;
	float distY = position.y - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	if (distance <= radius) {
		info->Collided = true;
	}


	return info;
}

CollisionInfo* Collider::RectangleCircleCollision(const Collider* other) const
{
	//other being a rectangle
	CollisionInfo* info = new CollisionInfo();
	info->Collided = false;

	float testX = other->position.x;
	float testY = other->position.y;

	if (other->position.x < rect.x)							//left?
		testX = rect.x;
	else if (other->position.x >rect.x + rect.w)			//right?
		testX = rect.x + rect.w;

	if (other->position.y < rect.y)							//top?
		testY = rect.y;
	else if (other->position.y > rect.y + rect.h)			//bot?
		testY = rect.y + rect.h;


	float distX = other->position.x - testX;
	float distY = other->position.y - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	if (distance <= other->radius) {
		info->Collided = true;
	}


	return info;

	return info;
}

CollisionInfo* Collider::CircleCircleCollision(const Collider* other) const
{
	//c1 -> being a Circle
	//c2 -> being a Circle
	CollisionInfo* info = new CollisionInfo();
	info->Collided = false;

	fPoint distance;
	distance.x = other->position.x - position.x;
	distance.y = other->position.y - position.y;

	if (distance.x*distance.x + distance.y*distance.y <= (radius + other->radius) * (radius + other->radius)) {
		fPoint v1 = { velocity.x - velocity.x, velocity.y - velocity.y };
		fPoint v2 = { other->velocity.x - velocity.x, other->velocity.y - velocity.y };

		info->Collided = true;
	}
	
	 return info;
}

CollisionInfo* Collider::RectangleRectangleCollsion(const Collider* other) const
{
	//side collsions
	other->collInfo->Collided = false;
	other->collInfo->Left = false;
	other->collInfo->Right = false;
	other->collInfo->Top= false;
	other->collInfo->Bot = false;

	//Position is the center of the obj
	fPoint distance;
	distance.x = other->position.x - position.x;
	distance.y = other->position.y - position.y;


	if (other->rect.x + other->rect.w  > rect.x &&
		other->rect.x < rect.x + rect.w &&
		other->rect.y + other->rect.h  > rect.y &&
		other->rect.y < rect.y + rect.h)
	{
		other->collInfo->Collided = true;
		if (distance.x > 0)
		{
			//right
			LOG("C->Right");
			other->collInfo->Right = false;
		}
		if (distance.x < 0)
		{
			//left
			LOG("C->Left");
			other->collInfo->Left = false;
		}
		if (distance.y > 0)
		{
			//bot
			LOG("C->BoT");
			other->collInfo->Bot = false;
		}
		if (distance.y < 0)
		{
			//top
			LOG("C->Top");
			other->collInfo->Top = false;
		}

	}

	return other->collInfo;
}


CollisionInfo* Collider::Intersects(const	Collider* other) const
{

	switch (shape)
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

void Collider::SetPosition(int x, int y)
{
	switch (shape)
	{
	case Shape::RECTANGLE:
		rect.x = x;
		rect.y = y;
		rect.x = position.x - rect.w * 0.5f;
		rect.y = position.y - rect.h * 0.5f;
		break;
	case Shape::CIRCLE:
		position.x = x;
		position.y = y;
		break;
	default:

		break;
	}

}

void Collider::SetCenter()
{
	switch (shape)
	{
	case RECTANGLE:
		rect.x = position.x - rect.w * 0.5f;
		rect.y = position.y - rect.h * 0.5f;
		break;
	case CIRCLE:
		//Nothing since the center is alredy the pos
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