namespace demo_gjk
{
    Vector2 position;
    Vector2 direction;
    float radius;
    float half_length;
    Vector2 half_extents;

    Vector2 SupportCircle(Vector2 position, float radius, Vector2 d)
    {
        return position + d * radius;
    }

    Vector2 SupportCapsule(Vector2 position, Vector2 direction, float half_length, float radius, Vector2 d)
    {
        Vector2 top = position + direction * half_length;
        Vector2 bot = position - direction * half_length;
        Vector2 proj = Vector2DotProduct(top, d) >= Vector2DotProduct(bot, d) ? top : bot;
        return proj + d * radius;
    }

    Vector2 SupportAABB(Vector2 position, Vector2 half_extents, Vector2 d)
    {
        Vector2 proj = Vector2Zeros;
        proj.x = d.x >= 0.0f ? half_extents.x : -half_extents.x;
        proj.y = d.y >= 0.0f ? half_extents.y : -half_extents.y;
        return position + proj;
    }

    Vector2 SupportOBB(Vector2 position, Vector2 direction, Vector2 half_extents, Vector2 d)
    {
        // Transform d into OBB's local-space then treat as AABB
        Vector2 x_axis = direction;
        Vector2 y_axis = { -direction.y, direction.x };
        Vector2 d_local =
        {
            Vector2DotProduct(d, x_axis),
            Vector2DotProduct(d, y_axis)
        };

        Vector2 proj =
        {
            d_local.x >= 0.0f ? half_extents.x : -half_extents.x,
            d_local.y >= 0.0f ? half_extents.y : -half_extents.y
        };

        return position + x_axis * proj.x + y_axis * proj.y;
    }

    void Load()
    {
        position = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
        direction = Vector2UnitX;
        radius = 20.0f;
        half_length = 50.0f;
        half_extents.x = half_length;
        half_extents.y = radius;
    }

    void Unload()
    {
    }

    void Update(float dt)
    {
        static float angle = 0.0f;
        if (IsKeyDown(KEY_Q))
            angle -= 100.0f * dt * DEG2RAD;
        if (IsKeyDown(KEY_E))
            angle += 100.0f * dt * DEG2RAD;

        direction = Vector2Rotate(Vector2UnitX, angle);
    }

    void Draw()
    {
        ClearBackground(WHITE);

        Vector2 d = Vector2Normalize(GetMousePosition() - position);
        //Vector2 p = SupportCircle(position, radius, d);
        //Vector2 p = SupportCapsule(position, direction, half_length, radius, d);
        //Vector2 p = SupportAABB(position, half_extents, d);
        Vector2 p = SupportOBB(position, direction, half_extents, d);

        //DrawCircleV(position, radius, RED);
        //DrawCapsule(position, direction, half_length, radius, RED);
        //DrawAABB(position, half_extents, RED);
        DrawOBB(position, half_extents, direction, RED);
        DrawCircleV(p, 4.0f, BLUE);

        // Support point is "poi between shape and plane perpendicular to d"
        DrawLineEx(position, GetMousePosition(), 4.0f, ORANGE);
        DrawCircleV(GetMousePosition(), 4.0f, ORANGE);
    }
}
