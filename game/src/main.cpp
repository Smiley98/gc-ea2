#include "Build.cpp"

int main()
{
    App app;
    AppLoad(&app);

    while (!WindowShouldClose())
    {
        AppUpdate(&app, GetFrameTime());
        AppDraw(&app);
    }

    AppUnload(&app);
    return 0;
}

// Lab 1 -- Get students to make OBB demo
// OBB test
//{
//    static Vector2 obb_pos = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
//    static Vector2 obb_extents = { 40.0f, 20.0f };
//    static Vector2 obb_direction = Vector2Rotate(Vector2UnitX, 30.0f * DEG2RAD);
//
//    Vector2 obb_mtv = Vector2Zeros;
//    bool obb_collision = HitTestCircleOBB(GetMousePosition(), 4.0f, obb_pos, obb_direction, obb_extents, &obb_mtv);
//    obb_pos += obb_mtv;
//
//    DrawOBB(obb_pos, obb_extents, obb_direction, obb_collision ? RED : GREEN);
//    DrawCircleV(GetMousePosition(), 4.0f, BLUE);
//}

// Notes:
// Apparently frame 1 (in addition to frame 0) gives huge time-delta?
// Investigate implementing proper fixed update??
// Might cause a headache / jittering, but may be worth while to weave into a timers lesson!
