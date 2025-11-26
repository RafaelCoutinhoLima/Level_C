#include "collision.h"
#include <math.h>
#include <raylib.h>

// Helpers
static float get_tile_size(const Level* level) {
    return (level && level->tileSize > 0.0f) ? level->tileSize : 32.0f;
}
// Horizontal resolution
static void resolve_horizontal(Player* player, const Level* level, CollisionResult* result, Rectangle* bounds) {
    if (!player || !level || !result || !bounds)
        return;

    const float tileSize = get_tile_size(level);
    const float epsilon  = 0.001f;
    const float vx       = player->velocity.x;
    int dir = (vx > 0.0f) ? 1 : (vx < 0.0f ? -1 : 0);

    // Se a velocidade foi zerada em um frame anterior, ainda verificamos qual lado está penetrando.
    if (dir == 0) {
        const int leftIndex  = (int)floorf(bounds->x / tileSize);
        const int rightIndex = (int)floorf((bounds->x + bounds->width - epsilon) / tileSize);
        const int top        = (int)floorf(bounds->y / tileSize);
        const int bottom     = (int)floorf((bounds->y + bounds->height - epsilon) / tileSize);

        bool  rightHit     = false;
        float overlapRight = 0.0f;
        const int rx = rightIndex;
        for (int ty = top; ty <= bottom; ty++) {
            if (!level_is_tile_solid(level, rx, ty)) continue;
            Rectangle tileRect = level_tile_bounds(level, rx, ty);
            if (!CheckCollisionRecs(*bounds, tileRect)) continue;
            const float overlap = (bounds->x + bounds->width) - tileRect.x;
            if (overlap > overlapRight) overlapRight = overlap;
            rightHit = true;
        }

        bool  leftHit     = false;
        float overlapLeft = 0.0f;
        const int lx = leftIndex;
        for (int ty = top; ty <= bottom; ty++) {
            if (!level_is_tile_solid(level, lx, ty)) continue;
            Rectangle tileRect = level_tile_bounds(level, lx, ty);
            if (!CheckCollisionRecs(*bounds, tileRect)) continue;
            const float overlap = (tileRect.x + tileRect.width) - bounds->x;
            if (overlap > overlapLeft) overlapLeft = overlap;
            leftHit = true;
        }

        if (!rightHit && !leftHit)
            return;

        dir = (rightHit && (!leftHit || overlapRight >= overlapLeft)) ? 1 : -1;
    }

    while (true) {
        const int left   = (int)floorf(bounds->x / tileSize);
        const int right  = (int)floorf((bounds->x + bounds->width - epsilon) / tileSize);
        const int top    = (int)floorf(bounds->y / tileSize);
        const int bottom = (int)floorf((bounds->y + bounds->height - epsilon) / tileSize);

        bool collided = false;

        if (dir > 0) {
            const int tx = right;
            for (int ty = top; ty <= bottom; ty++) {
                if (!level_is_tile_solid(level, tx, ty)) continue;
                Rectangle tileRect = level_tile_bounds(level, tx, ty);
                if (!CheckCollisionRecs(*bounds, tileRect)) continue;

                bounds->x = tileRect.x - bounds->width - epsilon;
                player->velocity.x = 0.0f;
                result->hitWallRight = true;
                collided = true;
                TraceLog(LOG_DEBUG, "[Collision] parede direita em tile(%d,%d)", tx, ty);
                break;
            }
        } else {
            const int tx = left;
            for (int ty = top; ty <= bottom; ty++) {
                if (!level_is_tile_solid(level, tx, ty)) continue;
                Rectangle tileRect = level_tile_bounds(level, tx, ty);
                if (!CheckCollisionRecs(*bounds, tileRect)) continue;

                bounds->x = tileRect.x + tileRect.width + epsilon;
                player->velocity.x = 0.0f;
                result->hitWallLeft = true;
                collided = true;
                TraceLog(LOG_DEBUG, "[Collision] parede esquerda em tile(%d,%d)", tx, ty);
                break;
            }
        }

        if (!collided)
            break;
    }
}
// Vertical resolution
static void resolve_vertical(Player* player, const Level* level, CollisionResult* result, Rectangle* bounds) {
    if (!player || !level || !result || !bounds)
        return;

    if (player->velocity.y == 0.0f) {
        player->isOnGround = false;
        return;
    }

    const float tileSize = get_tile_size(level);
    const float epsilon  = 0.001f;
    const int dir        = (player->velocity.y > 0.0f) ? 1 : -1;
    const float centerX  = bounds->x + bounds->width * 0.5f;
    const float minPen   = 0.15f;

    player->isOnGround = false;

    while (true) {
        const int left   = (int)floorf(bounds->x / tileSize);
        const int right  = (int)floorf((bounds->x + bounds->width - epsilon) / tileSize);
        const int top    = (int)floorf(bounds->y / tileSize);
        const int bottom = (int)floorf((bounds->y + bounds->height - epsilon) / tileSize);

        bool collided = false;

        if (dir > 0) { // caindo
            const int ty = bottom;
            for (int tx = left; tx <= right; tx++) {
                if (!level_is_tile_solid(level, tx, ty)) continue;
                Rectangle tileRect = level_tile_bounds(level, tx, ty);
                if (!CheckCollisionRecs(*bounds, tileRect)) continue;

                if (centerX < tileRect.x - epsilon || centerX > tileRect.x + tileRect.width + epsilon)
                    continue;

                const float penetration = (bounds->y + bounds->height) - tileRect.y;
                if (penetration <= 0.0f)
                    continue;

                if (penetration < minPen) {
                    bounds->y -= (penetration + epsilon);
                    continue;
                }

                bounds->y = tileRect.y - bounds->height;
                player->velocity.y = 0.0f;
                player->isOnGround = true;
                result->hitGround = true;
                collided = true;
                TraceLog(LOG_DEBUG, "[Collision] piso em tile(%d,%d)", tx, ty);
                break;
            }
        } else { // subindo
            const int ty = top;
            for (int tx = left; tx <= right; tx++) {
                if (!level_is_tile_solid(level, tx, ty)) continue;
                Rectangle tileRect = level_tile_bounds(level, tx, ty);
                if (!CheckCollisionRecs(*bounds, tileRect)) continue;

                bounds->y = tileRect.y + tileRect.height + epsilon;
                player->velocity.y = 0.0f;
                result->hitCeiling = true;
                collided = true;
                TraceLog(LOG_DEBUG, "[Collision] teto em tile(%d,%d)", tx, ty);
                break;
            }
        }

        if (!collided)
            break;

        if (player->velocity.y == 0.0f)
            break;
    }
}

