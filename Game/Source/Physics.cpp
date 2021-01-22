#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Physics.h"
#include "Player.h"
#include "Point.h"
#include "Math.h"

void PhysBody::AddForce(float forcex, float forcey)
{
	fPoint force;

	force.x = forcex;
	force.y = forcey;

	forcesList.Add(force);
}

void PhysBody::AddMomentum(float vx, float vy)
{
	velocity.x += vx;
	velocity.y += vy;
}

RectangleCollider::RectangleCollider(float width, float height, Type type)
{
	this->width = width;
	this->height = height;
	this->type = type;
}

RectangleCollider* Physics::AddRectangleCollider(int width, int height, RectangleCollider::Type type)
{
	RectangleCollider* ret = nullptr;

	for (uint i = 0; i < 50; ++i)
	{
		if (colliderList[i] == nullptr)
		{
			ret = colliderList[i] = new RectangleCollider(width, height,type);
			break;
		}
	}

	return ret;
}

void RectangleCollider::SetColliderPos(fPoint position)
{
	this->position = position;
}

Spaceship::Spaceship(fPoint position, float mass, int health, float fuel,float rotation)
{
	this->position = position;
	this->mass = mass;
	this->health = health;
	this->fuel = fuel;
	this->rotation = rotation;
}

Planet::Planet(fPoint position, float mass, float atmosphereRadius)
{
	this->position = position;
	this->mass = mass;
	this->atmosphereRadius = atmosphereRadius;
}

Physics::Physics() : Module()
{
}

// Destructor
Physics::~Physics()
{
}

bool Physics::Start()
{

	gravityConstant = 0.2f;

	return true;
}


bool Physics::Update(float dt)
{

	for (int i = 0; i < app->player->spaceship->forcesList.Count(); i++)
	{
		app->player->spaceship->totalForce.x += app->player->spaceship->forcesList[i].x;
		app->player->spaceship->totalForce.y += app->player->spaceship->forcesList[i].y;
	}

	app->player->spaceship->forcesList.Clear();

	app->player->spaceship->position = Verlet(*app->player->spaceship, dt);
	
	RectangleCollider* c1;
	RectangleCollider* c2;

	for (uint i = 0; i < 50; ++i)
	{
		if (colliderList[i] == nullptr)
			continue;

		c1 = colliderList[i];

		for (uint k = i + 1; k < 50; ++k)
		{
			if (colliderList[k] == nullptr)
				continue;

			c2 = colliderList[k];

			if (detectCollision(c1, c2)) solveCollision(c1, c2);
		}
	}

	return true;
}

bool Physics::PostUpdate()
{
	for (uint i = 0; i < 50; ++i)
	{
		if (colliderList[i] == nullptr)
			continue;
		SDL_Rect rect = { colliderList[i]->position.x, colliderList[i]->position.y, colliderList[i]->width, colliderList[i]->height };
		app->render->DrawRectangle(rect, 255, 255, 255, 55, true, true);
	}

	return true;
}

fPoint Physics::GravityForce(PhysBody b1, PhysBody b2)
{
	fPoint force;
	fPoint distance;
	float hypotenuse;
	float angle;
	int i;
	fPoint center;

	center.x = b2.position.x + b2.collider->width / 2;
	center.y = b2.position.y + b2.collider->height / 2;

	distance.x = b1.position.x - center.x;
	if (distance.x <= 0) i = 1;
	else if (distance.x > 0) i = -1;
	distance.y = b1.position.y - center.y;
	hypotenuse = sqrt(pow(distance.x, 2) + pow(distance.y, 2));

	angle = asin(distance.y / hypotenuse);

	force.x = i * gravityConstant * b1.mass * b2.mass * cos(angle)/ pow(hypotenuse, 2);
	force.y = -gravityConstant * b1.mass * b2.mass * sin(angle)/ pow(hypotenuse, 2);

	return force;
}

fPoint Physics::Verlet(PhysBody b, float dt)
{
	dt = 0.2f;

	if (b.totalForce.x > 50.0f) b.totalForce.x = 50.0f;
	if (b.totalForce.y > 50.0f) b.totalForce.y = 50.0f;

	b.acceleration.x = b.totalForce.x / b.mass;
	b.acceleration.y = b.totalForce.y / b.mass;

	b.velocity.x = b.velocity.x + b.acceleration.x * pow(dt, 2);
	b.velocity.y = b.velocity.y + b.acceleration.y * pow(dt, 2);

	b.position.x = b.position.x + b.velocity.x * dt + 0.5 * b.acceleration.x * pow(dt, 2);
	b.position.y = b.position.y + b.velocity.y * dt + 0.5 * b.acceleration.y * pow(dt, 2);

	return b.position;
}

bool Physics::detectCollision(RectangleCollider* c1, RectangleCollider* c2)
{
	if (c1->type != RectangleCollider::Type::SPACESHIP) return false;

	c1->min.x = c1->position.x;
	c1->min.y = c1->position.y + c1->height;
	c1->max.x = c1->position.x + c1->width;
	c1->max.y = c1->position.y;

	c2->min.x = c2->position.x;
	c2->min.y = c2->position.y + c2->height;
	c2->max.x = c2->position.x + c2->width;
	c2->max.y = c2->position.y;

	float d1x = c2->min.x - c1->max.x;
	float d1y = c2->min.y - c1->max.y;
	float d2x = c1->min.x - c2->max.x;
	float d2y = c1->min.y - c2->max.y;

	if (d1x > 0.0f || d1y < 0.0f)
		return false;

	if (d2x > 0.0f || d2y < 0.0f)
		return false;

	if (d2y > -1 && d2y < 1) direction = 1;
	if (d1y > -1 && d1y < 1) direction = 2;
	if (d1x > -1 && d1x < 1) direction = 3;
	if (d2x > -1 && d2x < 1) direction = 4;

	return true;
}

void Physics::solveCollision(RectangleCollider* c1, RectangleCollider* c2)
{
	int subs = 1;

	if (direction == 1)	app->player->spaceship->position.y -= subs;
	if (direction == 2)	app->player->spaceship->position.y += subs;
	if (direction == 3)	app->player->spaceship->position.x -= subs;
	if (direction == 4)	app->player->spaceship->position.x += subs;
}