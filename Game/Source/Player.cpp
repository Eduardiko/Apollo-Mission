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
	float pi = 3.1416;

	spaceshipTex = app->tex->Load("Assets/Textures/spaceship.png");
	playerPos = { 100.0f, 300.0f };
	spaceshipRect = { 0,0,17,43 };

	spaceship = new Spaceship(playerPos, 10.0f, 2, 100.0f,0.0f);

	playerPos2 = { 500.0f, 400.0f };
	spaceship2 = new Spaceship(playerPos2, 10.0f, 2, 100.0f,0.0f);

	propulsionForce = 0.002f;
	angleRot = 4.0f;
	forceOff = 14.0f;

	

	//animations rects
	idle=  { 0,0,17,43 };
	up = { 17,0,17,43 };
	left= { 37,0,19,43 };
	right= { 65, 0, 19, 43 };

	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		LOG("Respawning spaceship");
		Respawn();
	}

	LOG("Rotation : %d", spaceship->rotation);

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		//spaceship->AddForce(0.0f, -propulsionForce * forceOff);

		//spaceship->AddForce((sin(spaceship->rotation)*propulsionForce)*forceOff, -(cos(spaceship->rotation)*propulsionForce) * forceOff);
		fPoint f = { 0.0f, 0.0f };
		float rad = spaceship->rotation * pi / 180;

		f.x = (sin(spaceship->rotation) * propulsionForce) * forceOff;
		f.y= (cos(spaceship->rotation) * propulsionForce) * forceOff;

		//if (rad >= 0 && rad <= 90)
		//{
		//	f.x = spaceship->position.x * cos(rad) * propulsionForce;
		//	f.y = spaceship->position.y * sin(rad) * propulsionForce;
		//}

		//// second quadrant
		//if (rad > 90 && rad <= 180)
		//{
		//	f.x = spaceship->position.x * cos(rad) * propulsionForce;
		//	f.y = spaceship->position.y * -sin(rad) * propulsionForce;
		//}

		//// third quadrant
		//if (rad > 180 && rad <= 270)
		//{
		//	f.x = spaceship->position.x * -cos(rad) * propulsionForce;
		//	f.y = spaceship->position.y * -sin(rad) * propulsionForce;
		//}

		//// fourth quadrant
		//if (rad > 270 && rad <= 360)
		//{
		//	f.x = spaceship->position.x * -cos(rad) * propulsionForce;
		//	f.y = spaceship->position.y * sin(rad) * propulsionForce;
		//}

		spaceship->AddForce(f.x, f.y);
		currentAnim = &up;
		
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		//spaceship->AddForce(-propulsionForce * forceOff, 0.0f);
	
		spaceship->rotation -= angleRot/40;
		currentAnim = &left;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		spaceship->AddForce(0.0f, propulsionForce * forceOff);
	
		currentAnim = &idle;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		//spaceship->AddForce(propulsionForce * forceOff, 0.0f);
		
		spaceship->rotation += angleRot/40;
		currentAnim = &right;
	}
	else
	{
		currentAnim = &idle;
	}

	spaceship->AddForce(gravForce.x, gravForce.y);

	return true;
}

bool Player::PostUpdate()
{
	bool ret = true;


	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	gravForce = app->physics->GravityForce(*spaceship, *spaceship2);

	app->render->DrawTexture(spaceshipTex, spaceship->position.x, spaceship->position.y, currentAnim,1.0f,spaceship->rotation);
	app->render->DrawTexture(spaceshipTex, spaceship2->position.x, spaceship2->position.y, &spaceshipRect);

	return ret;
}

bool Player::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void Player::Respawn()
{
	spaceship->position = playerPos;
	//Reset forces
	spaceship->totalForce.x = spaceship->totalForce.y = 0;
	spaceship->rotation = 0;
}
