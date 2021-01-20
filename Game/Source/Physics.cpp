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

Spaceship::Spaceship(fPoint position, float mass, int health, float fuel)
{
	this->position = position;
	this->mass = mass;
	this->health = health;
	this->fuel = fuel;
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

	distance.x = b1.position.x - b2.position.x;
	distance.y = b1.position.y - b2.position.y;

	int i, j;
	if (distance.x < 0) i = 1;
	else i = -1;

	if (distance.y < 0)
	{
		j = 1;
	}
	else j = -1;

	force.x = 0.01 * b1.mass * b2.mass * i / pow(distance.x, 2);
	force.y = 0.01 * b1.mass * b2.mass * j / pow(distance.y, 2);

	if (distance.x == 0) 
		force.x = 0;
	if (distance.y == 0) 
		force.y = 0;

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