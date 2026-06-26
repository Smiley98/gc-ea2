struct Demo
{
    void (*Load)();
    void (*Unload)();
    void (*Update)(float dt);
    void (*Draw)();
};

#include "Demo00Empty.cpp"
#include "Demo01Colliders.cpp"
#include "Demo02Forces.cpp"
#include "Demo03Friction.cpp"

