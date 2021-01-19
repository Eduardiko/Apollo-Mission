#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Physics.h"
#include "Point.h"
#include "Math.h"

void PhysBody::AddForce(float forcex, float forcey)
{
	force.x += forcex;
	force.y += forcey;
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

// 
bool Physics::PreUpdate()
{

	return true;
}

// 
bool Physics::Update(float dt)
{

	return true;
}

// 
bool Physics::PostUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (!debug)
		return true;


	return true;
}


// Called before quitting
bool Physics::CleanUp()
{

	return true;
}

fPoint Physics::GravityForce(PhysBody b1, PhysBody b2)
{
	fPoint force;
	fPoint distance;

	distance.x = b2.position.x - b1.position.x;
	distance.y = b2.position.y - b1.position.y;

	force.x = -gravityConstant * b1.mass * b2.mass / pow(distance.x, 2);

	return force;
}