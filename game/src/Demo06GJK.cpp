namespace demo_gjk
{
    Vector2 position;
    Vector2 direction;
    float radius;
    float half_length;

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

    void Load()
    {
        position = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
        direction = Vector2UnitX;
        radius = 20.0f;
        half_length = 50.0f;
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
        Vector2 p = SupportCapsule(position, direction, half_length, radius, d);

        //DrawCircleV(position, radius, RED);
        DrawCapsule(position, direction, half_length, radius, RED);
        DrawCircleV(p, 4.0f, BLUE);

        // Support point is "poi between shape and plane perpendicular to d"
        DrawLineEx(position, GetMousePosition(), 4.0f, ORANGE);
        DrawCircleV(GetMousePosition(), 4.0f, ORANGE);
    }
}
