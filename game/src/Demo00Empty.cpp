namespace demo_empty
{
    void Draw()
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawCircleV(GetMousePosition(), 20.0f, RED);
        EndDrawing();
    }
}
