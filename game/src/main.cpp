#include "Build.cpp"

struct App
{
    std::vector<Demo> demos;
    Demo* demo = nullptr;
};

void AppLoadDemos(App* app);
void AppUnloadDemos(App* app);

void AppLoad(App* app)
{
    InitWindow(800, 800, "Game");
    InitAudioDevice();
    SetTargetFPS(60);

    AppLoadDemos(app);
    if (app->demo->Load != nullptr)
        app->demo->Load();
}

void AppUnload(App* app)
{
    if (app->demo->Unload != nullptr)
        app->demo->Unload();
    AppUnloadDemos(app);

    CloseAudioDevice();
    CloseWindow();
}

void AppUpdate(App* app, float dt)
{
    static int frame_count = 0;
    if (frame_count >= 2)
    {
        if (app->demo->Update != nullptr)
            app->demo->Update(dt);
    }
    frame_count++;
}

void AppDraw(App* app)
{
    if (app->demo->Draw != nullptr)
        app->demo->Draw();
}

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

void AppLoadDemos(App* app)
{
    Demo d00_empty =
    {
        .Load = nullptr,
        .Unload = nullptr,
        .Update = nullptr,
        .Draw = demo_empty::Draw,
    };

    Demo d01_colliders =
    {
        .Load = demo_colliders::Load,
        .Unload = nullptr,
        .Update = demo_colliders::Update,
        .Draw = demo_colliders::Draw,
    };

    Demo d02_forces =
    {
        .Load = demo_forces::Load,
        .Unload = demo_forces::Unload,
        .Update = demo_forces::Update,
        .Draw = demo_forces::Draw,
    };

    Demo d03_friction =
    {
        .Load = demo_friction::Load,
        .Unload = demo_friction::Unload,
        .Update = demo_friction::Update,
        .Draw = demo_friction::Draw,
    };

    Demo d04_ball_pit =
    {
        .Load = demo_ball_pit::Load,
        .Unload = demo_ball_pit::Unload,
        .Update = demo_ball_pit::Update,
        .Draw = demo_ball_pit::Draw,
    };

    app->demos.push_back(d00_empty);
    app->demos.push_back(d01_colliders);
    app->demos.push_back(d02_forces);
    app->demos.push_back(d03_friction);
    app->demos.push_back(d04_ball_pit);
    app->demo = &app->demos.back();
}

void AppUnloadDemos(App* app)
{
    app->demo = nullptr;
    app->demos.clear();
}

// Notes:
// Apparently frame 1 (in addition to frame 0) gives huge time-delta?
// Investigate implementing proper fixed update??
// Might cause a headache / jittering, but may be worth while to weave into a timers lesson!
