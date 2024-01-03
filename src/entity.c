#include "entity.h"
#include "physics.h"
#include "raylib.h"
#include "sprite_manager.h"
#include <stddef.h>

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
    player.sprite = LoadTexture("assets/sprites/Tree001.png");

    player.mass = 1.0f;

    const float playerWidth = 32;
    const float playerHeight = 32;
    const float capsuleRadius = 10;
    const float padding = 10;
    Vector2 x1 = {playerWidth/2, capsuleRadius + padding};
    Vector2 x2 = {playerWidth/2, playerHeight - capsuleRadius};
    player.base_capsule_collider = (CapsuleCollider){x1,x2, capsuleRadius};
    return player;
}

void destroyPlayer(Player* player) {
    UnloadTexture(player->sprite);
}

void playerFrameReset(Player* player) {
    player->input_vector = (Vector2){0};
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
