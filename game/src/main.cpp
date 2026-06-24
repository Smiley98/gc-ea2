#include "raylib.h"
#include "raymath.h"

void GameInit()
{
    InitWindow(800, 800, "Game");
    SetTargetFPS(60);
}

void GameUnload()
{
    CloseWindow();
}

void GameUpdate(float dt)
{
}

void GameDraw()
{
    BeginDrawing();
    ClearBackground(WHITE);
    DrawCircleV(GetMousePosition(), 20.0f, RED);
    EndDrawing();
}

int main()
{
    GameInit();

    while (!WindowShouldClose())
    {
        GameUpdate(GetFrameTime());
        GameDraw();
    }

    GameUnload();
    return 0;
}
