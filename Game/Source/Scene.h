#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"
#include "Physics.h"

struct SDL_Texture;
class Planet;
class Fuel;



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

	fPoint CircularMotion(float x, float y, float radius, float speed, Planet* planet, float dt);
	fPoint CometMotion(float x, float y, float speed);
	void DrawRadius();
	void FieldCheck();
	void RespawnComet();

private:
	SDL_Texture* backgroundTex;
	Animation backgroundAnim;

public:
	//planets rects
	List<PhysBody> planetList;
	SDL_Texture* planetsTex;

	SDL_Rect earthRect;
	SDL_Rect whitePlanetRect;
	SDL_Rect marsRect;
	SDL_Rect cheeseRect;
	SDL_Rect moonRect;
	SDL_Rect asteroidRect;
	SDL_Rect deathstarRect;

	Planet* earth;
	Planet* mars;
	Planet* cheesePlanet;
	Planet* whitePlanet;
	Planet* moon;
	Planet* asteroid;
	Planet* comet;
	Planet* deathStar;

	fPoint earthiPos;
	fPoint marsiPos;
	fPoint cheeseiPos;
	fPoint mooniPos;
	fPoint asteroidiPos;
	fPoint cometiPos;
	fPoint deathStariPos;

	Fuel* fuel_1;

private:
	Animation* cometAnim;
	Animation _comet;	
	unsigned int cometCounter;
public: 
	bool gameIsOver;

};

#endif // __SCENE_H__