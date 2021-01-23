#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Player.h"
#include "UI.h"

#include "Defs.h"
#include "Log.h"

#define VSYNC true

struct Animation;

UI::UI() : Module()
{
	
}

// Destructor
UI::~UI()
{}

// Called before render is available
bool UI::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

// Called before the first frame
bool UI::Start()
{
	LOG("UI start");
	counter = 0;

	uiTex = app->tex->Load("Assets/Textures/ui.png");


	popUp.PushBack({ 0, 6, 206, 90 });
	popUp.PushBack({ 221, 0, 206, 90 });

	fuel.PushBack({ 82,122,60,70 });
	fuel.PushBack({ 141,122,60,70 });

	turnOff.PushBack({ 0,0,0,0 });

	popUpAnim = &popUp;
	
	return true;
}

// Called each loop iteration
bool UI::PreUpdate()
{
	
	return true;
}

bool UI::Update(float dt)
{
	if(counter > 350)
		popUpAnim = &turnOff;
	
	if(app->player->isAlive)
	{
		if (app->player->fuel > 0)
		{
			fuelAnim = &fuel;
		}
		else
		{
			fuelAnim = &lowFuel;
		}
	}
	

	popUpAnim->Update(dt);
	SDL_Rect rect = popUpAnim->GetCurrentFrame();
	app->render->DrawTexture(uiTex, 380,220, &rect, 1.1f);

	fuelAnim->Update(dt);
	rect = fuelAnim->GetCurrentFrame();
	app->render->DrawTexture(uiTex, 10, 10, &rect, 1.0f);

	counter++;
	return true;
}

bool UI::PostUpdate()
{
	
	return true;
}

// Called before quitting
bool UI::CleanUp()
{
	
	return true;
}
