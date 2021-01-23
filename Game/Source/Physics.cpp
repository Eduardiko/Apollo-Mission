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

void RectangleCollider::SetColliderPos(fPoint position, float offX, float offY)
{
	this->position.x = position.x + offX;
	this->position.y = position.y + offY;
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

	gravityConstant = 10000.0f;

	debug = false;

	return true;
}


bool Physics::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) debug = !debug;

	for (int i = 0; i < app->player->spaceship->forcesList.Count(); i++)
	{
		app->player->spaceship->totalForce.x += app->player->spaceship->forcesList[i].x;
		app->player->spaceship->totalForce.y += app->player->spaceship->forcesList[i].y;
	}

	app->player->spaceship->forcesList.Clear();
		
	RectangleCollider* c1;
	RectangleCollider* c2;

	for (uint i = 0; i < 50; ++i)
	{
		if (colliderList[i] == nullptr)
			continue;

		c1 = colliderList[i];
		c1->center.x = c1->position.x + c1->width / 2;
		c1->center.y = c1->position.y + c1->height / 2;

		for (uint k = i + 1; k < 50; ++k)
		{
			if (colliderList[k] == nullptr)
				continue;

			c2 = colliderList[k];
			c2->center.x = c2->position.x + c2->width / 2;
			c2->center.y = c2->position.y + c2->height / 2;

			if(DetectCollision(c1, c2)) SolveCollision(c1, c2);
		}
	}

	app->player->spaceship->position = Verlet(*app->player->spaceship, dt);

	return true;
}

bool Physics::PostUpdate()
{
	if (debug)
	{
		for (uint i = 0; i < 50; ++i)
		{
			if (colliderList[i] == nullptr)
				continue;
			SDL_Rect rect = { colliderList[i]->position.x, colliderList[i]->position.y, colliderList[i]->width, colliderList[i]->height };
			app->render->DrawRectangle(rect, 255, 255, 255, 55, true, true);
		}
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

	distance.x = b1.collider->center.x - b2.collider->center.x;
	if (distance.x <= 0) i = 1;
	else if (distance.x > 0) i = -1;
	distance.y = b1.collider->center.y - b2.collider->center.y;
	hypotenuse = sqrt(pow(distance.x, 2) + pow(distance.y, 2));

	angle = asin(distance.y / hypotenuse);

	force.x = i * gravityConstant * b1.mass * b2.mass * cos(angle)/ pow(hypotenuse, 2);
	force.y = -gravityConstant * b1.mass * b2.mass * sin(angle)/ pow(hypotenuse, 2);

	if (force.x > 1000.0f) force.x = 1000.0f;
	if (force.x < -1000.0f) force.x = -1000.0f;
	if (force.y > 1000.0f) force.y = 1000.0f;
	if (force.y < -1000.0f) force.y = -1000.0f;

	return force;
}

fPoint Physics::Verlet(PhysBody b, float dt)
{
	
	b.acceleration.x = b.totalForce.x / b.mass;
	b.acceleration.y = b.totalForce.y / b.mass;

	b.velocity.x = b.velocity.x + b.acceleration.x * pow(dt, 2);
	b.velocity.y = b.velocity.y + b.acceleration.y * pow(dt, 2);

	b.position.x = b.position.x + b.velocity.x * dt + 0.5 * b.acceleration.x * pow(dt, 2);
	b.position.y = b.position.y + b.velocity.y * dt + 0.5 * b.acceleration.y * pow(dt, 2);

	return b.position;
}

bool Physics::DetectCollision(RectangleCollider* c1, RectangleCollider* c2)
{
	RectangleCollider* holdC;

	if (c2->type == RectangleCollider::Type::PLANET && c1->type == RectangleCollider::Type::PLANET) return false;

	if (c2->type == RectangleCollider::Type::SPACESHIP && c1->type == RectangleCollider::Type::PLANET)
	{
		holdC = c2;
		c2 = c1;
		c1 = holdC;
	}

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

	if (d2y > -3 && d2y < 3) direction = 1;
	if (d1y > -3 && d1y < 3) direction = 2;
	if (d1x > -3 && d1x < 3) direction = 3;
	if (d2x > -3 && d2x < 3) direction = 4;

	return true;
}

void Physics::SolveCollision(RectangleCollider* c1, RectangleCollider* c2)
{
	int subs = 1;

	if (direction == 1)
	{
		app->player->spaceship->position.y -= subs;
		app->player->spaceship->totalForce.y *= -0.2f;
		app->player->spaceship->totalForce.x *= 0.2f;
	}
	if (direction == 2)
	{
		app->player->spaceship->position.y += subs;
		app->player->spaceship->totalForce.y *= -0.2f;
		app->player->spaceship->totalForce.x *= 0.2f;
	}
	if (direction == 3)
	{
		app->player->spaceship->position.x -= subs;
		app->player->spaceship->totalForce.x *= -0.2f;
		app->player->spaceship->totalForce.y *= 0.2f;
	}
	if (direction == 4)
	{
		app->player->spaceship->position.x += subs;
		app->player->spaceship->totalForce.x *= -0.2f;
		app->player->spaceship->totalForce.y *= 0.2f;
	}
}
