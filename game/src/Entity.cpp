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

void EntityApplyCollisionImpulse(Entity& a/*dynamic*/, Entity& b/*static*/, Vector2 mtv)
{
    float inverse_mass_sum = a.inverse_mass + b.inverse_mass;
    assert(inverse_mass_sum > EPSILON);

    Vector2 normal = Vector2Normalize(mtv);
    Vector2 vel_rel = a.vel - b.vel;
    float proj_vel_norm = Vector2DotProduct(vel_rel, normal);

    // Objects already separating (fails if vel_rel is pointing the wrong way, textbook is incorrect)
    if (proj_vel_norm > 0.0f) return;

    float e = fminf(a.restitution, b.restitution);
    float j = (-(1.0f + e) * proj_vel_norm) / inverse_mass_sum;

    Vector2 impulse = normal * j;
    a.vel += impulse * a.inverse_mass;
    b.vel -= impulse * b.inverse_mass;

    Vector2 tangent = Vector2Normalize(Vector2Tangent(vel_rel, normal));

    float jt = -Vector2DotProduct(vel_rel, tangent) / inverse_mass_sum;
    float friction = sqrtf(a.friction * b.friction);
    jt = Clamp(jt, -j * friction, j * friction);

    Vector2 tangent_impulse = tangent * jt;

    a.vel += tangent_impulse * a.inverse_mass;
    b.vel -= tangent_impulse * b.inverse_mass;
}

void EntityApplyCollisionMtv(Entity& a/*dynamic*/, Entity& b/*static*/, Vector2 mtv)
{
    if (EntityIsMassInfinite(b))
    {
        a.pos += mtv;
    }
    else
    {
        a.pos += mtv * 0.5f;
        b.pos -= mtv * 0.5f;
    }
}
