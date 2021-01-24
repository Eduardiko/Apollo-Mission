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

#define M_PI 3.14159265358979323846f

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
	requestedToRestart = hasDied = outOfFuel=false;
	fuel = MAX_FUEL;
	fuelConsumption = 1.0f / 10;
	won = false;
	conqueredMars = conquredEarth = false;
	spaceshipTex = app->tex->Load("Assets/Textures/spaceship.png");
	explosionsTex = spaceshipTex;

	playerPos = { 100.0f, 300.0f };
	spaceshipRect = { 0,0,17,43 };

	spaceship = new Spaceship(playerPos, 10.0f, 3, 100.0f,0.0f);
	spaceship->collider = app->physics->AddRectangleCollider(25, 25, RectangleCollider::Type::SPACESHIP);
	spaceship->rotation = 0.0f;
	angle = 0.0f;

	propulsionForce = 500.0f;
	accumulatedForce = { 0.0f, 0.0f };
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
	explosionAnim.speed = 7.0f;
	explosionAnim.loop = false;

	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	
	if (fuel <= 0)
		outOfFuel = true;

	if (hasDied )
	{
		currentAnim->Update(dt);
		currentAnim = &explosionAnim;

		if(app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			requestedToRestart = true;
	}
	
	if (!hasDied)
	{
		if (conqueredMars && conquredEarth &&  conqueredCheese)
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

		if (!isAlive )
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

		if (fuel <= 0)
			currentAnim = &idleAnim;

		if (isAlive && fuel >= 0)
		{

			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
			{
				spaceship->totalForce -= accumulatedForce;
				accumulatedForce = { 0.0f, 0.0f };
			}

			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{

				fuel -= fuelConsumption;
				currentAnim = &upAnim;

				fPoint f = { 0.0f, 0.0f };

				angle = ToRadians(spaceship->rotation);

				f.x = sin(angle) * propulsionForce;
				f.y = -cos(angle) * propulsionForce;

				accumulatedForce.x += f.x;
				accumulatedForce.y += f.y;
				spaceship->AddForce(accumulatedForce.x, accumulatedForce.y);

				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				{
					fuel -= fuelConsumption / 2;
					currentAnim = &rightAnim;
					spaceship->rotation += angleRot;
					
				}
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				{
					fuel -= fuelConsumption / 2;
					currentAnim = &leftAnim;
					spaceship->rotation -= angleRot;
					
				}

			} else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				spaceship->rotation -= angleRot;
				currentAnim = &leftAnim;
			} else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				spaceship->rotation += angleRot;
				currentAnim = &rightAnim;
			} else
			{
				currentAnim = &idleAnim;
			}
			
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				float forceBreak = 3.0f;

				if (spaceship->velocity.x < 0.0f) spaceship->velocity.x += forceBreak;
				if (spaceship->velocity.x > 0.0f) spaceship->velocity.x -= forceBreak;
				if (spaceship->velocity.y < 0.0f) spaceship->velocity.y += forceBreak;
				if (spaceship->velocity.y > 0.0f) spaceship->velocity.y -= forceBreak;
			}

		}

		//Map Limits
		if (spaceship->position.x + spaceship->collider->width > 1200.0f)
		{
			spaceship->position.x = 1200.0f - spaceship->collider->width;
			spaceship->velocity.x *= -0.2f;
		}
		if (spaceship->position.x < 0)
		{
			spaceship->position.x = 0;
			spaceship->velocity.x *= -0.2f;
		}
		if (spaceship->position.y + spaceship->collider->height > 700)
		{
			spaceship->position.y = 700 - spaceship->collider->height;
			spaceship->velocity.y *= -0.2f;
		}
		if (spaceship->position.y < 0)
		{
			spaceship->position.y = 0;
			spaceship->velocity.y *= -0.2f;
		}

		spaceship->collider->SetColliderPos(spaceship->position, -4.0f, 0.0f);

		currentAnim->Update(dt);
	}
	
	return true;
}

bool Player::PostUpdate()
{
	bool ret = true;


	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	//LOG("Rotation : %f", spaceship->rotation);
	SDL_Rect rect = currentAnim->GetCurrentFrame();
	
	app->scene->FieldCheck();
	if(currentAnim==&explosionAnim)
		app->render->DrawTexture(spaceshipTex, spaceship->position.x-10, spaceship->position.y-7, &rect, 1.0f, spaceship->rotation, 8, 14);
	else
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
	hasDied = false;
	if (isAlive)
	{
		isAlive = false;
		fuel = MAX_FUEL;
		conquredEarth = conqueredMars = false;
		won = false;
		app->audio->PlayFx(app->audio->respawnFx);

		app->ui->counter = 0;
		app->ui->popUpAnim = &app->ui->popUp;
		
		//Reset forces
		spaceship->totalForce.x = spaceship->totalForce.y = 0;
		spaceship->rotation = 0;
	}
}


void Player::Die()
{

}

float Player::ToRadians(float angle)
{
	float radians = angle;

	radians = (radians * M_PI)/180;

	return radians;
}