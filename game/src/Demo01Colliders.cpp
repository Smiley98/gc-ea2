namespace demo_colliders
{
    std::array<Collider, 5> colliders_a;
    std::array<Collider, 4> colliders_b;

    Vector2 collider_pos_a;
    Vector2 collider_pos_b;

    int collider_index_a;
    int collider_index_b;

    bool collision = false;

    void Load()
    {
        colliders_a[1].type = colliders_b[1].type = COLLIDER_TYPE_CIRCLE;
        colliders_a[2].type = colliders_b[2].type = COLLIDER_TYPE_AABB;
        colliders_a[3].type = colliders_b[3].type = COLLIDER_TYPE_CAPSULE;
        colliders_a[4].type = COLLIDER_TYPE_PLANE;

        colliders_a[1].circle.radius = colliders_b[1].circle.radius = 15.0f;
        colliders_a[2].aabb.half_extents = colliders_b[2].aabb.half_extents = { 10.0f, 20.0f };
        colliders_a[3].capsule.radius = colliders_b[3].capsule.radius = 10.0f;
        colliders_a[3].capsule.half_length = colliders_b[3].capsule.half_length = 15.0f;
        colliders_a[3].capsule.direction = colliders_b[3].capsule.direction = Vector2UnitX;
        colliders_a[4].plane.normal = Vector2UnitX;

        collider_pos_a = { GetScreenWidth() * 0.25f, GetScreenHeight() * 0.25f };
        collider_pos_b = { GetScreenWidth() * 0.75f, GetScreenHeight() * 0.25f };

        collider_index_a = 1;
        collider_index_b = 1;
    }

    void Update(float dt)
    {
        static float angle = 0.0f;
        if (IsKeyDown(KEY_Q))
            angle -= 100.0f * dt * DEG2RAD;
        if (IsKeyDown(KEY_E))
            angle += 100.0f * dt * DEG2RAD;

        Vector2 normal = Vector2Rotate(Vector2UnitX, angle);
        colliders_a[4].plane.normal = normal;

        {
            collider_pos_a = GetMousePosition();
            Vector2 mtv = Vector2Zeros;
            CollisionFunc func = COLLISION_TABLE[collider_index_a][collider_index_b];
            collision = func(
                collider_pos_a, colliders_a[collider_index_a],
                collider_pos_b, colliders_b[collider_index_b], &mtv);

            if (collision)
                collider_pos_b += mtv;
        }
    }

    void Draw()
    {
        BeginDrawing();
        ClearBackground(WHITE);

        DrawCollider(collider_pos_a, colliders_a[collider_index_a], collision ? RED : GREEN);
        DrawCollider(collider_pos_b, colliders_b[collider_index_b], collision ? RED : GREEN);
        GuiToggleGroup({ 10, 10, 90, 30 }, "None A;Circle A;AABB A;Capsule A;Plane A", &collider_index_a);
        GuiToggleGroup({ 10, 45, 90, 30 }, "None B;Circle B;AABB B;Capsule B", &collider_index_b);

        if (collider_index_a == COLLIDER_TYPE_PLANE)
        {
            Collider a = colliders_a[collider_index_a];
            Collider b = colliders_b[collider_index_b];

            switch (collider_index_b)
            {
            case COLLIDER_TYPE_CIRCLE:
                DrawProjCirclePlane(collider_pos_b, b.circle.radius, collider_pos_a, a.plane.normal);
                break;

            case COLLIDER_TYPE_AABB:
                DrawProjBoxPlane(collider_pos_b, b.aabb.half_extents, collider_pos_a, a.plane.normal);
                break;

            case COLLIDER_TYPE_CAPSULE:
                DrawProjCapsulePlane(collider_pos_b, b.capsule.direction, b.capsule.half_length, b.capsule.radius, collider_pos_a, a.plane.normal);
                break;
            }
        }

        EndDrawing();
    }
}
