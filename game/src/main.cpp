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

int main()
{
    InitWindow(800, 800, "Game");
    SetTargetFPS(60);

    std::array<Collider, 5> colliders_a;
    std::array<Collider, 4> colliders_b;
    colliders_a[1].type = colliders_b[1].type = COLLIDER_TYPE_CIRCLE;
    colliders_a[2].type = colliders_b[2].type = COLLIDER_TYPE_AABB;
    colliders_a[3].type = colliders_b[3].type = COLLIDER_TYPE_CAPSULE;
    colliders_a[4].type  = COLLIDER_TYPE_PLANE;

    colliders_a[1].circle.radius = colliders_b[1].circle.radius = 15.0f;
    colliders_a[2].aabb.half_extents = colliders_b[2].aabb.half_extents = { 10.0f, 20.0f };
    colliders_a[3].capsule.radius = colliders_b[3].capsule.radius = 10.0f;
    colliders_a[3].capsule.half_length = colliders_b[3].capsule.half_length = 15.0f;
    colliders_a[3].capsule.direction = colliders_b[3].capsule.direction = Vector2UnitX;
    colliders_a[4].plane.normal = Vector2UnitX;

    Vector2 collider_pos_a = { GetScreenWidth() * 0.25f, GetScreenHeight() * 0.25f };
    Vector2 collider_pos_b = { GetScreenWidth() * 0.75f, GetScreenHeight() * 0.25f };
    int collider_index_a = 1;
    int collider_index_b = 1;

    float entity_spawn_current = 0.0f;
    float entity_spawn_total = 0.5f;

    std::vector<Entity> entities;
    InitBallPitDemo(&entities);
    //InitForcesDemo(&entities);
    //InitFrictionDemo(&entities);

    bool is_first_frame = true;
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        {
            static float plane_angle = 0.0f;
            if (IsKeyDown(KEY_Q))
                plane_angle -= 100.0f * dt * DEG2RAD;
            if (IsKeyDown(KEY_E))
                plane_angle += 100.0f * dt * DEG2RAD;

            Vector2 normal = Vector2Rotate(Vector2UnitX, plane_angle);
            colliders_a[4].plane.normal = normal;
            //entities[0].collider.plane.normal = normal;
        }

        if (!is_first_frame)
        {
            for (Entity& e : entities)
                EntityUpdate(e, dt);
        }
        else
            is_first_frame = false;

        std::vector<EntityHit> hits = DetectCollisions(entities);
        for (const EntityHit& hit : hits)
        {
            ApplyCollisionImpulse(*hit.a, *hit.b, hit.mtv);
            ApplyCollisionMtv(*hit.a, *hit.b, hit.mtv);

            if (hit.a->on_collision != nullptr)
                hit.a->on_collision(hit.a, hit.b);

            if (hit.b->on_collision != nullptr)
                hit.b->on_collision(hit.b, hit.a);
        }

        for (size_t i = 0; i < entities.size(); i++)
        {
            if (entities[i].destroy && entities[i].on_destroy != nullptr)
                entities[i].on_destroy(&entities[i], &entities);
        }

        // entities[0..3] are planes so don't remove them!
        if (entities.size() >= 128)
            entities[4].destroy = true;
        std::erase_if(entities, [](const Entity& e) { return e.destroy; });

        //bool collider_collision = false;
        //{
        //    collider_pos_a = GetMousePosition();
        //    Vector2 mtv = Vector2Zeros;
        //    CollisionFunc func = COLLISION_TABLE[collider_index_a][collider_index_b];
        //    collider_collision = func(collider_pos_a, colliders_a[collider_index_a], collider_pos_b, colliders_b[collider_index_b], &mtv);
        //
        //    if (collider_collision)
        //        collider_pos_b += mtv;
        //}

        BeginDrawing();
        ClearBackground(WHITE);

        //DrawCollider(collider_pos_a, colliders_a[collider_index_a], collider_collision ? RED : GREEN);
        //DrawCollider(collider_pos_b, colliders_b[collider_index_b], collider_collision ? RED : GREEN);
        //GuiToggleGroup({ 10, 10, 90, 30 }, "None A;Circle A;AABB A;Capsule A;Plane A", &collider_index_a);
        //GuiToggleGroup({ 10, 45, 90, 30 }, "None B;Circle B;AABB B;Capsule B", &collider_index_b);
        //
        //if (collider_index_a == COLLIDER_TYPE_PLANE)
        //{
        //    Collider a = colliders_a[collider_index_a];
        //    Collider b = colliders_b[collider_index_b];
        //
        //    switch (collider_index_b)
        //    {
        //    case COLLIDER_TYPE_CIRCLE:
        //        DrawProjCirclePlane(collider_pos_b, b.circle.radius, collider_pos_a, a.plane.normal);
        //        break;
        //
        //    case COLLIDER_TYPE_AABB:
        //        DrawProjBoxPlane(collider_pos_b, b.aabb.half_extents, collider_pos_a, a.plane.normal);
        //        break;
        //
        //    case COLLIDER_TYPE_CAPSULE:
        //        DrawProjCapsulePlane(collider_pos_b, b.capsule.direction, b.capsule.half_length, b.capsule.radius, collider_pos_a, a.plane.normal);
        //        break;
        //    }
        //}

        for (const Entity& e : entities)
            DrawCollider(e.pos, e.collider, e.color);

        // OBB test
        {
            static Vector2 obb_pos = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
            static Vector2 obb_extents = { 40.0f, 20.0f };
            static Vector2 obb_direction = Vector2Rotate(Vector2UnitX, 30.0f * DEG2RAD);

            Vector2 obb_mtv = Vector2Zeros;
            bool obb_collision = HitTestCircleOBB(GetMousePosition(), 4.0f, obb_pos, obb_direction, obb_extents, &obb_mtv);
            obb_pos += obb_mtv;

            DrawOBB(obb_pos, obb_extents, obb_direction, obb_collision ? RED : GREEN);
            DrawCircleV(GetMousePosition(), 4.0f, BLUE);
        }

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

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

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
