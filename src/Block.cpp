#include "Block.h"
#include <string.h>
#include <stdio.h>

Block::Block(const char * image, int x, int y)
{
    strcpy(this->image, image);
    this->textura = al_load_bitmap(this->image);
}

Block::Block(const Block &obj)
{

    strcpy(this->image, obj.image);

    this->textura = al_load_bitmap(this->image);
    
}

Block::Block()
{
    image[0] = '\0';
    textura = NULL;
}

Block::~Block()
{
    al_destroy_bitmap(textura);
}

void Block::setImage(const char* image)
{
    strcpy(this->image, image);
}

void Block::setTexture()
{
    textura = al_load_bitmap(this->image);
    if(!textura)
    {
        printf("Erro ao carregar a textura");
    }
}

ALLEGRO_BITMAP* Block::getTexture()
{
    return textura;
}

void Block::setCoord(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Block::draw()
{
    al_draw_bitmap(this->getTexture(),(x*16),(y*16),0);
}

char* Block::getImage()
{
    return image;
}