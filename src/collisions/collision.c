#include "collision.h"

#include <math.h>
#include <raylib.h>

static float get_tile_size(const Level* level){
    return (level && level->tileSize > 0.0f) ? level->tileSize : 32.0f;
}

static void resolve_horizontal(Player* player, const Level* level, CollisionResult* result, Rectangle* bounds){
    if (!player || !level || !result || !bounds)
        return;

    if (player->velocity.x == 0.0f)
        return;

    const float tileSize = get_tile_size(level);
    const float epsilon = 0.001f;
    const int dir = (player->velocity.x > 0.0f)? 1 : -1;

    while (true){
        const int left = (int)floorf(bounds->x / tileSize);
        const int right = (int)floorf((bounds->x + bounds->width - epsilon)/ tileSize);
        const int top = (int)floorf(bounds->y/ tileSize);
        const int bottom = (int)floorf((bounds->y + bounds->height - epsilon)/ tileSize);
        bool collided = false;

        if (dir > 0){
            const int tx = right;
            for (int ty = top; ty<=bottom; ty++){
                if (!level_is_tile_solid(level, tx, ty))
                    continue;
                
                Rectangle tileRect = level_tile_bounds(level, tx, ty);
                if (!CheckCollisionRecs(*bounds, tileRect))
                    continue;
                
                bounds->x = tileRect.x - bounds->width;
                player->velocity.x = 0.0f;
                result->hitWallRight = true;
                collided = true;
                TraceLog(LOG_DEBUG, "[Collision] parede direita em tile(%d,%d)", tx, ty);
                break;
            }
        }else{
            const int tx = left;
            for (int ty = top; ty<=bottom; ty++){
                if (!level_is_tile_solid(level, tx, ty))
                    continue;
                
                Rectangle tileRect = level_tile_bounds(level, tx, ty);
                if (!CheckCollisionRecs(*bounds, tileRect))
                    continue;
                
                bounds->x = tileRect.x + tileRect.width;
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

static void resolve_vertical(Player* player, const Level* level, CollisionResult* result, Rectangle* bounds){
    if (!player || !level || !result || !bounds)
        return;

    if (player->velocity.y == 0.0f){
        player->isOnGround = false;
        return;
    }

    const float tileSize = get_tile_size(level);
    const float epsilon = 0.001f;
    const int dir = (player->velocity.y > 0.0f) ? 1 : -1;

    player->isOnGround = false;

    while (true){
        const int left = (int)floorf(bounds->x / tileSize);
        const int right = (int)floorf((bounds->x + bounds->width - epsilon)/ tileSize);
        const int top = (int)floorf(bounds->y / tileSize);
        const int bottom = (int)floorf((bounds->y + bounds->height - epsilon) / tileSize);

        bool collided = false;

        if (dir > 0){ // caindo
            const int ty = bottom;
            for (int tx = left; tx<= right; tx++){
                if(!level_is_tile_solid(level, tx, ty))
                    continue;

                Rectangle tileRect = level_tile_bounds(level, tx, ty);
                if (!CheckCollisionRecs(*bounds, tileRect))
                    continue;
                
                bounds->y = tileRect.y - bounds->height;
                player->velocity.y = 0.0f;
                player->isOnGround = true;
                result->hitGround = true;
                collided = true;
                TraceLog(LOG_DEBUG, "[Collision] piso em tile(%d,%d)", tx, ty);
                break;
            }
        }else{ // subindo
            const int ty = top;
            for (int tx = left; tx<=right; tx++){
                if (!level_is_tile_solid(level, tx, ty))
                    continue;
                
                Rectangle tileRect = level_tile_bounds(level, tx, ty);
                if (!CheckCollisionRecs(*bounds, tileRect))
                    continue;

                bounds->y = tileRect.y + tileRect.height;
                player->velocity.y = 0.0f;
                result->hitCeiling = true;
                collided = true;
                TraceLog(LOG_DEBUG, "[Collision] teto em tile(%d,%d)", tx, ty);
                break;
            }
        }
        if (!collided)
            break;

        if (player->velocity.y == 0.0f) // não cair em loop
            break;
    }
}

void collision_result_reset(CollisionResult* result){
    if (!result)
        return;

    result->hitGround = false;
    result->hitCeiling = false;
    result->hitTrap = false;
    result->hitWallLeft = false;
    result->hitWallRight = false;
    result->reachedGoal = false;
    result->died = false;
}

void collisions_resolve_player_map(Player* player, const Level* level, CollisionResult* result){
    if (!player || !level || !result)
        return;

    Rectangle bounds = player_get_bounds(player);

    resolve_horizontal(player, level, result, &bounds);
    resolve_vertical(player, level, result, &bounds);

    player->position.x = bounds.x + bounds.width * 0.5f;
    player->position.y = bounds.y + bounds.height;
    player_update_hitbox(player);
}

void collisions_check_player_traps(Player* player, const TrapSet* trapSet, CollisionResult* result){
    if (!player || !trapSet || !result)
        return;

    Rectangle playerBounds = player_get_bounds(player);

    for (size_t i = 0; i<trapSet->count; i++){
        const Trap* trap = trap_set_get(trapSet, i);
        if (!trap || !trap->active)
            continue;

        if (CheckCollisionRecs(playerBounds, trap->hitbox)){
            result->hitTrap = true;
            result->died = true;
            player->isAlive = false;

            TraceLog(LOG_INFO, "[Collision] atingiu trap idx=%zu type=%d", i, trap->type);
            break;
        }
    }
}

void collisions_check_goal(Player* player, const Rectangle* goalRect, CollisionResult* result){
    if (!player || !goalRect || !result)
        return;

    Rectangle playerBounds = player_get_bounds(player);
    if (CheckCollisionRecs(playerBounds, *goalRect)){
        result->reachedGoal = true;
    }
}
