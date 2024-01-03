#include "map_loader.h"

void loadMap(char *filename , TileMap *tiles, GameColliderList *colliders, Cable *cable, Player *player) {
    Image image = LoadImage(filename);
    Color *pixels = LoadImageColors(image);
    int mapWidth = image.width;
    int mapHeight = image.height;
    t_map = createTileMap();

    // Read color value from each pixel
    for (int j = 0; j < mapHeight; j++) {
        for (int i = 0; i < mapWidth; i++) {
            Color pixel = pixels[j * mapWidth + i];
            if (pixel.a != 0) {
                // int sprite_id = pixel.b;
                // if (pixel.g) sprite_id += 256 + pixel.g;
                addTileToMap(t_map, pixel.b, i*16, j*16)
            }
        }
    }
    
    UnloadImage(image);

}