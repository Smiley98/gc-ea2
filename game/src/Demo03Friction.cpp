namespace demo_friction
{
    std::vector<Entity> entities;

    void Load()
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
            entities.push_back(plane);
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
            entities.push_back(ball);
        }
    }

    void Unload()
    {
        entities.clear();
    }

    void Update(float dt)
    {
        // Rotate plane
        {
            static float angle = 0.0f;
            if (IsKeyDown(KEY_Q))
                angle -= 100.0f * dt * DEG2RAD;
            if (IsKeyDown(KEY_E))
                angle += 100.0f * dt * DEG2RAD;

            entities[0].collider.plane.normal = Vector2Rotate(Vector2UnitY * -1.0f, angle);
        }

        for (Entity& e : entities)
            EntityUpdate(e, dt);

        std::vector<EntityHit> hits = WorldDetectCollisions(entities);
        WorldResolveCollisions(hits);
    }

    void Draw()
    {
        BeginDrawing();
        ClearBackground(WHITE);

        for (const Entity& e : entities)
            DrawCollider(e.pos, e.collider, e.color);

        Entity& plane = entities[0];
        Entity& ball = entities[1];
        Vector2 normal = plane.collider.plane.normal;
        Vector2 vel_plane_to_ball = plane.vel - ball.vel;
        Vector2 tangent = Vector2Tangent(vel_plane_to_ball, normal);
        float proj_gravity = fabsf(Vector2DotProduct(GRAVITY, normal));
        DrawLineEx(ball.pos, ball.pos + tangent, 4.0f, DARKBLUE);
        DrawLineEx(ball.pos, ball.pos + GRAVITY, 4.0f, RED);
        DrawLineEx(ball.pos, ball.pos + normal * proj_gravity, 4.0f, GREEN);

        EndDrawing();
    }
}
