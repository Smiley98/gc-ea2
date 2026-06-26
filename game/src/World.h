#pragma once

std::vector<EntityHit> WorldDetectCollisions(const std::vector<Entity>& entities);
void WorldResolveCollisions(std::vector<EntityHit> hits);
