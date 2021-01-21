#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"

#include "SDL/include/SDL.h"

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

	Spaceship* spaceship;
	fPoint gravForce;

	Spaceship* spaceship2;
	fPoint playerPos;

	fPoint playerPos2;
	SDL_Texture* spaceshipTex;
	SDL_Rect spaceshipRect;

	//animations
	SDL_Rect* currentAnim;
	SDL_Rect idle;
	SDL_Rect up;
	SDL_Rect left;
	SDL_Rect right;

	float propulsionForce;
	float angleRot;
	float forceOff;

	float pi;
};

#endif // __PLAYER_H__