#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Player.h"
#include "UI.h"

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
	
	isAlive = true;
	fuel = MAX_FUEL;
	fuelConsumption = 1.0f / 10;
	won = false;
	conqueredMars = conquredEarth = false;
	spaceshipTex = app->tex->Load("Assets/Textures/spaceship.png");
	explosionsTex = spaceshipTex;

	playerPos = { 100.0f, 300.0f };
	spaceshipRect = { 0,0,17,43 };

	spaceship = new Spaceship(playerPos, 10.0f, 2, 100.0f,0.0f);
	spaceship->collider = app->physics->AddRectangleCollider(25, 25, RectangleCollider::Type::SPACESHIP);

	propulsionForce = 2000.0f;
	angleRot = 4.0f;

	gravForce.x = 0.0f;
	gravForce.y = 0.0f;

	//animations rects
	idle = { 0,0,17,43 };
	up = { 17,0,17,43 };
	left = { 37,0,19,43 };
	right = { 65, 0, 19, 43 };

	idleAnim.PushBack(idle);
	idleAnim.loop = false;
	upAnim.PushBack(up);
	leftAnim.PushBack(left);
	rightAnim.PushBack(right);
	
	expl_1 = { 99,0,40,60 };
	expl_2 = { 158,0,40,60 };
	expl_3 = {228,0,40,60 };
	expl_4 = { 294,0,40,60 };
	expl_5 = { 359,0,40,60 };
	expl_1 = { 425,0,40,60 };

	explosionAnim.PushBack(expl_1);
	explosionAnim.PushBack(expl_2);
	explosionAnim.PushBack(expl_3);
	explosionAnim.PushBack(expl_4);
	explosionAnim.PushBack(expl_5);
	explosionAnim.PushBack(expl_6);
	explosionAnim.loop = false;

	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	fuel -= fuelConsumption;
	//LOG("fuel : %f", fuel);
	if (conqueredMars && conquredEarth)
		won = true;

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		LOG("Respawning spaceship");
		Respawn();
	}
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->ui->WinGame();
	}
	


	if (!isAlive)
	{
		currentAnim->Update(dt);
		currentAnim = &explosionAnim;
	}

	if (!isAlive && currentAnim->HasFinished())
	{
		explosionAnim.Reset();
		isAlive = true;
		spaceship->position = playerPos;
	}

	
	if (isAlive)
	{

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{

			fPoint f = { 0.0f, 0.0f };

			float angle = ToAngles(spaceship->rotation);


			if (angle >= 0 && angle <= 90)
			{
				//LOG("1st quadrant");
				f.x = abs((sin(angle) * propulsionForce));
				f.y = -abs((cos(angle) * propulsionForce));

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
			if (angle > 270 && angle < 360)
			{
				//LOG("4th quadrant");
				f.x = -abs((sin(angle) * propulsionForce));
				f.y = -abs((cos(angle) * propulsionForce));

			}

			spaceship->AddForce(f.x, f.y);


			currentAnim = &upAnim;


			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{

				spaceship->rotation += angleRot;
				currentAnim = &rightAnim;
			}

			else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{

				spaceship->rotation -= angleRot;
				currentAnim = &leftAnim;
			}

		}

		else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			float forceBreak = 1700.0f;

			if (spaceship->totalForce.x < 0.0f) spaceship->totalForce.x += forceBreak;
			if (spaceship->totalForce.x > 0.0f) spaceship->totalForce.x -= forceBreak;
			if (spaceship->totalForce.y < 0.0f) spaceship->totalForce.y += forceBreak;
			if (spaceship->totalForce.y > 0.0f) spaceship->totalForce.y -= forceBreak;
		}

		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{

			spaceship->rotation -= angleRot;
			currentAnim = &leftAnim;
		}

		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{

			spaceship->rotation += angleRot;
			currentAnim = &rightAnim;
		}

		else
		{
			currentAnim = &idleAnim;
		}

	}


	if (spaceship->position.x + spaceship->collider->width > 1200.0f)
	{
		spaceship->position.x = 1200.0f - spaceship->collider->width;
		spaceship->totalForce.x *= -0.2f;
	}
	if (spaceship->position.x < 0)
	{
		spaceship->position.x = 0;
		spaceship->totalForce.x *= -0.2f;
	}
	if (spaceship->position.y + spaceship->collider->height > 700)
	{
		spaceship->position.y = 700 - spaceship->collider->height;
		spaceship->totalForce.y *= -0.2f;
	}
	if (spaceship->position.y < 0)
	{
		spaceship->position.y = 0;
		spaceship->totalForce.y *= -0.2f;
	}

	spaceship->collider->SetColliderPos(spaceship->position, -4.0f, 0.0f);

	currentAnim->Update(dt);
	
	return true;
}

bool Player::PostUpdate()
{
	bool ret = true;


	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	//LOG("Rotation : %f", spaceship->rotation);
	SDL_Rect rect = currentAnim->GetCurrentFrame();
	
	app->scene->GravityField();

	app->render->DrawTexture(spaceshipTex, spaceship->position.x, spaceship->position.y, &rect, 1.0f, spaceship->rotation, 8, 14);

	return ret;
}

bool Player::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void Player::Respawn()
{
	if (isAlive)
	{
		isAlive = false;
		app->audio->PlayFx(app->audio->respawnFx);
		
		//Reset forces
		spaceship->totalForce.x = spaceship->totalForce.y = 0;
		spaceship->rotation = 0;
	}
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