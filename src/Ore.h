#ifndef ORE_H
#define ORE_H

#include <allegro5/allegro.h>

class Ore
{
    private:
        
        ALLEGRO_BITMAP *textura;
        
        char image[100];
        int points;
        int x;
        int y;

    public:
        
        Ore();
        Ore(Ore &obj);

        void setTexture();
        ALLEGRO_BITMAP* getTexture();
        void setImage(char*);
        void setCoord(int, int);
        void setPoints();
        void setPoints(int points);
        void draw();
};

#endif