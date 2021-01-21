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

Spaceship::Spaceship(fPoint position, float mass, int health, float fuel,float rotation)
{
	this->position = position;
	this->mass = mass;
	this->health = health;
	this->fuel = fuel;
	this->rotation = rotation;
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

	gravityConstant = 6.674f * pow(10, -11);

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

	return true;
}

fPoint Physics::GravityForce(PhysBody b1, PhysBody b2)
{
	fPoint force;
	fPoint distance;
	float hypotenuse;
	float angle;
	int i;

	distance.x = b1.position.x - b2.position.x;
	if (distance.x <= 0) i = 1;
	else if (distance.x > 0) i = -1;
	distance.y = b1.position.y - b2.position.y;
	hypotenuse = sqrt(pow(distance.x, 2) + pow(distance.y, 2));

	angle = asin(distance.y / hypotenuse);

	force.x = i * 0.5 * b1.mass * b2.mass * cos(angle)/ pow(hypotenuse, 2);
	force.y = -0.5 * b1.mass * b2.mass * sin(angle)/ pow(hypotenuse, 2);

	return force;
}

fPoint Physics::Verlet(PhysBody b, float dt)
{
	dt = 0.2f;

	b.acceleration.x = b.totalForce.x / b.mass;
	b.acceleration.y = b.totalForce.y / b.mass;

	b.velocity.x = b.velocity.x + b.acceleration.x * pow(dt, 2);
	b.velocity.y = b.velocity.y + b.acceleration.y * pow(dt, 2);

	b.position.x = b.position.x + b.velocity.x * dt + 0.5 * b.acceleration.x * pow(dt, 2);
	b.position.y = b.position.y + b.velocity.y * dt + 0.5 * b.acceleration.y * pow(dt, 2);

	return b.position;
}