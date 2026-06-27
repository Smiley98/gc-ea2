namespace demo_forces
{
    std::vector<Entity> entities;

    void Load()
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

        entities.push_back(light);
        entities.push_back(medium);
        entities.push_back(heavy);
    }

    void Unload()
    {
        entities.clear();
    }

    void Update(float dt)
    {
        for (Entity& e : entities)
            EntityUpdate(e, dt);

        bool reset = true;
        for (const Entity& e : entities)
            reset &= e.pos.y >= GetScreenHeight();

        if (reset)
            DemoReset(&demo_02_forces);
    }

    void Draw()
    {
        ClearBackground(WHITE);
        for (const Entity& e : entities)
            DrawCollider(e.pos, e.collider, e.color);
    }
}
