#include "entity.h"
#include "physics.h"
#include "raylib.h"
#include "raymath.h"
#include "sprite_manager.h"
#include <stddef.h>
#include <stdio.h>

#define GAME_COLLIDER_LIST_CAPACITY 1e3*sizeof(GameCollider)

GameColliderList createGameColliderList() {
    GameColliderList c_list;
    c_list.size = 0;
    c_list.capacity = GAME_COLLIDER_LIST_CAPACITY;
    c_list.colliders = malloc(c_list.capacity);
    return c_list;
}

void addGameColliderToList(GameColliderList* c_list, GameCollider* e) {
    c_list->colliders[sizeof(GameCollider)*c_list->size] = *e;
    c_list->size += 1;
}

GameCollider* getGameColliderFromList(GameColliderList* c_list, size_t index) {
    return &c_list->colliders[sizeof(GameCollider) * index];
}

void clearGameColliderList(GameColliderList* c_list) { c_list->size = 0; }

void destroyGameColliderList(GameColliderList* c_list) {
    free(c_list->colliders);
}

TileMap createTileMap() {
    TileMap t_map;
    t_map.size = 0;
    t_map.capacity = GAME_COLLIDER_LIST_CAPACITY;
    t_map.tiles = malloc(t_map.capacity);
    return t_map;
}

void addTileToMap(TileMap* t_map, int sprite_id, int tile_x, int tile_y) {
    if (sprite_id > getMaxSprites()) return;
    Tile t;
    t.SpriteID = sprite_id;
    t.Position = (Vector2){tile_x, tile_y};
    t_map->tiles[sizeof(Tile)*t_map->size] = t;
    t_map->size += 1;
}

Tile* getTileFromMap(TileMap* t_map, size_t index) {
    return &t_map->tiles[sizeof(Tile) * index];
}

void clearTileMap(TileMap* t_map) { t_map->size = 0; }

void destroyGameTileMap(TileMap* t_map) {
    free(t_map->tiles);
}

Player createPlayer() {
    Player player = {0};
    player.spriteIdle = LoadTexture("assets/sprites/player-idle.png");
    player.spriteRun = LoadTexture("assets/sprites/player-run.png");
    player.spriteIdleL = LoadTexture("assets/sprites/player-idleL.png");
    player.spriteRunL = LoadTexture("assets/sprites/player-runL.png");
    player.facing_direction = 1.0f;
    player.mass = 1.0f;

    const float playerWidth = 32;
    const float playerHeight = 31;
    const float capsuleRadius = 7;
    const float padding = 0;
    Vector2 x1 = {playerWidth/2, capsuleRadius + padding};
    Vector2 x2 = {playerWidth/2, playerHeight - capsuleRadius};
    player.base_capsule_collider = (CapsuleCollider){x1,x2, capsuleRadius};
    return player;
}

Vector2 computePlayerHandPosition(Player* player) {
    if (player->facing_direction > 0) {
        const Vector2 handPosition = {32,16};
        return Vector2Add(handPosition, player->position);
    }
    const Vector2 handPosition = {0,16};
    return Vector2Add(handPosition, player->position);
};

void playerJump(Player* player) {
    printf("Player jump!\n");
    player->velocity.y = 0;
    player->input_vector.y -= 100.0f;
}

void renderPlayer(Player* player) {
    DrawCircleV(computePlayerHandPosition(player), 5, GREEN);
    const float velThreshold = 2.0f;
    if (fabsf(player->velocity.x) < velThreshold) {
        // IDLE
        if (player->facing_direction > 0) {
            DrawTexture(player->spriteIdle, player->position.x, player->position.y, WHITE);
        }
        else {
            DrawTexture(player->spriteIdleL, player->position.x, player->position.y, WHITE);
        }
    }
    else {
        // Player Run animation
        Texture2D runTexture;
        if (player->facing_direction > 0) runTexture = player->spriteRun;
        else runTexture = player->spriteRunL;

        const int nFrames = 6;
        const int frameSpeed = 6;
        static int frameCounter = 0;
        frameCounter++;
        static int currentFrame = 0;
        const float width = (float)player->spriteRun.width / ((float) nFrames);
        Rectangle frameRec = {0.0f, 0.0f, width, (float)player->spriteRun.height};
        if (frameCounter >= 1.0f/(GetFrameTime()*frameSpeed)) {
            frameCounter = 0;
            currentFrame++;
            if (currentFrame > nFrames - 1) currentFrame = 0;
        }
        frameRec.x = currentFrame * width;
        DrawTextureRec(runTexture, frameRec, player->position, WHITE);
    }
}

void destroyPlayer(Player* player) {
    UnloadTexture(player->spriteIdle);
    UnloadTexture(player->spriteRun);

    UnloadTexture(player->spriteIdleL);
    UnloadTexture(player->spriteRunL);
}

void playerFrameReset(Player* player) {
    player->input_vector = (Vector2){0};
    if (player->velocity.x != 0) player->facing_direction = player->velocity.x;
}

CapsuleCollider playerComputeCollider(const Player* player) {
    const Vector2 x1 = Vector2Add(player->base_capsule_collider.x1, player->position);
    const Vector2 x2 = Vector2Add(player->base_capsule_collider.x2, player->position);
    CapsuleCollider c = {x1, x2, player->base_capsule_collider.radius};
    return c;
}

void renderTileMap(TileMap* t_map) {
    for (size_t i = 0; i < t_map->size; i++) {
        Tile* tile = getTileFromMap(t_map, i);
        Texture2D texture = getSpriteFromID(tile->SpriteID);
        DrawTexture(texture, tile->Position.x, tile->Position.y, WHITE);
    }
}
