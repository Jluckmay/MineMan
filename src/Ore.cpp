#include "Ore.h"
#include <string.h>

Ore::Ore()
{
    image[0]='\0';
    x=0;
    y=0;
    points=20;
}

Ore::Ore(Ore &obj)
{
    strcpy(this->image, obj.image);
    this->setTexture();
    this->x = obj.x;
    this->y = obj.y;
    this->points = obj.points;
}

void Ore::setTexture()
{
    textura = al_load_bitmap(image);
}

ALLEGRO_BITMAP* Ore::getTexture()
{
    return textura;
}

void Ore::draw()
{
    al_draw_bitmap(this->getTexture(),(x*16),(y*16),0);
}

void Ore::setCoord(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Ore::setImage(char* image)
{
    strcpy(this->image, image);
}

void Ore::setPoints()
{
    this->points = 20;
}

void Ore::setPoints(int points)
{
    this->points = points;
}