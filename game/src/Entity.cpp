#include "Entity.h"

constexpr Vector2 GRAVITY = { 0.0f, 100.0f };

void EntityUpdate(Entity& entity, float dt)
{
    Vector2 acc = entity.force * entity.inverse_mass;
    acc += GRAVITY * entity.gravity_scale;

    entity.vel += acc * dt;
    entity.pos += entity.vel * dt;

    entity.force = Vector2Zeros;
}