// Public API
void collision_result_reset(CollisionResult* result) {
    if (!result)
        return;

    result->hitGround   = false;
    result->hitCeiling  = false;
    result->hitTrap     = false;
    result->hitWallLeft = false;
    result->hitWallRight= false;
    result->reachedGoal = false;
    result->died        = false;
}

void collisions_resolve_player_map(Player* player, const Level* level, CollisionResult* result){
    if (!player || !level || !result)
        return;

    Rectangle bounds = player_get_bounds(player);
    const float skinX = 0.1f;

    bounds.x += skinX;
    bounds.width -= skinX * 2.0f;

    const bool horizontalFirst = (player->velocity.y < 0.0f) && (fabsf(player->velocity.x) > 0.0f);

    if (horizontalFirst){
        resolve_horizontal(player, level, result, &bounds);
        resolve_vertical(player, level, result, &bounds);
    }else{
        resolve_vertical(player, level, result, &bounds);
        resolve_horizontal(player, level, result, &bounds);
    }

    const float leftEdge  = bounds.x;
    const float rightEdge = bounds.x + bounds.width;
    const float center    = bounds.x + bounds.width * 0.5f;

    bounds.width += skinX * 2.0f;

    if (result->hitWallRight){
        bounds.x = rightEdge - bounds.width;
    }else if (result->hitWallLeft){
        bounds.x = leftEdge;
    }else{
        bounds.x = center - bounds.width * 0.5f;
    }

    player->position.x = bounds.x + bounds.width * 0.5f;
    player->position.y = bounds.y + bounds.height;
    player_update_hitbox(player);
}

