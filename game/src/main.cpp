#include "Build.cpp"

void InitWalls(std::vector<Entity>* entities);
void InitBallPitDemo(std::vector<Entity>* entities);
void MakeBall(Entity* e);

inline Color GetRandomColor()
{
    float r = GetRandomValue(0, 255) / 255.0f;
    float g = GetRandomValue(0, 255) / 255.0f;
    float b = GetRandomValue(0, 255) / 255.0f;
    return ColorFromNormalized({ r, g, b, 1.0f });
}

void OnCollisionTest(Entity* self, Entity* other)
{
    self->color = GetRandomColor();
    self->bounces--;
    if (self->bounces <= 0)
        self->destroy = true;
}

void OnDestroyTest(Entity* self, std::vector<Entity>* collection)
{
    Entity e{};
    MakeBall(&e);
    collection->push_back(e);
}

namespace demo_ball_pit
{
    void Load()
    {

    }

    void Update(float dt)
    {

    }

    void Draw()
    {

    }
}

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

//float entity_spawn_current = 0.0f;
//float entity_spawn_total = 0.5f;

//std::vector<Entity> entities;
//InitBallPitDemo(&entities);
//InitFrictionDemo(&entities);

//for (Entity& e : entities)
//    EntityUpdate(e, dt);

//std::vector<EntityHit> hits = DetectCollisions(entities);
//for (const EntityHit& hit : hits)
//{
//    ApplyCollisionImpulse(*hit.a, *hit.b, hit.mtv);
//    ApplyCollisionMtv(*hit.a, *hit.b, hit.mtv);
//
//    if (hit.a->on_collision != nullptr)
//        hit.a->on_collision(hit.a, hit.b);
//
//    if (hit.b->on_collision != nullptr)
//        hit.b->on_collision(hit.b, hit.a);
//}

//for (size_t i = 0; i < entities.size(); i++)
//{
//    if (entities[i].destroy && entities[i].on_destroy != nullptr)
//        entities[i].on_destroy(&entities[i], &entities);
//}

// entities[0..3] are planes so don't remove them!
//if (entities.size() >= 128)
//    entities[4].destroy = true;
//std::erase_if(entities, [](const Entity& e) { return e.destroy; });

//for (const Entity& e : entities)
//    DrawCollider(e.pos, e.collider, e.color);

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

// The relative velocity is plane relative to ball rather than ball relative to plane because friciton is *opposite* to velocity!
// (If the plane is guaranteed to be stationary, we can pass -ball.vel)
//{
//    Entity& plane = entities[0];
//    Entity& ball = entities[1];
//    Vector2 normal = plane.collider.plane.normal;
//    Vector2 vel_plane_to_ball = plane.vel - ball.vel;
//    Vector2 tangent = Vector2Tangent(vel_plane_to_ball, normal);
//    float proj_gravity = fabsf(Vector2DotProduct(GRAVITY, normal));
//    DrawLineEx(ball.pos, ball.pos + tangent, 4.0f, DARKBLUE);
//    DrawLineEx(ball.pos, ball.pos + GRAVITY, 4.0f, RED);
//    DrawLineEx(ball.pos, ball.pos + normal * proj_gravity, 4.0f, GREEN);
//}

void InitWalls(std::vector<Entity>* entities)
{
    Collider collider;
    collider.type = COLLIDER_TYPE_PLANE;

    {
        Entity left{};
        collider.plane.normal = Vector2UnitX;
        left.pos = { 0.0f, (float)GetScreenHeight() * 0.5f };
        left.collider = collider;
        left.restitution = 1.0f;
        left.color = GREEN;
        entities->push_back(left);
    }

    {
        Entity right{};
        collider.plane.normal = Vector2UnitX * -1.0f;
        right.pos = { (float)GetScreenWidth(), (float)GetScreenHeight() * 0.5f };
        right.collider = collider;
        right.restitution = 1.0f;
        right.color = GREEN;
        entities->push_back(right);
    }

    {
        Entity top{};
        collider.plane.normal = Vector2UnitY;
        top.pos = { GetScreenWidth() * 0.5f, 0.0f };
        top.collider = collider;
        top.restitution = 1.0f;
        top.color = GREEN;
        entities->push_back(top);
    }

    {
        Entity bottom{};
        collider.plane.normal = Vector2UnitY * -1.0f;
        bottom.pos = { GetScreenWidth() * 0.5f, (float)GetScreenHeight() };
        bottom.collider = collider;
        bottom.restitution = 1.0f;
        bottom.color = GREEN;
        entities->push_back(bottom);
    }
}

void InitBallPitDemo(std::vector<Entity>* entities)
{
    InitWalls(entities);

    Entity e{};
    MakeBall(&e);
    entities->push_back(e);
}

void MakeBall(Entity* e)
{
    Vector2 centre = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
    Vector2 direction = Vector2Rotate(Vector2UnitX, GetRandomValue(0, 360) * DEG2RAD);
    e->pos = centre + direction * 50.0f;
    e->vel = direction * 150.0f;

    e->gravity_scale = 0.0f;
    e->inverse_mass = 1.0f;

    e->restitution = 1.0f;
    e->friction = 0.0f;

    e->color = GetRandomColor();
    e->bounces = 3;

    e->on_collision = OnCollisionTest;
    e->on_destroy = OnDestroyTest;

    e->collider.type = COLLIDER_TYPE_CIRCLE;
    e->collider.circle.radius = 10.0f;
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

    app->demos.push_back(d00_empty);
    app->demos.push_back(d01_colliders);
    app->demos.push_back(d02_forces);
    app->demos.push_back(d03_friction);
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
