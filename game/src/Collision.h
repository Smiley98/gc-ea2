#pragma once

inline bool Overlap(float min1, float max1, float min2, float max2)
{
    return !((max1 < min2) || (max2 < min1));
}

inline void NearestPointsOnLines(Vector2 bot1, Vector2 top1, Vector2 bot2, Vector2 top2, Vector2* nearest1, Vector2* nearest2)
{
    {
        Vector2 A = Vector2ProjectPointLine(top1, bot2, top2); // proj_top1_line2 
        Vector2 B = Vector2ProjectPointLine(bot1, bot2, top2); // proj_bot1_line2
        Vector2 C = Vector2ProjectPointLine(A, bot1, top1);    // proj_a_line1
        Vector2 D = Vector2ProjectPointLine(B, bot1, top1);    // proj_b_line1
        *nearest1 = Vector2LengthSqr(A - C) < Vector2LengthSqr(B - D) ? C : D; // Point on line 1 nearest to line 2
    }

    {
        Vector2 A = Vector2ProjectPointLine(top2, bot1, top1); // proj_top2_line1 
        Vector2 B = Vector2ProjectPointLine(bot2, bot1, top1); // proj_bot2_line1
        Vector2 C = Vector2ProjectPointLine(A, bot2, top2);    // proj_a_line2
        Vector2 D = Vector2ProjectPointLine(B, bot2, top2);    // proj_b_line2
        *nearest2 = Vector2LengthSqr(A - C) < Vector2LengthSqr(B - D) ? C : D; // Point on line 2 nearest to line 1
    }
}

inline bool HitTestCircles(Vector2 position_a, float radius_a, Vector2 position_b, float radius_b, Vector2* mtv = nullptr)
{
    float radii_sum = radius_a + radius_b;
    bool collision = Vector2DistanceSqr(position_a, position_b) <= radii_sum * radii_sum;
    if (collision && mtv != nullptr)
    {
        *mtv = Vector2Normalize(position_b - position_a) * (radii_sum - Vector2Distance(position_a, position_b));
    }
    return collision;
}

inline bool HitTestCapsules(
    Vector2 position_a, Vector2 direction_a, float half_length_a, float radius_a,
    Vector2 position_b, Vector2 direction_b, float half_length_b, float radius_b, Vector2* mtv = nullptr)
{
    Vector2 top1 = position_a + direction_a * half_length_a;
    Vector2 bot1 = position_a - direction_a * half_length_a;
    Vector2 top2 = position_b + direction_b * half_length_b;
    Vector2 bot2 = position_b - direction_b * half_length_b;
    Vector2 proj1, proj2;
    NearestPointsOnLines(bot1, top1, bot2, top2, &proj1, &proj2);
    return HitTestCircles(proj1, radius_a, proj2, radius_b, mtv);
}

inline bool HitTestAABBs(Vector2 position_a, Vector2 half_extents_a, Vector2 position_b, Vector2 half_extents_b, Vector2* mtv = nullptr)
{
    Vector2 min_a = position_a - half_extents_a;
    Vector2 max_a = position_a + half_extents_a;
    Vector2 min_b = position_b - half_extents_b;
    Vector2 max_b = position_b + half_extents_b;

    bool collision =
        Overlap(min_a.x, max_a.x, min_b.x, max_b.x) &&
        Overlap(min_a.y, max_a.y, min_b.y, max_b.y);

    if (collision && mtv != nullptr)
    {
        Vector2 min = Vector2Max(min_a, min_b);
        Vector2 max = Vector2Min(max_a, max_b);

        float dx = max.x - min.x;
        float dy = max.y - min.y;

        if (dx < dy)
        {
            *mtv = { dx, 0.0f };
            if (position_a.x > min.x) mtv->x *= -1.0f;
        }
        else
        {
            *mtv = { 0.0f, dy };
            if (position_a.y > min.y) mtv->y *= -1.0f;
        }
    }

    return collision;
}

inline bool HitTestCircleAABB(Vector2 position_circle, float radius, Vector2 position_aabb, Vector2 half_extents, Vector2* mtv = nullptr)
{
    Vector2 proj = Vector2Clamp(position_circle, position_aabb - half_extents, position_aabb + half_extents);
    return HitTestCircles(position_circle, radius, proj, 1.0f, mtv);
}

inline bool HitTestCircleCapsule(Vector2 position_circle, float radius_circle,
    Vector2 position_capsule, Vector2 direction, float half_length, float radius_capsule, Vector2* mtv = nullptr)
{
    Vector2 top = position_capsule + direction * half_length;
    Vector2 bot = position_capsule - direction * half_length;
    Vector2 proj = Vector2ProjectPointLine(position_circle, bot, top);
    return HitTestCircles(position_circle, radius_circle, proj, radius_capsule, mtv);
}

