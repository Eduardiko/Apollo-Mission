//#pragma once
//#ifndef COLLISIONS_H
//#define COLLISIONS_H
//
//#define MAX_COLLIDERS 50
//
//#include "Module.h"
//#include "SDL/include/SDL_rect.h"
//
//struct CircleCollider
//{
//    enum Type
//    {
//        NONE = -1,
//        PLAYER,
//        PLANET,
//        ASTEROID,
//        MAX
//    };
//
//public:
//
//
//    CircleCollider(int posX, int posY, int radius, Type type, Module* listener);
//    double DistanceBetweenPoints(int x1, int y1, int x2, int y2);
//    bool Intersects(CircleCollider* A, CircleCollider* B);
//    void SetPos(int x, int y);
//
//    int x = 0, y = 0;
//    int radius = 0;
//    Type type;
//    bool pendingToDelete = false;
//    Module* listener = nullptr;
//};
//
//class Collisions : public Module
//{
//public:
//
//    Collisions();
//    virtual ~Collisions();
//
//    bool PreUpdate();
//
//    bool Update(float dt);
//
//    bool PostUpdate();
//
//    bool CleanUp();
//
//    CircleCollider* AddCollider(int posX, int posY, int radius, CircleCollider::Type type, Module* listener);
//
//    bool DeleteCollider(CircleCollider* collider);
//
//    void DrawCollider();
//
//    inline uint GetColliderCount() const
//    {
//        return colliderCount;
//    };
//
//    CircleCollider* collider;
//
//private:
//
//    CircleCollider* colliders[MAX_COLLIDERS] = { nullptr };
//
//    bool matrix[CircleCollider::Type::MAX][CircleCollider::Type::MAX];
//
//    bool debug = false;
//    uint colliderCount = 0;
//};
//
//#endif // MODULE_COLLISIONS_H