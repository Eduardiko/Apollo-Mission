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
	

	spaceshipTex = app->tex->Load("Assets/Textures/spaceship.png");
	playerPos = { 100.0f, 300.0f };
	spaceshipRect = { 0,0,17,43 };

	spaceship = new Spaceship(playerPos, 10.0f, 2, 100.0f,0.0f);

	playerPos2 = { 500.0f, 400.0f };
	spaceship2 = new Spaceship(playerPos2, 10.0f, 2, 100.0f,0.0f);

	propulsionForce = 0.002f*5;
	angleRot = 4.0f;
	brakeForce = propulsionForce / 3;

	

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

	

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		
		fPoint f = { 0.0f, 0.0f };
		
		float angle = ToAngles(spaceship->rotation);


		if (angle >= 0 && angle <= 90)
		{
			//LOG("1st quadrant");
			f.x = abs( (sin(angle) * propulsionForce) );
			f.y = -abs( (cos(angle) * propulsionForce) );
			
		}
		if (angle > 90 && angle < 180)
		{
			//LOG("2nd quadrant");
			f.x = abs((sin(angle) * propulsionForce));
			f.y = abs((cos(angle) * propulsionForce));

		}
		if (angle > 180 && angle < 270)
		{
			//LOG("3rd quadrant");
			f.x = -abs((sin(angle) * propulsionForce));
			f.y = abs((cos(angle) * propulsionForce));

		}
		if (angle > 270  && angle < 360)
		{
			//LOG("4th quadrant");
			f.x = -abs((sin(angle) * propulsionForce));
			f.y = -abs((cos(angle) * propulsionForce));

		}
		
		spaceship->AddForce(f.x, f.y);
		
		
		currentAnim = &up;


		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{

			spaceship->rotation += angleRot / 40;
			currentAnim = &right;
		}

		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{

			spaceship->rotation -= angleRot / 40;
			currentAnim = &left;
		}
		
	}

	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		//LOG("Current force : %f   ,    %f", spaceship->totalForce.x , spaceship->totalForce.y);
		fPoint f = { 0.0f , 0.0f };
		float angle = ToAngles(spaceship->rotation);

		int f_x= spaceship->totalForce.x * brakeForce;
		int f_y = spaceship->totalForce.y * brakeForce;
		

		spaceship->AddForce(-f_x, -f_y);
	}

	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
	
		spaceship->rotation -= angleRot/40;
		currentAnim = &left;
	}

	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		
		spaceship->rotation += angleRot / 40;
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

	//gravForce = app->physics->GravityForce(*spaceship, *spaceship2);
	//LOG("Rotation : %f", spaceship->rotation);
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
	app->audio->PlayFx(app->audio->respawnFx);
	spaceship->position = playerPos;
	//Reset forces
	spaceship->totalForce.x = spaceship->totalForce.y = 0;
	spaceship->rotation = 0;
}

float Player::ToAngles(float rot)
{
	int degrees = rot;
	
	 degrees = degrees % 360;

	 if (degrees < 0)
	 {
		 degrees += 360;
	 }

	 return degrees;
}