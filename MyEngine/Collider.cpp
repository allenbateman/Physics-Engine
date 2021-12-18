#include "Collider.h"
#include "math.h"

//rectangle constructor
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
	activeGravity = true;
	Bounce = true;
	switch (type)
	{
	case PLAYER:
		color.r = 52;
		color.g = 235;
		color.b = 58;
		color.a = 255;
		break;
	case ENEMY:
		color.r = 235;
		color.g = 52;
		color.b = 52;
		color.a = 255;
		break;
	case WALL:
		mass = 10000;
		color.r = 52;
		color.g = 52;
		color.b = 235;
		color.a = 255;
		break;
	}
}
//circle constructor
Collider::Collider(fPoint center, float radius, Type type, Module* listener, float _mass) : position(center),radius (radius), type(type)
{
	shape = Shape::CIRCLE;
	listeners[0] = listener;
	collInfo = new CollisionInfo();
	mass = _mass;
	lastPosition = position;
	activeGravity = true;
	Bounce = true;
	switch (type)
	{
	case PLAYER:
		color.r = 52;
		color.g = 235;
		color.b = 58;
		color.a = 255;
		break;
	case ENEMY:
		color.r = 235;
		color.g = 52;
		color.b = 52;
		color.a = 255;
		break;
	case WALL:
		mass = 10000;
		color.r = 52;
		color.g = 52;
		color.b = 235;
		color.a = 255;
		break;
	}

}
//bullet Constructor
Collider::Collider(fPoint center, float radius, BulletType bulletType, Module* listener, float _mass) : position(center), radius(radius)
{
	type = BULLET;
	shape = CIRCLE;
	collInfo = new CollisionInfo();
	switch (bulletType)
	{
	case BOMB:
		bulletProperties.type = BOMB;
		bulletProperties.velocity = 0.1f;
		activeGravity = true;
		Bounce = true;
		friction = 0.6f;
		coeficientOfRestitution = 0.7f;
		mass = 1;
		listeners[0] = listener;
		color.r = 152;
		color.g = 52;
		color.b = 235;
		color.a = 255;
		break;
	case LASER:
		bulletProperties.type = LASER;
		bulletProperties.velocity = 1;
		activeGravity = false;
		Bounce = true;
		friction = 0;
		coeficientOfRestitution = 0;
		mass = 1;
		listeners[0] = listener;
		color.r = 235;
		color.g = 235;
		color.b = 52;
		color.a = 255;
		break;
	case BOUNCER:
		bulletProperties.type = BOUNCER;
		bulletProperties.velocity = 0.5f;
		bulletProperties.bounceCounter = 3;
		activeGravity = true;
		Bounce = true;
		friction = 0.8f;
		coeficientOfRestitution = 0.8f;
		mass = 1;
		listeners[0] = listener;
		color.r = 235;
		color.g = 82;
		color.b = 52;
		color.a = 255;
		break;
	default:
		break;
	}
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
}

CollisionInfo* Collider::CircleRectangleCollision(const Collider* other) const
{
	//other being a rectangle
	other->collInfo->Collided = false;
	other->collInfo->Bot = false;
	other->collInfo->Top = false;
	other->collInfo->Left = false;
	other->collInfo->Right = false;

	float testX = position.x;
	float testY = position.y;

	if (position.x < other->rect.x)							//left?
	{
		testX = other->rect.x;
		other->collInfo->Right = true;	//the rectangle has a collision on the right
	}
	else if (position.x > other->rect.x + other->rect.w)	//right?
	{
		testX = other->rect.x + other->rect.w;
		other->collInfo->Left = true;	//the rectangle has a collision on the left
	}

	if (position.y < other->rect.y)							//top?
	{
		testY = other->rect.y;
		other->collInfo->Bot = true;	//the rectangle has a collision on the bot
	}
	else if (position.y > other->rect.y + other->rect.h)	//bot?
	{
		testY = other->rect.y + other->rect.h;
		other->collInfo->Top = true;	//the rectangle has a collision on the bot
	}


	float distX = position.x - testX;
	float distY = position.y - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	if (distance <= radius) {
		other->collInfo->Collided = true;
	}


	return other->collInfo;
}

CollisionInfo* Collider::RectangleCircleCollision(const Collider* other) const
{
	//other being a circle
	other->collInfo->Collided = false;
	other->collInfo->Bot = false;
	other->collInfo->Top = false;
	other->collInfo->Left = false;
	other->collInfo->Right = false;


	float testX = other->position.x;
	float testY = other->position.y;

	if (other->position.x < rect.x)							//left?
	{
		testX = rect.x;
		other->collInfo->Right = true; //the circle has a collision on the right
	}
	else if (other->position.x > rect.x + rect.w)			//right?
	{
		testX = rect.x + rect.w;
		other->collInfo->Left = true; //the circle has a collision on the left
	}

	if (other->position.y < rect.y)							//top?
	{	
		testY = rect.y;
		other->collInfo->Bot = true; //the circle has a collision on the bot
	}
	else if (other->position.y > rect.y + rect.h)			//bot?
	{
		testY = rect.y + rect.h;
		other->collInfo->Top = true; //the circle has a collision on the top
	}


	float distX = other->position.x - testX;
	float distY = other->position.y - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	if (distance <= other->radius) {
		other->collInfo->Collided = true;
	}


	return other->collInfo;
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
			other->collInfo->Left = true;

		}else if (distance.x < 0)
		{
			//left
			LOG("C->Left");
			other->collInfo->Right = true;
		}
		if (distance.y > 0)
		{
			//bot
			LOG("C->BoT");
			other->collInfo->Top = true;

		}else if (distance.y < 0)
		{
			//top
			LOG("C->Top");
			other->collInfo->Bot = true;
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
					//LOG("circle -> circle ");
					return CircleCircleCollision(other);
					break;
				case  Shape::RECTANGLE:
					//LOG("circle -> rect ");
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
					//LOG("rect -> circle ");
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