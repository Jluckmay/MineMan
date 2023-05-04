#include "Block.h"
#include <string.h>
#include <stdio.h>

Block::Block(const char * image, int x = 0, int y = 0, int parede = 0)
{
    strcpy(this->image, image);
    this->textura = al_load_bitmap(this->image);
    this->setCoord(x,y);
    this->parede = parede;
}

Block::Block(const Block &obj)
{

    strcpy(this->image, obj.image);

    this->textura = al_load_bitmap(this->image);


    this->parede = obj.parede;
    this->x = obj.x;
    this->y = obj.y;
    
}

Block::Block()
{
    image[0] = '\0';
    textura = NULL;
    ore = NULL;
    parede = 0;
    x = 0;
    y =0;
}

Block::~Block()
{
    al_destroy_bitmap(textura);
    delete(ore);
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

void Block::wall()
{
    parede = 1;
}

int Block::isWall()
{
    return parede;
}

void Block::setOre(Ore* aux)
{
    this->ore = aux;
}

Ore* Block::getOre()
{
    return ore;
}