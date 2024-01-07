#include <stdio.h>
#include "map_loader.h"
#include "entity.h"
#include "raylib.h"
#include "sprite_manager.h"
#include "appliance.h"

void loadMap(char *filename , TileMap *t_map, GameColliderList *colliders, ApplianceList *a_list, Cable *cable, Player *player) {
    Image image = LoadImage(filename);
    Color *pixels = LoadImageColors(image);
    int map_width = image.width;
    int map_height = image.height;

    bool startNewCollider = true;
    int x1, x2, y;
    const int capsuleRadius = 8;

    bool playerInitialPositionFound = false;    
    int floor = GetRandomValue(8,12);

    // Read color value from each pixel
    for (int j = 0; j < map_height; j++) {
        for (int i = 0; i < map_width; i++) {
            Color pixel = pixels[j * map_width + i];
            // Place sprites
            Vector2 position = {16*i, 16*j};
            if (pixel.a != 0) {
                if (pixel.r == 100) {
                    int wall = GetRandomValue(1,5);
                    addTileToMap(t_map, wall, position.x, position.y);
                }
                else if (pixel.r == 255) {
                    addTileToMap(t_map, floor, position.x, position.y);
                }
                else if (pixel.g) {
                    switch (pixel.g) {
                        case 255:
                        {
                            playerInitialPositionFound = true;
                            cable->anchors[0].position = position;
                            player->position = (Vector2){i*16, j*16 - 16};
                            player->hatPosition = position;
                            // Control screen
                            addTileToMap(t_map, SPRITE_CONTROLS_INGAME_ID, position.x - 64, position.y - 50);
                            break;
                        }
                        case 32:
                        {
                            addApplianceToList(a_list, createAppliance(WASHING_MACHINE, position));
                            break;
                        }
                        case 64:
                        {
                            addApplianceToList(a_list, createAppliance(BLENDER, position));
                            break;
                        }
                        case 92:
                        {
                            addApplianceToList(a_list, createAppliance(TELEVISION, position));
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            // Create colliders
            bool HorizontalCollider = pixel.r == 255;
            if (HorizontalCollider && startNewCollider) {
                x1 = i * 16 + 8;
                y = j * 16 + 8;
                startNewCollider = false;
            } else if (!HorizontalCollider && !startNewCollider) {
                x2 = (i - 1) * 16 + 8;
                CapsuleCollider collider = createCapsule(x1, x2, y, capsuleRadius);
                GameCollider gameCollider = {0};
                gameCollider.capsule_collider = collider;
                gameCollider.collision_mask = PLAYER_CABLE_COLLIDE;
                gameCollider.friction_damping = 4.0f;
                addGameColliderToList(colliders, &gameCollider);
                startNewCollider = true;
            }
        }
        if (!startNewCollider) {
            x2 = map_width * 16 - 8;
            CapsuleCollider collider = createCapsule(x1, x2, y, capsuleRadius);
            GameCollider gameCollider = {0};
            gameCollider.capsule_collider = collider;
            gameCollider.collision_mask = PLAYER_CABLE_COLLIDE;
            gameCollider.friction_damping = 4.0f;
            addGameColliderToList(colliders, &gameCollider);
            startNewCollider = true;
        }
    }

    int x, y1, y2;
    for (int i = 0; i < map_width; i++) {
        for (int j = 0; j < map_height; j++) {
            Color pixel = pixels[j * map_width + i];

            // Create colliders
            bool VerticalCollider = pixel.r == 100;
            if (VerticalCollider && startNewCollider) {
                x = i * 16 + 8;
                y1 = j * 16 + 8;
                startNewCollider = false;
            } else if (!VerticalCollider && !startNewCollider) {
                y2 = (j-1) * 16 + 8;
                CapsuleCollider collider = createCapsuleVertical(x, y1, y2, capsuleRadius);
                GameCollider gameCollider = {0};
                gameCollider.capsule_collider = collider;
                gameCollider.collision_mask = PLAYER_CABLE_COLLIDE;
                gameCollider.friction_damping = 0.0f;
                addGameColliderToList(colliders, &gameCollider);
                startNewCollider = true;
            }
        }
        if (!startNewCollider) {
            y2 = map_height * 16 - 8;
            CapsuleCollider collider = createCapsuleVertical(x, y1, y2, capsuleRadius);
            GameCollider gameCollider = {0};
            gameCollider.capsule_collider = collider;
            gameCollider.collision_mask = PLAYER_CABLE_COLLIDE;
            gameCollider.friction_damping = 0.0f;
            addGameColliderToList(colliders, &gameCollider);
            startNewCollider = true;
        }
    }
    if (!playerInitialPositionFound) {
        printf("WARNING: No player in the level\n");
    }

    UnloadImageColors(pixels);
    UnloadImage(image);

}
