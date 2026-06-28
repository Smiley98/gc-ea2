struct App
{
    std::vector<Demo*> demos;
    size_t demo_index;
    bool demo_select;
};

void AppLoadDemos(App* app);
void AppUnloadDemos(App* app);
void AppSetDemo(App* app, size_t demo_index);

void AppLoad(App* app)
{
    InitWindow(800, 800, "Game");
    InitAudioDevice();
    SetTargetFPS(60);

    AppLoadDemos(app);
    if (app->demos[app->demo_index]->Load != nullptr)
        app->demos[app->demo_index]->Load();
}

void AppUnload(App* app)
{
    if (app->demos[app->demo_index]->Unload != nullptr)
        app->demos[app->demo_index]->Unload();
    AppUnloadDemos(app);

    CloseAudioDevice();
    CloseWindow();
}

void AppUpdate(App* app, float dt)
{
    static int frame_count = 0;
    frame_count++;
    if (frame_count > 2)
    {
        if (app->demos[app->demo_index]->Update != nullptr)
            app->demos[app->demo_index]->Update(dt);
    }

    if (IsKeyPressed(KEY_GRAVE))
        app->demo_select = !app->demo_select;
}

void AppDraw(App* app)
{
    BeginDrawing();

    if (app->demos[app->demo_index]->Draw != nullptr)
        app->demos[app->demo_index]->Draw();

    if (app->demo_select)
    {
        int demo_index = app->demo_index;
        GuiToggleGroup({ 10, 760, 90, 30 }, "0 - Empty;1 - Collision;2 - Forces;3 - Friction;4 - Ball Pit; 5 - OBBs", &demo_index);

        if (demo_index != app->demo_index)
            AppSetDemo(app, (size_t)demo_index);
    }

    EndDrawing();
}

void AppLoadDemos(App* app)
{
    app->demos.push_back(&demo_00_empty);
    app->demos.push_back(&demo_01_colliders);
    app->demos.push_back(&demo_02_forces);
    app->demos.push_back(&demo_03_friction);
    app->demos.push_back(&demo_04_ball_pit);
    app->demos.push_back(&demo_05_obbs);
    app->demos.push_back(&demo_06_gjk);
    app->demo_index = app->demos.size() - 1;
}

void AppUnloadDemos(App* app)
{
    app->demos.clear();
    app->demo_index = 0;
}

void AppSetDemo(App* app, size_t demo_index)
{
    if (app->demos[app->demo_index]->Unload != nullptr)
        app->demos[app->demo_index]->Unload();

    app->demo_index = demo_index;

    if (app->demos[app->demo_index]->Load != nullptr)
        app->demos[app->demo_index]->Load();
}