void collisions_check_player_traps(Player* player, TrapSet* trapSet, CollisionResult* result) {
    if (!player || !trapSet || !result) return;

    Rectangle pRect = player_get_bounds(player);
    const float kOneWayThreshold = 8.0f;

    for (size_t i = 0; i < trapSet->count; i++) {
        Trap* trap = trap_set_get_mutable(trapSet, i);
        if (!trap || !trap->active) continue;

        if (trap->type == TRAP_TYPE_DISAPPEARING && trap->state == TRAP_STATE_OFF) continue;

        Rectangle effectiveHitbox = trap->hitbox;
        
        if (trap->type == TRAP_TYPE_SPIKE || trap->type == TRAP_TYPE_FALSE) {
            float marginX = 6.0f;    
            float heightRatio = 0.5f; 
            effectiveHitbox.x += marginX;
            effectiveHitbox.width -= (marginX * 2);
            effectiveHitbox.y += (effectiveHitbox.height * (1.0f - heightRatio));
            effectiveHitbox.height *= heightRatio;
        }

        if (CheckCollisionRecs(pRect, effectiveHitbox)) {
            switch (trap->type) {
                case TRAP_TYPE_SPIKE:
                    result->hitTrap = true;
                    result->died = true;
                    player->isAlive = false;
                    TraceLog(LOG_INFO, "Player morreu em espinho!");
                    break;

                case TRAP_TYPE_FALSE:
                    trap->active = true; 
                    TraceLog(LOG_INFO, "Trap Falsa ativada!");
                    break;
                    
                case TRAP_TYPE_ONEWAY: {
                    bool isFalling = player->velocity.y > 0;
                    float feetY = pRect.y + pRect.height;
                    float trapTop = trap->hitbox.y; 
                    bool isAbove = (feetY <= trapTop + kOneWayThreshold + (player->velocity.y * GetFrameTime()));

                    if (isFalling && isAbove) {
                        player->position.y = trap->hitbox.y; // Mantém posição original
                        player_update_hitbox(player);
                        player->velocity.y = 0.0f;
                        player->isOnGround = true;
                        result->hitGround = true;
                    }
                } break;

                case TRAP_TYPE_DISAPPEARING: {
                    float playerCenterX = pRect.x + pRect.width / 2.0f;
                    float trapCenterX = trap->hitbox.x + trap->hitbox.width / 2.0f;
                    float distanceHorizontal = fabsf(playerCenterX - trapCenterX);
                    
                    float playerFeetY = pRect.y + pRect.height;
                    float trapTopY = trap->hitbox.y;
                    float verticalDistance = fabsf(playerFeetY - trapTopY);

                    if (distanceHorizontal <= 64.0f && verticalDistance <= 16.0f && trap->state == TRAP_STATE_ACTIVE){
                        trap->state = TRAP_STATE_OFF;
                        TraceLog(LOG_INFO, "Bloco D desapareceu, idx=%zu", i);
                    }

                    if (trap->state == TRAP_STATE_ACTIVE){
                        float feetY = pRect.y +pRect.height;
                        float overlapY = feetY - trap->hitbox.y;

                        if (player->velocity.y >= 0 && overlapY > 0 && overlapY <16.0f){
                            player->position.y = trap->hitbox.y;
                            player_update_hitbox(player);
                            player->velocity.y = 0.0f;
                            player->isOnGround = true;
                            result->hitGround = true;
                        }
                    }
                    
                } break;
                default: 
                    break;
            }
        }
    }
}
void collisions_check_goal(Player* player, const Rectangle* goalRect, CollisionResult* result) {
    if (!player || !goalRect || !result)
        return;

    Rectangle playerBounds = player_get_bounds(player);
    if (CheckCollisionRecs(playerBounds, *goalRect))
        result->reachedGoal = true;
}