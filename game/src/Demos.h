#pragma once
struct Demo
{
    void (*Load)();
    void (*Unload)();
    void (*Update)(float dt);
    void (*Draw)();
};

inline void DemoReset(Demo* demo)
{
    if (demo->Unload != nullptr)
        demo->Unload();

    if (demo->Load != nullptr)
        demo->Load();
}

extern Demo demo_00_empty;
extern Demo demo_01_colliders;
extern Demo demo_02_forces;
extern Demo demo_03_friction;
extern Demo demo_04_ball_pit;
extern Demo demo_05_obbs;
extern Demo demo_06_gjk;
