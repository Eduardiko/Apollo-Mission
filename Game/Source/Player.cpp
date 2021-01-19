#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"

#include "Log.h"

Player::Player()
{
	name.Create("player");
}

Player::~Player()
{

}

bool Player::Awake(pugi::xml_node&)
{
	

	return true;
}

bool Player::Start()
{

	spaceshipTex = app->tex->Load("Assets/Textures/rocket.png");
	playerPos = { 100.0f, 350.0f };
	spaceshipRect = { 0,0,600,600 };

	spaceship = new Spaceship(playerPos, 10.0f, 2, 100.0f);

	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{



	return true;
}

bool Player::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->render->DrawTexture(spaceshipTex, spaceship->position.x, spaceship->position.y, &spaceshipRect);

	return ret;
}

bool Player::CleanUp()
{
	LOG("Freeing scene");

	return true;
}