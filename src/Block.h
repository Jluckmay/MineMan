#ifndef BLOCK_H
#define BLOCK_H

#include <allegro5/allegro.h>

class Block
{
    private:

        ALLEGRO_BITMAP *textura;
        
        char image[100];
        int x;
        int y;

    public:

        Block();
        Block(const Block &obj);
        Block(const char *image, int x, int y);

        ~Block();

        void setImage(const char* image);

        char* getImage();

        void setTexture();

        void setCoord(int x, int y);

        void draw();

        ALLEGRO_BITMAP* getTexture();
};

#endif