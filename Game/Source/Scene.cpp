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
	
	
	earthRect = { 117 , 0 , 100 , 96 };
	whitePlanetRect = { 336 , 0 , 96 , 96 };
	marsRect = { 359 ,232, 72, 72 };
	moonRect = { 108 , 253,30,30 };
	asteroidRect = { 192,253,27,27 };

	earthiPos = { 300.0f, 300.0f };
	earth = new Planet(earthiPos, 200.0f, 200.0f);
	earth->collider = app->physics->AddRectangleCollider(84, 80, RectangleCollider::Type::PLANET);
	planetList.Add(*earth);

	marsiPos = { 700.0f, 600.0f };
	mars = new Planet(marsiPos, 100.0f, 130.0f);
	mars->collider = app->physics->AddRectangleCollider(60, 60, RectangleCollider::Type::PLANET);
	planetList.Add(*mars);

	mooniPos = { 300.0f,300.0f };
	moon = new Planet(mooniPos, 20.0f, 1.0f);
	moon->collider = app->physics->AddRectangleCollider(24, 24, RectangleCollider::Type::ASTEROID);
	planetList.Add(*moon);
	
	asteroidiPos = { 700.0f, 600.0f };
	asteroid = new Planet(asteroidiPos, 0.0f, 0.0f);
	asteroid->collider = app->physics->AddRectangleCollider(23, 23, RectangleCollider::ASTEROID);
	planetList.Add(*asteroid);

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

	moon->position = CircularMotion(earth->collider->center.x, earth->collider->center.y, earth->atmosphereRadius, 0.2f, moon, dt);
	asteroid->position = CircularMotion(mars->collider->center.x, mars->collider->center.y, mars->atmosphereRadius, 1.0f, asteroid, dt);

	earth->collider->SetColliderPos(earth->position, 8.0f, 8.0f);
	mars->collider->SetColliderPos(mars->position, 6.0f, 6.0f);
	moon->collider->SetColliderPos(moon->position, 2.0f, 2.0f);
	asteroid->collider->SetColliderPos(asteroid->position, 2.0f, 2.0f);

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{

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

	app->render->DrawTexture(planetsTex, earth->position.x, earth->position.x, &earthRect);
	app->render->DrawTexture(planetsTex, mars->position.x, mars->position.y, &marsRect);
	app->render->DrawTexture(planetsTex, moon->position.x, moon->position.y, &moonRect);
	app->render->DrawTexture(planetsTex, asteroid->position.x, asteroid->position.y, &asteroidRect);

	DrawRadius();


	app->ui->Update(app->dt);

	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

		return true;
}

fPoint Scene::CircularMotion(float x, float y, float radius, float speed, Planet* planet, float dt)
{
	planet->orbitalSpeed += dt*speed;

	fPoint p = { 0.0f , 0.0f };

	p.x = x - planet->collider->width / 2 + cos(planet->orbitalSpeed) * radius;
	p.y = y - planet->collider->height / 2 + sin(planet->orbitalSpeed) * radius;

	return p;
}

void Scene::DrawRadius()
{

	for (int i = 0; i < planetList.Count(); i++)
	{
		app->render->DrawCircle(planetList[i].collider->center.x, planetList[i].collider->center.y, planetList[i].atmosphereRadius, 255, 255, 255, 50);
	}
}

void Scene::GravityField()
{
	for (int i = 0; i < planetList.Count(); i++)
	{
		float distancex;
		float distancey;
		distancex = abs(app->player->spaceship->collider->center.x - planetList[i].collider->center.x);
		distancey = abs(app->player->spaceship->collider->center.y - planetList[i].collider->center.y);

		float distance;
		distance = sqrt(pow(distancex, 2) + pow(distancey, 2));

		if (distance < planetList[i].atmosphereRadius)
		{
			fPoint force;
			force = app->physics->GravityForce(*app->player->spaceship, planetList[i]);
			app->player->spaceship->AddForce(force.x, force.y);
		}
	}
}