#ifndef __UI_H__
#define __UI_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"

#include "SDL/include/SDL.h"

struct SDL_Texture;
struct Animation;

class UI : public Module
{
public:

	UI();

	// Destructor
	virtual ~UI();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
	void UpdateFuel();
	void WinGame();

public:
	SDL_Texture* uiTex;

	Animation* fuelAnim;
	Animation fuel;
	Animation lowFuel;

	Animation* popUpAnim;
	Animation popUp;

	Animation* conqueredAnim;
	Animation conquered;

	Animation* fuelIconAnim;
	Animation fuelIcon;

	Animation* winAnim;
	Animation win;

	Animation* spacebarAnim;
	Animation spacebar;

	Animation* hearthAnim;
	Animation hearth;

	Animation turnOff;
public:
	int counter;
public:
	bool pickedFuel;

	bool EarthHaveBeenConquered;
	bool MarsaveBeenConquered;
};

#endif // __RENDER_H__