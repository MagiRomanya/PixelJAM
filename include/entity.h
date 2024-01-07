#ifndef ENTITY_H_
#define ENTITY_H_

#include "raylib.h"
#include "raymath.h"
#include "physics.h"

typedef enum {
NO_COLLIDE,
PLAYER_COLLIDE,
CABLE_COLLIDE,
PLAYER_CABLE_COLLIDE,
} COLLISION_MASK ;

typedef struct {
    COLLISION_MASK collision_mask;
    float friction_damping;
    CapsuleCollider capsule_collider;
} GameCollider;

struct GameColliderList{
    GameCollider* colliders;
    size_t size;
    size_t capacity;
};

GameColliderList createGameColliderList();

void addGameColliderToList(GameColliderList* c_list, GameCollider* c);

GameCollider* getGameColliderFromList(GameColliderList* c_list, size_t index);

void clearGameColliderList(GameColliderList* c_list);

void destroyGameColliderList(GameColliderList* c_list);

struct Player {
    Vector2 position;
    Vector2 velocity;
    Vector2 force;

    // Render
    Texture2D spriteIdle;
    Texture2D spriteIdleL;
    Texture2D spriteRun;
    Texture2D spriteRunL;
    Texture2D spriteJump;
    Texture2D spriteJumpL;

    // Input
    Vector2 input_vector;
    float facing_direction;
    bool canDoubleJump;

    // Physics
    bool grounded;
    float mass;
    CapsuleCollider base_capsule_collider;
    Vector2 hatPosition;
    Vector2 hatVelocity;
};

void playerFrameReset(Player* player);

CapsuleCollider playerComputeCollider(const Player* player);

Player createPlayer();

void playerJump(Player* player);

void renderPlayer(Player* player);

Vector2 computePlayerHandPosition(Player* player);

void destroyPlayer(Player* player);

typedef struct {
    int SpriteID;
    Vector2 Position;
} Tile;

typedef struct {
    Tile *tiles;
    size_t size;
    size_t capacity;
} TileMap;

TileMap createTileMap();

void addTileToMap(TileMap* t_map, int sprite_id, int tile_x, int tile_y);

Tile* getTileFromMap(TileMap* t_map, size_t index);

void clearTileMap(TileMap* t_map);

void destroyGameTileMap(TileMap* t_map);

void renderTileMap(TileMap* t_map);

#endif // ENTITY_H_