inline bool HitTestCapsuleAABB(Vector2 position_capsule, Vector2 direction, float half_length, float radius, Vector2 position_aabb, Vector2 half_extents, Vector2* mtv = nullptr)
{
    Vector2 rec_vertices[4]
    {
        position_aabb + Vector2{ -half_extents.x,  half_extents.y }, // top left
        position_aabb + Vector2{  half_extents.x,  half_extents.y }, // top right
        position_aabb + Vector2{ -half_extents.x, -half_extents.y }, // bottom left
        position_aabb + Vector2{  half_extents.x, -half_extents.y }, // bottom right
    };

    // Projection of rectangle vertices onto capsule
    Vector2 proj_rec_capsule[4];
    for (int i = 0; i < 4; i++)
        proj_rec_capsule[i] = Vector2ProjectPointLine(rec_vertices[i], position_capsule - direction * half_length, position_capsule + direction * half_length);

    Vector2 proj_capsule_rec[4];
    for (int i = 0; i < 4; i++)
        proj_capsule_rec[i] = Vector2Clamp(proj_rec_capsule[i], position_aabb - half_extents, position_aabb + half_extents);

    int index = 0;
    float d0 = Vector2LengthSqr(proj_rec_capsule[0] - proj_capsule_rec[0]);
    for (int i = 1; i < 4; i++)
    {
        float d = Vector2LengthSqr(proj_rec_capsule[i] - proj_capsule_rec[i]);
        if (d < d0)
        {
            d0 = d;
            index = i;
        }
    }

    return HitTestCircles(proj_rec_capsule[index], radius, proj_capsule_rec[index], 1.0f, mtv);
}

inline bool HitTestPlaneCircle(Vector2 position_plane, Vector2 normal, Vector2 position_circle, float radius, Vector2* mtv = nullptr)
{
    float proj_mag = Vector2DotProduct(position_circle - position_plane, normal);
    bool collision = proj_mag <= radius;
    if (collision && mtv != nullptr)
    {
        *mtv = normal * (radius - proj_mag);
    }
    return collision;
}

inline bool HitTestPlaneCapsule(Vector2 position_plane, Vector2 normal, Vector2 position_capsule, Vector2 direction, float half_length, float radius, Vector2* mtv = nullptr)
{
    Vector2 top = position_capsule + direction * half_length;
    Vector2 bot = position_capsule - direction * half_length;
    float proj_mag1 = Vector2DotProduct(top - position_plane, normal);
    float proj_mag2 = Vector2DotProduct(bot - position_plane, normal);
    float proj_mag = proj_mag1 < proj_mag2 ? proj_mag1 : proj_mag2;
    bool collision = proj_mag <= radius;
    if (collision && mtv != nullptr)
    {
        *mtv = normal * (radius - proj_mag);
    }
    return collision;
}

inline bool HitTestPlaneAABB(Vector2 position_plane, Vector2 normal, Vector2 position_aabb, Vector2 half_extents, Vector2* mtv = nullptr)
{
    Vector2 proj = Vector2Clamp(position_aabb - normal * Vector2LengthSqr(half_extents), position_aabb - half_extents, position_aabb + half_extents);
    return HitTestPlaneCircle(position_plane, normal, proj, 1.0f, mtv);
}

inline bool HitTestCircleOBB(Vector2 position_circle, float radius, Vector2 position_obb, Vector2 direction_obb, Vector2 half_extents, Vector2* mtv = nullptr)
{
    // Convert point to obb's local space by applying inverse transformation:
    // 1. Subtract obb's position
    // 2. Apply opposite of obb's rotation

    float radians = Vector2Angle(Vector2UnitX, direction_obb);
    return HitTestCircleAABB((position_circle - position_obb) * MatrixRotateZ(-radians), radius, Vector2Zeros, half_extents, mtv);
}

// Note: HitTestCircleOBB's mtv behaves as if the box is an AABB (incorrect)
// This is the same as HitTestCircleBox with r = 1.0f...
//inline bool HitTestAABBPoint(Vector2 position_aabb, Vector2 half_extents, Vector2 point)
//{
//    bool a = point.x >= position_aabb.x - half_extents.x;
//    bool b = point.x <= position_aabb.x + half_extents.x;
//    bool c = point.y >= position_aabb.y - half_extents.y;
//    bool d = point.y <= position_aabb.y + half_extents.y;
//    bool collision = a && b && c && d;
//    return collision;
//}
