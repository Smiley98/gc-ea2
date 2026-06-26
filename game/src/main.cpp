#include "Build.cpp"

void InitWalls(std::vector<Entity>* entities);
void InitForcesDemo(std::vector<Entity>* entities);
void InitFrictionDemo(std::vector<Entity>* entities);
void InitBallPitDemo(std::vector<Entity>* entities);
void MakeBall(Entity* e);

std::vector<EntityHit> DetectCollisions(const std::vector<Entity>& entities);

void ApplyCollisionImpulse(Entity& a/*dynamic*/, Entity& b/*static*/, Vector2 mtv);
void ApplyCollisionMtv(Entity& a/*dynamic*/, Entity& b/*static*/, Vector2 mtv);

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

}

namespace demo_forces
{

}

namespace demo_friction
{

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
    static bool is_first_frame = true;
    if (!is_first_frame)
    {
        if (app->demo->Update != nullptr)
            app->demo->Update(dt);
    }
    else
        is_first_frame = false;
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
//InitForcesDemo(&entities);
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

void InitForcesDemo(std::vector<Entity>* entities)
{
    Entity light{}, medium{}, heavy{};
    light.gravity_scale = medium.gravity_scale = heavy.gravity_scale = 1.0f;
    light.color = medium.color = heavy.color = GRAY;

    Collider collider;
    collider.type = COLLIDER_TYPE_CIRCLE;
    collider.circle.radius = 20.0f;

    light.collider = medium.collider = heavy.collider = collider;

    light.pos = { 200.0f, 600.0f };
    medium.pos = { 400.0f, 600.0f };
    heavy.pos = { 600.0f, 600.0f };

    light.inverse_mass = 1.0f / 10.0f;
    medium.inverse_mass = 1.0f / 25.0f;
    heavy.inverse_mass = 1.0f / 50.0f;

    // Apply 100,000N of force to all 3 balls
    light.force = medium.force = heavy.force = Vector2UnitY * -100000.0f;

    entities->push_back(light);
    entities->push_back(medium);
    entities->push_back(heavy);
}

void InitFrictionDemo(std::vector<Entity>* entities)
{
    Vector2 position = { GetScreenWidth() * 0.5f, 600.0f };

    {
        Entity plane{};
        plane.pos = position;
        plane.collider.type = COLLIDER_TYPE_PLANE;
        plane.collider.plane.normal = Vector2UnitY * -1.0f;
        
        plane.gravity_scale = 0.0f;
        plane.inverse_mass = 0.0f;
        plane.restitution = 1.0f;
        plane.friction = 1.0f;
        
        plane.color = GREEN;
        entities->push_back(plane);
    }

    {
        Entity ball{};
        ball.collider.type = COLLIDER_TYPE_CIRCLE;
        ball.collider.circle.radius = 20.0f;
        ball.pos = position - Vector2UnitY * ball.collider.circle.radius;
        ball.vel = Vector2UnitX * 250.0f;

        ball.gravity_scale = 1.0f;
        ball.inverse_mass = 1.0f;
        ball.restitution = 1.0f;
        ball.friction = 1.0f;

        ball.color = PURPLE;
        entities->push_back(ball);
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

std::vector<EntityHit> DetectCollisions(const std::vector<Entity>& entities)
{
    std::vector<EntityHit> hits;
    for (int i = 0; i < entities.size(); i++)
    {
        for (int j = i + 1; j < entities.size(); j++)
        {
            const Entity& a = entities[i];
            const Entity& b = entities[j];
            assert(a.collider.type != COLLIDER_TYPE_NONE && b.collider.type != COLLIDER_TYPE_NONE);
            if (EntityIsMassInfinite(a) && EntityIsMassInfinite(b)) continue; // Don't test two static objects

            Vector2 mtv = Vector2Zeros;
            CollisionFunc func = COLLISION_TABLE[a.collider.type][b.collider.type];
            bool collision = func(a.pos, a.collider, b.pos, b.collider, &mtv);

            if (collision)
            {
                EntityHit hit;
                if (EntityIsMassInfinite(a))
                {
                    hit.a = (Entity*)&b;
                    hit.b = (Entity*)&a;
                }
                else
                {
                    hit.a = (Entity*)&a;
                    hit.b = (Entity*)&b;
                }

                float dot = Vector2DotProduct(hit.a->pos - hit.b->pos, mtv);
                if (dot < 0.0f)
                {
                    mtv *= -1.0f;
                }

                hit.mtv = mtv;
                hits.push_back(hit);
            }
        }
    }

    return hits;
}

void ApplyCollisionImpulse(Entity& a/*dynamic*/, Entity& b/*static*/, Vector2 mtv)
{
    float inverse_mass_sum = a.inverse_mass + b.inverse_mass;
    assert(inverse_mass_sum > EPSILON);

    Vector2 normal = Vector2Normalize(mtv);
    Vector2 vel_rel = a.vel - b.vel;
    float proj_vel_norm = Vector2DotProduct(vel_rel, normal);

    // Objects already separating (fails if vel_rel is pointing the wrong way, textbook is incorrect)
    if (proj_vel_norm > 0.0f) return;

    float e = fminf(a.restitution, b.restitution);
    float j = (-(1.0f + e) * proj_vel_norm) / inverse_mass_sum;

    Vector2 impulse = normal * j;
    a.vel += impulse * a.inverse_mass;
    b.vel -= impulse * b.inverse_mass;

    Vector2 tangent = Vector2Normalize(Vector2Tangent(vel_rel, normal));

    float jt = -Vector2DotProduct(vel_rel, tangent) / inverse_mass_sum;
    float friction = sqrtf(a.friction * b.friction);
    jt = Clamp(jt, -j * friction, j * friction);

    Vector2 tangent_impulse = tangent * jt;

    a.vel += tangent_impulse * a.inverse_mass;
    b.vel -= tangent_impulse * b.inverse_mass;
}

void ApplyCollisionMtv(Entity& a/*dynamic*/, Entity& b/*static*/, Vector2 mtv)
{
    if (EntityIsMassInfinite(b))
    {
       a.pos += mtv;
    }
    else
    {
        a.pos += mtv * 0.5f;
        b.pos -= mtv * 0.5f;
    }
}

void AppLoadDemos(App* app)
{
    Demo d00_empty =
    {
        .Load = demo_empty::Load,
        .Unload = nullptr,
        .Update = demo_empty::Update,
        .Draw = demo_empty::Draw,
    };

    Demo d01_colliders =
    {
        .Load = demo_colliders::Load,
        .Unload = nullptr,
        .Update = demo_colliders::Update,
        .Draw = demo_colliders::Draw,
    };

    app->demos.push_back(d00_empty);
    app->demos.push_back(d01_colliders);
    app->demo = &app->demos.back();
}

void AppUnloadDemos(App* app)
{
    app->demo = nullptr;
    app->demos.clear();
}
