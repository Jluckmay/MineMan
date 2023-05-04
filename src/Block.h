#ifndef BLOCK_H
#define BLOCK_H

#include <allegro5/allegro.h>
#include "Ore.h"

class Block
{
    private:

        ALLEGRO_BITMAP *textura;
        
        Ore* ore;
        char image[100];
        int parede;
        int x;
        int y;

    public:

        Block();
        Block(const Block &obj);
        Block(const char *image, int x, int y, int parede);

        ~Block();

        void setImage(const char* image);

        char* getImage();

        void setTexture();

        void setCoord(int x, int y);

        void draw();

        void wall();

        int isWall();

        void setOre(Ore *aux);
        
        Ore* getOre();

        ALLEGRO_BITMAP* getTexture();
};

#endif