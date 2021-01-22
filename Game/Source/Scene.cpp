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
	planetsTex = app->tex->Load("Assets/Textures/planets.png");
	
	
	earth1 = { 117 , 0 , 96 , 96 };
	earth2= { 227 , 0 , 96 , 96 };
	earth3 = { 336 , 0 , 96 , 96 };
	earth4 = { 458 , 0 , 96 , 96 };
	
	earthAnim.PushBack(earth1);
	earthAnim.PushBack(earth2);
	earthAnim.PushBack(earth3);
	earthAnim.PushBack(earth4);

	mars = { 359 ,232,90,90 };

	moonRect = { 108 , 253,30,30 };
	moonPos = { 300.0f,300.0f };
	moon = new Planet(moonPos, 5.0f, 50);
	moon->orbitalSpeed = 0.0f;

	for (int i = 0; i < 4; i++)
	{
		backgroundAnim.PushBack({ i * 64,0,64,64 });
	}

	backgroundAnim.loop = true;
	backgroundAnim.speed = 10.0f;


	app->audio->explosionFx = app->audio->LoadFx("Assets/Audio/Fx/explosionFx.wav");
	app->audio->respawnFx = app->audio->LoadFx("Assets/Audio/Fx/respawnFx.wav");
	app->audio->radioFx = app->audio->LoadFx("Assets/Audio/Fx/radioFx.wav");
	app->audio->jetFx = app->audio->LoadFx("Assets/Audio/Fx/jetFx.wav");


	app->audio->PlayFx(app->audio->respawnFx);
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

	backgroundAnim.Update(dt);

	earthAnim.Update(0.012f/20);

	moonPos = CircularMotion(300, 300, 110, dt);

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		//LOG("Background parallax (?)");
		return false;

	}
	SDL_Rect rect = backgroundAnim.GetCurrentFrame();

	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			app->render->DrawTexture(backgroundTex, i * 64, j * 64, &rect);
		}
	}

	//earth
	SDL_Rect _rect = earthAnim.GetCurrentFrame();
	app->render->DrawTexture(planetsTex, 300, 300, &_rect, 0.1f);
	//mars
	app->render->DrawTexture(planetsTex, 700, 600, &mars);
	
	app->render->DrawTexture(planetsTex, moonPos.x, moonPos.y, &moonRect);
	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

		return true;
}

fPoint Scene::CircularMotion(float x, float y, float radius,float dt)
{
	float s = 1.0f/500;
	moon->orbitalSpeed += dt*s;

	fPoint p = { 0.0f , 0.0f };

	p.x = ((x+48) + cos(moon->orbitalSpeed) * radius);
	p.y = (y+48) + sin(moon->orbitalSpeed) * radius;

	return p;
}
