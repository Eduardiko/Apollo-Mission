#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Module.h"
#include "Point.h"
#include "List.h"


struct RectangleCollider
{
	enum Type
	{
		SPACESHIP,
		ASTEROID,
		EARTH,
		MARS,
		CHEESE,
		DSTAR,
		FUEL
	};
	

	fPoint position;
	fPoint min;
	fPoint max;
	float width;
	float height;
	fPoint center;
	bool pendingToDelete = false;
	Type type;

	RectangleCollider(float width, float height, Type type);
	void SetColliderPos(fPoint position, float offX, float offY);
};

class PhysBody
{
public:
	fPoint position;
	fPoint velocity;
	fPoint acceleration;
	fPoint totalForce;
	
	float mass;
	float rotation;
	float gravityRadius;
	float liftRadius;
	
	List<fPoint> forcesList;

	RectangleCollider* collider;
	
public: // Methods
	
	void AddForce(float forcex, float forcey); // f += df;
	void AddMomentum(float vx, float vy); // v += dv; // ...why would this not be conceptually correct?
};

class Spaceship : public PhysBody
{
public: // Properties
	int health; // Spaceship's hull structural integrity. You could also add another variable for energy shields.
	float fuel; // Could also be an integer, if you do not need too much precision.
	

public: // Methods
	Spaceship(fPoint position, float mass, int health, float fuel, float rotation);
};

class Planet : public PhysBody
{
public:
	float orbitalSpeed;
	

public:
	Planet(fPoint position, float mass, float gravityRadius, float liftRadius);
};

class Fuel :public PhysBody
{
public:
	bool picked;

	Fuel(float x,float y);
};


// Module --------------------------------------
class Physics : public Module
{
public:
	Physics();
	~Physics();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();

	fPoint GravityForce(PhysBody* b1, PhysBody b2);
	fPoint DragForce(PhysBody* b1);
	fPoint Verlet(PhysBody* b, float dt);

	RectangleCollider* colliderList[50] = { nullptr };
	RectangleCollider* AddRectangleCollider(int width, int height, RectangleCollider::Type type);
	bool DetectCollision(RectangleCollider* c1, RectangleCollider* c2);
	void SolveCollision(RectangleCollider* c1, RectangleCollider* c2);

	int direction;

	float d1x;
	float d2x;
	float d1y;
	float d2y;

private:
	bool debug;
	float gravityConstant;

};

#endif // __PHYSICS_H__