#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

struct Satelite
{
	fPoint currentPos = { 0.0f , 0.0f };
	fPoint center = { 0.0f , 0.0f };
	float radius;
	float speed;
};

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	fPoint CircularMotion(float x, float y, float radius,float dt);

private:
	SDL_Texture* backgroundTex;
	Animation backgroundAnim;

public:
	//planets rects
	SDL_Texture* planetsTex;

	Animation earthAnim;
	SDL_Rect earth1;
	SDL_Rect earth2;
	SDL_Rect earth3;
	SDL_Rect earth4;

	SDL_Rect mars;

	Satelite Moon;
	SDL_Rect moon;
	fPoint moonPos;
	SDL_Rect cheesePlanet;

	
};

#endif // __SCENE_H__