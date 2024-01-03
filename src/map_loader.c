#include "map_loader.h"
#include "entity.h"
#include "raylib.h"
#include <stdio.h>

void loadMap(char *filename , TileMap *tiles, GameColliderList *colliders, Cable *cable, Player *player) {
    Image image = LoadImage(filename);
    Color *pixels = LoadImageColors(image);
    int map_width = image.width;
    int map_height = image.height;

    bool startNewCollider = true;
    int x1, x2, y;
    const int capsuleRadius = 8;

    bool playerInitialPositionFound = false;
    bool cableInitialAnchorFound = false;

    // Read color value from each pixel
    for (int j = 0; j < map_height; j++) {
        for (int i = 0; i < map_width; i++) {
            Color pixel = pixels[j * map_width + i];
            if (pixel.a != 0) {
                // int sprite_id = pixel.b;
                // if (pixel.g) sprite_id += 256 + pixel.g;
                addTileToMap(tiles, pixel.b, i*16, j*16);

                printf("%i\n", pixel.r);
                if (pixel.r && startNewCollider) {
                    printf("hello %i\n", pixel.r);
                    x1 = i*16 + 8;
                    y = j*16 + 8;
                    startNewCollider = false;
                } else if (!pixel.r && !startNewCollider) {
                    x2 = (i-1)*16;
                    CapsuleCollider collider = createCapsule(x1, x2, y, capsuleRadius);
                    GameCollider gameCollider = {0};
                    gameCollider.capsule_collider = collider;
                    gameCollider.collision_mask = PLAYER_CABLE_COLLIDE;
                    gameCollider.friction_damping = 1.0f;
                    addGameColliderToList(colliders, &gameCollider);
                    startNewCollider = true;
                }
            }
        }
        if (!startNewCollider) {
            x2 = map_width * 16 - 8;
            CapsuleCollider collider = createCapsule(x1, x2, y, capsuleRadius);
            GameCollider gameCollider = {0};
            gameCollider.capsule_collider = collider;
            gameCollider.collision_mask = PLAYER_CABLE_COLLIDE;
            gameCollider.friction_damping = 1.0f;
            addGameColliderToList(colliders, &gameCollider);
            startNewCollider = true;
        }
    }
    if (!playerInitialPositionFound) {
        printf("WARNING: No player in the level\n");
    }
    if (!cableInitialAnchorFound) {
        printf("WARNING: No cable starting point in the level\n");
    }

    UnloadImageColors(pixels);
    UnloadImage(image);

}
