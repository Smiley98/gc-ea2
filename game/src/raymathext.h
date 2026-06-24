#pragma once
#include "raymath.h"

RMAPI Vector2 Vector2PerpendicularRight(Vector2 v)
{
    Vector2 perp = { v.y, -v.x };
    return perp;
}

RMAPI Vector2 Vector2PerpendicularLeft(Vector2 v)
{
    Vector2 perp = { -v.y, v.x };
    return perp;
}

RMAPI Vector2 Vector2Tangent(Vector2 v, Vector2 normal)
{
    float proj = Vector2DotProduct(v, normal);
    Vector2 tangent = v - normal * proj;
    return tangent;
}

RMAPI Vector2 Vector2ProjectPointLine(Vector2 P, Vector2 A, Vector2 B)
{
    Vector2 AB = (B - A);
    float t = Vector2DotProduct((P - A), AB) / Vector2DotProduct(AB, AB);
    return A + (AB * Clamp(t, 0.0f, 1.0f));
}
