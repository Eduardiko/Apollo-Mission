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
	LOG("%d", app->win->GetScale());
	counter = 0;
	pickedFuel = false;
	EarthHaveBeenConquered = MarsaveBeenConquered = false;
	uiTex = app->tex->Load("Assets/Textures/ui.png");


	popUp.PushBack({ 0, 6, 206, 90 });
	popUp.PushBack({ 221, 0, 206, 90 });

	fuel.PushBack({ 82,122,60,70 });
	fuel.PushBack({ 141,122,60,70 });

	lowFuel.PushBack({ 141,122,60,70 });
	lowFuel.PushBack({ 206,122,60,70 });


	
	conquered.PushBack({ 0,227,238,105});
	conquered.PushBack({ 238,227,238,105 });
	conquered.PushBack({ 0,227,238,105 });
	conquered.PushBack({ 238,227,238,105 });
	conquered.PushBack({ 0,0,0,0 });
	conquered.loop = false;
	

	fuelIcon.PushBack({ 315,143,40,45 });
	fuelIcon.PushBack({ 356,143,40,45 });

	win.PushBack({ 0,347,227,250 });
	win.PushBack({ 240,347,227,250 });

	turnOff.PushBack({ 0,0,0,0 });

	spacebar.PushBack({ 0,585,258,135 });
	spacebar.PushBack({ 0,720,258,135 });

	hearth_1 = { 32,866,260,45 };
	hearth_2 = { 32,912,120,45 };
	hearth_3 = { 32,958,183,45 };
	hearth_4 = { 32,1004,260,45 };

	popUpAnim = &popUp;
	fuelIconAnim = &fuelIcon;
	spacebarAnim = &turnOff;
	//winAnim = &turnOff;
	winAnim = &win;
	
	return true;
}

// Called each loop iteration
bool UI::PreUpdate()
{
	
	return true;
}

bool UI::Update(float dt)
{
	switch (app->player->lives)
	{
	case(1):
	{
		app->render->DrawTexture(uiTex, 10, 25, &hearth_1, 1.0f);
		break;
	}
	case(2):
	{
		app->render->DrawTexture(uiTex, 10, 25, &hearth_2, 1.0f);
		break;
	}
	case(3):
	{
		app->render->DrawTexture(uiTex, 10, 25, &hearth_3, 1.0f);
		break;
	}
	case(4):
	{
		app->render->DrawTexture(uiTex, 10, 25, &hearth_4, 1.0f);
		break;
	}
	

	}

	if(counter > 350)
		popUpAnim = &turnOff;
	
	if(app->player->isAlive)
	{
		if (app->player->fuel > 50)
		{
			fuelAnim = &fuel;
		}
		else
		{
			fuelAnim = &lowFuel;
		}
	}

	if (app->player->hasDied || app->player->outOfFuel)
	{
		spacebarAnim = &spacebar;

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			app->player->Respawn();
		}
	}
	else
		spacebarAnim = &turnOff;

	if ((app->player->conquredEarth || app->player->conqueredMars || app->player->conqueredCheese) && !app->player->won)
	{
		conqueredAnim = &conquered;
		conqueredAnim->Update(dt);
		SDL_Rect rect = conqueredAnim->GetCurrentFrame();
		app->render->DrawTexture(uiTex, 427, 250, &rect, 1.0f);
		
	}



	if (pickedFuel)
	{
		fuelIconAnim = &turnOff;
	}
	

	popUpAnim->Update(dt);
	SDL_Rect rect = popUpAnim->GetCurrentFrame();
	app->render->DrawTexture(uiTex, 380,220, &rect, 1.1f);

	fuelAnim->Update(dt);
	rect = fuelAnim->GetCurrentFrame();
	app->render->DrawTexture(uiTex, 10+260, 10, &rect, 1.0f);

	//fuel icon
	fuelIconAnim->Update(dt);
	rect = fuelIconAnim->GetCurrentFrame();
	app->render->DrawTexture(uiTex, app->scene->fuel_1->position.x, app->scene->fuel_1->position.y, &rect, 1.0f);

	spacebarAnim->Update(dt);
	rect = spacebarAnim->GetCurrentFrame();
	app->render->DrawTexture(uiTex,427 + 20,250, &rect, 1.0f);

	if (app->player->won)
	{
		

		winAnim->Update(dt);
		rect = winAnim->GetCurrentFrame();
		app->render->DrawTexture(uiTex, 300+127, 250, &rect, 1.0f);
		
	}

	UpdateFuel();
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

void UI::UpdateFuel()
{
	
	if (pickedFuel == true)
	{
		app->player->fuel = MAX_FUEL;
	}
	if (app->player->fuel > 0)
	{
		SDL_Rect rect = { 90+260,35,app->player->fuel * 2.5f,30 };
		if (app->player->fuel > 45)
			app->render->DrawRectangle(rect, 0, 255, 0, 255);
		else if (app->player->fuel < 45 && app->player->fuel > 15)
			app->render->DrawRectangle(rect, 255, 255, 0, 255);
		else
			app->render->DrawRectangle(rect, 255, 0, 0, 255);
	}
}

void UI::WinGame()
{
	app->player->won = true;
	//LOG("Win!");
	app->audio->PlayFx(app->audio->winFx);
	

}