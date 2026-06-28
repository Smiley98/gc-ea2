namespace demo_gjk
{
    std::vector<Entity> entities;

    Vector2 position;
    Vector2 direction;
    float radius;
    float half_length;

    Vector2 SupportCircle(Vector2 position, float radius, Vector2 d)
    {
        return position + d * radius;
    }

    //Vector2 SupportCapsule(Vector2 position, Vector2 direction, float half_length, float radius, Vector2 d)
    //{
    //
    //}

    void Load()
    {
        position = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
        direction = Vector2UnitX;
        radius = 20.0f;
        half_length = 50.0f;
    }

    void Unload()
    {
        entities.clear();
    }

    void Update(float dt)
    {

    }

    void Draw()
    {
        ClearBackground(WHITE);

        Vector2 d = Vector2Normalize(GetMousePosition() - position);
        Vector2 p = SupportCircle(position, radius, d);

        DrawCircleV(position, radius, RED);
        DrawCircleV(p, 4.0f, BLUE);
    }
}
