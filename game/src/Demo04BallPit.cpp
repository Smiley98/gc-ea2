namespace demo_ball_pit
{
    std::vector<Entity> entities;

    void MakeBall(Entity* e);
    void OnCollisionTest(Entity* self, Entity* other);
    void OnDestroyTest(Entity* self, std::vector<Entity>* collection);
    Color GetRandomColor();

    void Load()
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
            entities.push_back(left);
        }

        {
            Entity right{};
            collider.plane.normal = Vector2UnitX * -1.0f;
            right.pos = { (float)GetScreenWidth(), (float)GetScreenHeight() * 0.5f };
            right.collider = collider;
            right.restitution = 1.0f;
            right.color = GREEN;
            entities.push_back(right);
        }

        {
            Entity top{};
            collider.plane.normal = Vector2UnitY;
            top.pos = { GetScreenWidth() * 0.5f, 0.0f };
            top.collider = collider;
            top.restitution = 1.0f;
            top.color = GREEN;
            entities.push_back(top);
        }

        {
            Entity bottom{};
            collider.plane.normal = Vector2UnitY * -1.0f;
            bottom.pos = { GetScreenWidth() * 0.5f, (float)GetScreenHeight() };
            bottom.collider = collider;
            bottom.restitution = 1.0f;
            bottom.color = GREEN;
            entities.push_back(bottom);
        }

        entities.push_back({});
        MakeBall(&entities.back());
    }

    void Unload()
    {
        entities.clear();
    }

    void Update(float dt)
    {
        for (Entity& e : entities)
            EntityUpdate(e, dt);

        std::vector<EntityHit> hits = WorldDetectCollisions(entities);
        WorldResolveCollisions(hits);

        for (size_t i = 0; i < entities.size(); i++)
        {
            if (entities[i].destroy && entities[i].on_destroy != nullptr)
                entities[i].on_destroy(&entities[i], &entities);
        }

        // entities[0..3] are planes so don't remove them!
        if (entities.size() >= 128)
            entities[4].destroy = true;
        std::erase_if(entities, [](const Entity& e) { return e.destroy; });
    }

    void Draw()
    {
        BeginDrawing();
        ClearBackground(WHITE);
        for (const Entity& e : entities)
            DrawCollider(e.pos, e.collider, e.color);
        EndDrawing();
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

    Color GetRandomColor()
    {
        float r = GetRandomValue(0, 255) / 255.0f;
        float g = GetRandomValue(0, 255) / 255.0f;
        float b = GetRandomValue(0, 255) / 255.0f;
        return ColorFromNormalized({ r, g, b, 1.0f });
    }
}
