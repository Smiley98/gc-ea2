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
