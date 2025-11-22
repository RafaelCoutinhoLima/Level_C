#include "collision.h"

#include <math.h>
#include <raylib.h>

//------------------------------------------------------------------------------
// Helpers
//------------------------------------------------------------------------------
static float get_tile_size(const Level* level) {
    return (level && level->tileSize > 0.0f) ? level->tileSize : 32.0f;
}

//------------------------------------------------------------------------------
// Horizontal resolution
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// Vertical resolution
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// Public API
//------------------------------------------------------------------------------
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
    // Tolerância para subir na plataforma One-Way (pixels)
    const float kOneWayThreshold = 8.0f; 
    for (size_t i = 0; i < trapSet->count; i++) {
        // Usamos get_mutable para poder alterar o estado do Bloco D
        Trap* trap = trap_set_get_mutable(trapSet, i);
        if (!trap || !trap->active) continue;
        // Bloco D "OFF" é invisível e intangível
        if (trap->type == TRAP_TYPE_DISAPPEARING && trap->state == TRAP_STATE_OFF) continue;
        // Verifica colisão básica AABB
        if (CheckCollisionRecs(pRect, trap->hitbox)) {
            switch (trap->type) {
                //MORTAL (Espinhos, Fogo)
                case TRAP_TYPE_SPIKE:
                case TRAP_TYPE_FIRE:
                case TRAP_TYPE_MINE:
                case TRAP_TYPE_PROJECTILE:
                    result->hitTrap = true;
                    result->died    = true;
                    player->isAlive = false;
                    TraceLog(LOG_INFO, "[Collision] Trap mortal idx=%zu", i);
                    break;

                //PLATAFORMA FALSA (F)
                case TRAP_TYPE_FALSE:
                    // Pode adicionar um log de debug se quiser:
                    // TraceLog(LOG_DEBUG, "Atravessou plataforma falsa");
                    break;
                //ONE-WAY (H)
                case TRAP_TYPE_ONEWAY: {
                    // Lógica: Só colide se estiver CAINDO
                    bool isFalling = player->velocity.y > 0;
                    
                    // E se os pés estavam "acima" ou "quase acima" da plataforma
                    // (pRect.y + pRect.height) é o pé do jogador
                    float feetY = pRect.y + pRect.height;
                    float trapTop = trap->hitbox.y;
                    // Verificação de altura com tolerância
                    bool isAbove = (feetY <= trapTop + kOneWayThreshold + (player->velocity.y * GetFrameTime()));
                    if (isFalling && isAbove) {
                        // Encaixa o jogador em cima
                        player->position.y = trap->hitbox.y; // Define pé na altura do topo
                        player_update_hitbox(player); // Recalcula bounds
                        player->velocity.y = 0.0f;
                        player->isOnGround = true;
                        result->hitGround = true;
                    }
                } break;
                //BLOCO TEMPORÁRIO (D)
                case TRAP_TYPE_DISAPPEARING: {
                    // Comporta-se como sólido normal (chão/parede)
                    // Simplificação: Vamos tratar como plataforma sólida vindo de cima
                    // Se quiser colisão lateral perfeita, teria que usar lógica parecida com resolve_horizontal
                    
                    // Check simples: está pisando em cima?
                    float feetY = pRect.y + pRect.height;
                    float overlapY = feetY - trap->hitbox.y;
                    // Se está caindo e bateu no topo (com pequena margem de penetração)
                    if (player->velocity.y >= 0 && overlapY > 0 && overlapY < 16.0f) {
                        // Resolve física
                        player->position.y = trap->hitbox.y;
                        player_update_hitbox(player);
                        player->velocity.y = 0.0f;
                        player->isOnGround = true;
                        result->hitGround = true;
                        // Ativa o timer se estiver sólido
                        if (trap->state == TRAP_STATE_ACTIVE) {
                            trap->state = TRAP_STATE_WARNING;
                            trap->timer = 1.0f; // 1 segundo tremendo antes de sumir
                            TraceLog(LOG_INFO, "Bloco D ativado! idx=%zu", i);
                        }
                    }
                } break;

                default: break;
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