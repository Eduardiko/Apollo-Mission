#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{

	backgroundTex = app->tex->Load("Assets/Textures/background.png");

	for (int i = 0; i < 4; i++)
	{
		backgroundAnim.PushBack({ i * 64,0,64,64 });
	}

	backgroundAnim.loop = true;
	backgroundAnim.speed = 10.0f;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	dt = 0.2f;

	backgroundAnim.Update(dt);

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return false;;

	SDL_Rect rect = backgroundAnim.GetCurrentFrame();

	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			app->render->DrawTexture(backgroundTex, i * 64, j * 64, &rect);
		}
	}

	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

		return true;
}
