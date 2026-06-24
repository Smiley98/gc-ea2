#pragma once

enum ColliderType
{
    COLLIDER_TYPE_NONE,
    COLLIDER_TYPE_CIRCLE,
    COLLIDER_TYPE_AABB,
    //COLLIDER_TYPE_OBB,
    COLLIDER_TYPE_CAPSULE,
    COLLIDER_TYPE_PLANE
};

struct CircleCollider
{
    float radius;
};

struct AABBCollider
{
    Vector2 half_extents;
};

struct OBBCollider
{
    Vector2 half_extents;
    Vector2 direction;
};

struct CapsuleCollider
{
    float radius;
    float half_length;
    Vector2 direction;
};

struct PlaneCollider
{
    Vector2 normal;
};

struct Collider
{
    ColliderType type;
    union
    {
        CircleCollider circle;
        CapsuleCollider capsule;
        PlaneCollider plane;
        AABBCollider aabb;
        //OBBCollider obb;
    };
};

inline bool HitTestNone(Vector2 pA, Collider cA, Vector2 pB, Collider cB, Vector2* mtv)
{
    // This function should never be called because all colliders should have non-none types!
    assert(false);
    return false;
}

inline bool HitTestCircles(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{
    return HitTestCircles(position_a, collider_a.circle.radius, position_b, collider_b.circle.radius, mtv);
}

inline bool HitTestAABBs(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{
    return HitTestAABBs(position_a, collider_a.aabb.half_extents, position_b, collider_b.aabb.half_extents, mtv);
}

inline bool HitTestCapsules(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{
    return HitTestCapsules(
        position_a, collider_a.capsule.direction, collider_a.capsule.half_length, collider_a.capsule.radius,
        position_b, collider_b.capsule.direction, collider_b.capsule.half_length, collider_b.capsule.radius, mtv);
}

inline bool HitTestCircleAABB(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{
    return HitTestCircleAABB(position_a, collider_a.circle.radius, position_b, collider_b.aabb.half_extents, mtv);
}

inline bool HitTestAABBCircle(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{
    bool collision = HitTestCircleAABB(position_b, collider_b, position_a, collider_a, mtv);
    *mtv *= -1.0f;
    return collision;
}

inline bool HitTestCircleCapsule(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{
    return HitTestCircleCapsule(position_a, collider_a.circle.radius,
        position_b, collider_b.capsule.direction, collider_b.capsule.half_length, collider_b.capsule.radius, mtv);
}

inline bool HitTestCapsuleCircle(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{
    bool collision = HitTestCircleCapsule(position_b, collider_b, position_a, collider_a, mtv);
    *mtv = *mtv * -1.0f;
    return collision;
}

inline bool HitTestCapsuleAABB(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{
    return HitTestCapsuleAABB(position_a, collider_a.capsule.direction, collider_a.capsule.half_length, collider_a.capsule.radius, position_b, collider_b.aabb.half_extents, mtv);
}

inline bool HitTestAABBCapsule(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{
    bool collision = HitTestCapsuleAABB(position_b, collider_b, position_a, collider_a, mtv);
    *mtv = *mtv * -1.0f;
    return collision;
}

inline bool HitTestCirclePlane(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{
    return HitTestPlaneCircle(position_b, collider_b.plane.normal, position_a, collider_a.circle.radius, mtv);
}

inline bool HitTestPlaneCircle(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{
    return HitTestPlaneCircle(position_a, collider_a.plane.normal, position_b, collider_b.circle.radius, mtv);
}

inline bool HitTestAABBPlane(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{

    return HitTestPlaneAABB(position_b, collider_b.plane.normal, position_a, collider_a.aabb.half_extents, mtv);
}

inline bool HitTestPlaneAABB(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{
    return HitTestPlaneAABB(position_a, collider_a.plane.normal, position_b, collider_b.aabb.half_extents, mtv);
}

inline bool HitTestCapsulePlane(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{
    return HitTestPlaneCapsule(position_b, collider_b.plane.normal, position_a, collider_a.capsule.direction, collider_a.capsule.half_length, collider_a.capsule.radius, mtv);
}

inline bool HitTestPlaneCapsule(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv)
{
    return HitTestPlaneCapsule(position_a, collider_a.plane.normal, position_b, collider_b.capsule.direction, collider_b.capsule.half_length, collider_b.capsule.radius, mtv);
}

// MTV resolves B from A
using CollisionFunc = bool(*)(Vector2 position_a, Collider collider_a, Vector2 position_b, Collider collider_b, Vector2* mtv);

CollisionFunc COLLISION_TABLE[5][5] =
{
    //    NONE         CIRCLE                AABB                Capsule               PLANE
        { HitTestNone, HitTestNone,          HitTestNone,        HitTestNone,          HitTestNone         }, // NONE
        { HitTestNone, HitTestCircles,       HitTestCircleAABB,  HitTestCircleCapsule, HitTestCirclePlane  }, // CIRCLE
        { HitTestNone, HitTestAABBCircle,    HitTestAABBs,       HitTestAABBCapsule,   HitTestAABBPlane    }, // BOX
        { HitTestNone, HitTestCapsuleCircle, HitTestCapsuleAABB, HitTestCapsules,      HitTestCapsulePlane }, // CAPSULE
        { HitTestNone, HitTestPlaneCircle,   HitTestPlaneAABB,   HitTestPlaneCapsule,  HitTestNone         }  // PLANE
};
