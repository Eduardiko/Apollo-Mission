#include "Collisions.h"
#include "App.h"
#include "Render.h"
#include "Input.h"

#include "SDL/include/SDL_scancode.h"
#include "Log.h"

Collisions::Collisions() : Module()
{
	name.Create("collisions");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		colliders[i] = nullptr;
	}

	matrix[CircleCollider::Type::PLANET][CircleCollider::Type::PLAYER] = true;

	matrix[CircleCollider::Type::PLAYER][CircleCollider::Type::PLANET] = true;


}

Collisions::~Collisions() {}

bool Collisions::PreUpdate()
{

	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			--colliderCount;
		}
	}

	CircleCollider* c1;
	CircleCollider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// Skip empty colliders
		if (colliders[i] == nullptr)
		{
			continue;
		}

		c1 = colliders[i];

		// Avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// Skip empty colliders
			if (colliders[k] == nullptr)
			{
				continue;
			}

			c2 = colliders[k];

			if (c1->Intersects(c1, c2))
			{
				/*if (matrix[c1->type][c2->type] && c1->listener)
					c1->listener->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->listener)
					c2->listener->OnCollision(c2, c1);*/
			}
		}
	}

	return true;
}

bool Collisions::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug = !debug;
	}

	return true;
}

bool Collisions::PostUpdate()
{
	if (debug)
	{
		DrawCollider();
	}

	return true;
}

void Collisions::DrawCollider()
{
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			continue;
		}

		switch (colliders[i]->type)
		{
		case CircleCollider::Type::NONE:
			app->render->DrawCircle(colliders[i]->x, colliders[i]->y, colliders[i]->radius, 0, 255, 255, alpha);
			break;
		case CircleCollider::Type::PLAYER: // Red
			app->render->DrawCircle(colliders[i]->x, colliders[i]->y, colliders[i]->radius, 255, 0, 0, alpha);
			break;
		case CircleCollider::Type::PLANET: // Yellow
			app->render->DrawCircle(colliders[i]->x, colliders[i]->y, colliders[i]->radius, 255, 255, 0, alpha);
			break;
		case CircleCollider::Type::ASTEROID: // Yellow
			app->render->DrawCircle(colliders[i]->x, colliders[i]->y, colliders[i]->radius, 255, 255, 255, alpha);
			break;
		}
	}
}

bool Collisions::CleanUp()
{
	LOG("%s", "Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			--colliderCount;
		}
	}

	return true;
}

CircleCollider* Collisions::AddCollider(int posX, int posY, int radius, CircleCollider::Type type, Module* listener = nullptr)
{
	CircleCollider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new CircleCollider(posX, posY, radius, type, listener);
			++colliderCount;
			break;
		}
	}

	return ret;
}

bool Collisions::DeleteCollider(CircleCollider* collider)
{
	if (collider != nullptr)
	{
		for (uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			if (colliders[i] == collider)
			{
				collider->pendingToDelete = true;
				break;
			}
		}
	}

	return false;
}

// Collider class
CircleCollider::CircleCollider(int posX, int posY, int radius, Type type, Module* listener = nullptr) : x(posX), y(posY), radius(radius), type(type), listener(listener) {}

double CircleCollider::DistanceBetweenPoints(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
bool CircleCollider::Intersects(CircleCollider* A, CircleCollider* B)
{
	if (DistanceBetweenPoints(A->x, A->y, B->x, B->y) < (A->radius + B->radius))
	{
		return true;
	}

	return false;
}

void CircleCollider::SetPos(int x, int y)
{
	this->x = x;
	this->y = y;
}