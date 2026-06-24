#pragma once

inline Rectangle RecFromAABB(Vector2 position, Vector2 half_extents)
{
    Rectangle rec;
    rec.x = position.x - half_extents.x;
    rec.y = position.y - half_extents.y;
    rec.width = half_extents.x * 2.0f;
    rec.height = half_extents.y * 2.0f;
    return rec;
}

// Points FROM bottom TO top, outputs position (midpoint)
inline CapsuleCollider CapsuleFromPoints(Vector2 bottom, Vector2 top, float radius, Vector2* position = nullptr)
{
    CapsuleCollider capsule;

    capsule.radius = radius;
    capsule.half_length = (Vector2Distance(bottom, top) - radius * 2.0f) * 0.5f;
    capsule.direction = Vector2Normalize(top - bottom);

    if (position != nullptr)
        *position = (bottom + top) * 0.5f;

    return capsule;
}
