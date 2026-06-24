#pragma once

inline void DrawAABB(Vector2 position, Vector2 half_extents, Color color)
{
    DrawRectangleRec(RecFromAABB(position, half_extents), color);
}

inline void DrawOBB(Vector2 position, Vector2 half_extents, Vector2 direction, Color color)
{
    Rectangle rec;
    rec.x = position.x;
    rec.y = position.y;
    rec.width = half_extents.x * 2.0f;
    rec.height = half_extents.y * 2.0f;

    float radians = Vector2Angle(Vector2UnitX, direction);
    DrawRectanglePro(rec, { half_extents.x, half_extents.y }, radians * RAD2DEG, color);
}

inline void DrawCapsule(Vector2 position, Vector2 direction, float half_length, float radius, Color color)
{
    Vector2 top = position + direction * half_length;
    Vector2 bot = position - direction * half_length;
    DrawOBB(position, { half_length, radius }, direction, color);
    DrawCircleV(top, radius, color);
    DrawCircleV(bot, radius, color);

    DrawLineEx(position, top, half_length * 0.1f, BLUE);
    DrawLineEx(top, top + Vector2Rotate(direction, 45.0f * DEG2RAD) * radius, radius * 0.1f, SKYBLUE);

    DrawCircleV(top, radius * 0.25f, DARKBLUE);
    DrawCircleV(bot, radius * 0.25f, MAGENTA);
}

inline void DrawCollider(Vector2 position, Collider collider, Color color)
{
    switch (collider.type)
    {
        case COLLIDER_TYPE_CIRCLE:
        {
            DrawCircleV(position, collider.circle.radius, color);
            break;
        }
            
        case COLLIDER_TYPE_AABB:
        {
            DrawAABB(position, collider.aabb.half_extents, color);
            break;
        }
            
        case COLLIDER_TYPE_CAPSULE:
        {
            DrawCapsule(position, collider.capsule.direction, collider.capsule.half_length, collider.capsule.radius, color);
            break;
        }
            
        case COLLIDER_TYPE_PLANE:
        {
            Vector2 perp = Vector2PerpendicularLeft(collider.plane.normal);
            DrawLineEx(position + perp * 2000.0f, position - perp * 2000.0f, 4.0f, color);
            DrawLineEx(position, position + collider.plane.normal * 20.0f, 2.0f, RED);
            break;
        }
    }
}

inline void DrawProjCirclePlane(Vector2 circle_position, float radius, Vector2 plane_position, Vector2 normal)
{
    {
        Collider plane;
        plane.type = COLLIDER_TYPE_PLANE;
        plane.plane.normal = normal;
        DrawCollider(plane_position, plane, GREEN);
    }

    float proj_mag = Vector2DotProduct(circle_position - plane_position, normal);
    Vector2 proj = plane_position + normal * proj_mag;

    DrawCircleV(proj, radius, BLUE);
}

inline void DrawProjBoxPlane(Vector2 box_position, Vector2 half_extents, Vector2 plane_position, Vector2 normal)
{
    Vector2 proj = Vector2Clamp(box_position - normal * Vector2LengthSqr(half_extents), box_position - half_extents, box_position + half_extents);
    DrawCircleV(proj, 5.0f, BLUE);
}

inline void DrawProjCapsulePlane(Vector2 capsule_position, Vector2 direction, float half_length, float radius, Vector2 plane_position, Vector2 normal)
{
    Vector2 top = capsule_position + direction * half_length;
    Vector2 bot = capsule_position - direction * half_length;
    float proj_mag1 = Vector2DotProduct(top - plane_position, normal);
    float proj_mag2 = Vector2DotProduct(bot - plane_position, normal);
    float proj_mag = proj_mag1 < proj_mag2 ? proj_mag1 : proj_mag2;
    Vector2 proj = plane_position + normal * proj_mag;
    DrawCircleV(proj, radius, BLUE);
}
