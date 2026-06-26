#pragma once

struct Entity;
using CollisionCallback = void(*)(Entity* self, Entity* other);
using DestroyCallback = void(*)(Entity* self, std::vector<Entity>* collection);

struct Entity
{
    Vector2 pos;
    Vector2 vel;
    Vector2 force;

    float inverse_mass;
    float gravity_scale;

    float restitution;
    float friction;

    Collider collider;
    int bounces;

    CollisionCallback on_collision;
    DestroyCallback on_destroy;

    bool destroy;
    Color color;
};

struct EntityHit
{
    Entity* a = nullptr;
    Entity* b = nullptr;
    Vector2 mtv = Vector2Zeros;
};

void EntityUpdate(Entity& entity, float dt);
void EntityApplyCollisionImpulse(Entity& a, Entity& b, Vector2 mtv);
void EntityApplyCollisionMtv(Entity& a, Entity& b, Vector2 mtv);

inline bool EntityIsMassInfinite(const Entity& e)
{
    bool inf = e.inverse_mass <= EPSILON;
    return inf;
}
