#include "Demos.h"
#include "Demo00Empty.cpp"
#include "Demo01Colliders.cpp"
#include "Demo02Forces.cpp"
#include "Demo03Friction.cpp"
#include "Demo04BallPit.cpp"
#include "Demo05OBBs.cpp"

Demo demo_00_empty =
{
    .Load = nullptr,
    .Unload = nullptr,
    .Update = nullptr,
    .Draw = demo_empty::Draw,
};

Demo demo_01_colliders =
{
    .Load = demo_colliders::Load,
    .Unload = nullptr,
    .Update = demo_colliders::Update,
    .Draw = demo_colliders::Draw,
};

Demo demo_02_forces =
{
    .Load = demo_forces::Load,
    .Unload = demo_forces::Unload,
    .Update = demo_forces::Update,
    .Draw = demo_forces::Draw,
};

Demo demo_03_friction =
{
    .Load = demo_friction::Load,
    .Unload = demo_friction::Unload,
    .Update = demo_friction::Update,
    .Draw = demo_friction::Draw,
};

Demo demo_04_ball_pit =
{
    .Load = demo_ball_pit::Load,
    .Unload = demo_ball_pit::Unload,
    .Update = demo_ball_pit::Update,
    .Draw = demo_ball_pit::Draw,
};

Demo demo_05_obbs =
{
    .Load = nullptr,
    .Unload = nullptr,
    .Update = nullptr,
    .Draw = demo_obbs::Draw,
};
