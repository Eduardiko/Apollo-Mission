#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"

#include "SDL/include/SDL.h"

#define MAX_FUEL 100

struct SDL_Texture;

class Player : public Module
{
public:
	Player();
	virtual ~Player();

	bool Awake(pugi::xml_node& conf);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void Respawn();
	void Die();
	float ToRadians(float angle);

	Spaceship* spaceship;
	fPoint gravForce;

	fPoint playerPos;

	SDL_Texture* spaceshipTex;
	SDL_Texture* explosionsTex;
	SDL_Rect spaceshipRect;

	//animations
	Animation* currentAnim;
	Animation idleAnim;
	Animation upAnim;
	Animation leftAnim;
	Animation rightAnim;

	SDL_Rect idle;
	SDL_Rect up;
	SDL_Rect left;
	SDL_Rect right;

	Animation explosionAnim;
	SDL_Rect expl_1;
	SDL_Rect expl_2;
	SDL_Rect expl_3;
	SDL_Rect expl_4;
	SDL_Rect expl_5;
	SDL_Rect expl_6;

	float propulsionForce;
	float angleRot;
	float forceOff;
	float brakeForce;

public:
	bool isAlive;
	float fuel;
	float fuelConsumption;
	float angle;
	bool conquredEarth, conqueredMars, conqueredCheese;
	bool won;
	bool requestedToRestart;
	bool hasDied;
	bool outOfFuel;
	
};

#endif // __PLAYER_H__