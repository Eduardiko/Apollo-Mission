#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Module.h"
#include "Point.h"

class PhysBody
{
public:
	fPoint position;
	fPoint velocity;
	fPoint acceleration;
	fPoint force;
	float mass;
	float angle;

public: // Methods
	
	void AddForce(float forcex, float forcey); // f += df;
	void AddMomentum(float vx, float vy); // v += dv; // ...why would this not be conceptually correct?

};

class Spaceship : public PhysBody
{
public: // Properties
	int health; // Spaceship's hull structural integrity. You could also add another variable for energy shields.
	float fuel; // Could also be an integer, if you do not need too much precision.
	int ammo; // Number of remaining torpedoes. The method launchTorpedo() reduces this by 1.

public: // Methods
	Spaceship(fPoint position, float mass, int health, float fuel);
};

// Module --------------------------------------
class Physics : public Module
{
public:
	Physics();
	~Physics();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	fPoint GravityForce(PhysBody b1, PhysBody b2);
	//fPoint HydroDragForce();


	//void detectCollision(PhysBody b1, PhysBody b2);
	//void solveCollision();


private:
	bool debug;
	float gravityConstant;
};

#endif // __PHYSICS_H__