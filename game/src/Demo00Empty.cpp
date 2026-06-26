namespace demo_empty
{
    void Load()
    {

    }

    void Unload()
    {

    }

    void Update(float dt)
    {

    }

    void Draw()
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawCircleV(GetMousePosition(), 20.0f, RED);
        EndDrawing();
    }
}
