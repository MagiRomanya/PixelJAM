#include "map_loader.h"

void loadMap(char *filename , TileMap *tiles, GameColliderList *colliders, Cable *cable, Player *player) {
    Image image = LoadImage(filename);
    Color *pixels = LoadImageColors(image);
    int map_width = image.width;
    int map_height = image.height;
    TileMap t_map = createTileMap();

    GameColliderList c_list = createGameColliderList();
    bool startNewCollider = true;
    int x1, x2, y;
    const int r = 8;

    // Read color value from each pixel
    for (int j = 0; j < map_height; j++) {
        for (int i = 0; i < map_width; i++) {
            Color pixel = pixels[j * map_width + i];
            if (pixel.a != 0) {
                // int sprite_id = pixel.b;
                // if (pixel.g) sprite_id += 256 + pixel.g;
                addTileToMap(&t_map, pixel.b, i*16, j*16);

                if (pixel.r && startNewCollider) {
                    x1 = i*16;
                    y = j;
                    startNewCollider = false;
                } else if (!pixel.r && !startNewCollider) {
                    x2 = (i-1)*16;
                    CapsuleCollider collider = createCapsule(x1, x2, y, r);
                    startNewCollider = true;
                }
            }
        }
    }
    
    UnloadImage(image);

}
