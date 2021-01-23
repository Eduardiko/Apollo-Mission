#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"
#include "Physics.h"

struct SDL_Texture;
class Planet;

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
	void DrawRadius();
	void GravityField();

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
	SDL_Rect moonRect;

	Planet* earth;
	Planet* mars;
	Planet* whitePlanet;
	Planet* moon;
	Planet* asteroid;

	fPoint earthiPos;
	fPoint marsiPos;
	fPoint mooniPos;
	fPoint asteroidiPos;

};

#endif // __SCENE_H__