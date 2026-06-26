#include "World.h"

std::vector<EntityHit> WorldDetectCollisions(const std::vector<Entity>& entities)
{
    std::vector<EntityHit> hits;
    for (int i = 0; i < entities.size(); i++)
    {
        for (int j = i + 1; j < entities.size(); j++)
        {
            const Entity& a = entities[i];
            const Entity& b = entities[j];
            assert(a.collider.type != COLLIDER_TYPE_NONE && b.collider.type != COLLIDER_TYPE_NONE);
            if (EntityIsMassInfinite(a) && EntityIsMassInfinite(b)) continue; // Don't test two static objects

            Vector2 mtv = Vector2Zeros;
            CollisionFunc func = COLLISION_TABLE[a.collider.type][b.collider.type];
            bool collision = func(a.pos, a.collider, b.pos, b.collider, &mtv);

            if (collision)
            {
                EntityHit hit;
                if (EntityIsMassInfinite(a))
                {
                    hit.a = (Entity*)&b;
                    hit.b = (Entity*)&a;
                }
                else
                {
                    hit.a = (Entity*)&a;
                    hit.b = (Entity*)&b;
                }

                float dot = Vector2DotProduct(hit.a->pos - hit.b->pos, mtv);
                if (dot < 0.0f)
                {
                    mtv *= -1.0f;
                }

                hit.mtv = mtv;
                hits.push_back(hit);
            }
        }
    }

    return hits;
}

void WorldResolveCollisions(std::vector<EntityHit> hits)
{
    for (EntityHit hit : hits)
    {
        EntityApplyCollisionImpulse(*hit.a, *hit.b, hit.mtv);
        EntityApplyCollisionMtv(*hit.a, *hit.b, hit.mtv);
    }
}
