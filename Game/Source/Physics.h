#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Module.h"
#include "Point.h"
#include "Collisions.h"
#include "List.h"

class PhysBody
{
public:
	fPoint position;
	fPoint velocity;
	fPoint acceleration;
	fPoint totalForce;
	
	float mass;
	float rotation;
	
	List<fPoint> forcesList;

	//CircleCollider* collider;

public: // Methods
	
	void AddForce(float forcex, float forcey); // f += df;
	void AddMomentum(float vx, float vy); // v += dv; // ...why would this not be conceptually correct?
	//void OnCollision(CircleCollider* c1, CircleCollider* c2);

};

class Spaceship : public PhysBody
{
public: // Properties
	int health; // Spaceship's hull structural integrity. You could also add another variable for energy shields.
	float fuel; // Could also be an integer, if you do not need too much precision.
	int ammo; // Number of remaining torpedoes. The method launchTorpedo() reduces this by 1.

public: // Methods
	Spaceship(fPoint position, float mass, int health, float fuel,float rotation);
};

// Module --------------------------------------
class Physics : public Module
{
public:
	Physics();
	~Physics();

	bool Start();
	bool Update(float dt);

	fPoint GravityForce(PhysBody b1, PhysBody b2);
	fPoint Verlet(PhysBody b, float dt);
	//fPoint HydroDragForce();


	//void detectCollision(PhysBody b1, PhysBody b2);
	//void solveCollision();


private:
	bool debug;
	float gravityConstant;
};

#endif // __PHYSICS_H__