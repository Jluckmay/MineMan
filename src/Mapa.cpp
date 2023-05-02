#include "Mapa.h"
#include <stdio.h>

Mapa::Mapa()
{
    altura = 0;
    comprimento = 0;
    mapa = NULL;
}

Mapa::Mapa(int comp, int alt)
{
    comprimento = comp;
    altura = alt;

    mapa = new Block*[alt];

    for (int i = 0; i < alt; i++)
    {
       mapa[i] = new Block[comp];
    }
    
    for (int i = 0; i < alt; i++)
    {
        for (int j = 0; j < comp; j++)
        {
            if(i == 0 || i == (alt-1))
            {
                mapa[i][j].setImage("Sprites/Smooth_Stone.bmp");
            }
            else if(j == 0 || j == (comp-1))
            {
                mapa[i][j].setImage("Sprites/Smooth_Stone.bmp");
            }
            else
            {
                mapa[i][j].setImage("Sprites/Stone_Block.bmp");
            }
            
            mapa[i][j].setTexture();

            printf("carregando textura no mapa");
            al_draw_bitmap(mapa[i][j].getTexture(),(j*16),(i*16),0);
            printf("textura carregada");
        }
        
    }
    
}

Mapa::~Mapa()
{

    for (int i = 0; i < altura; i++)
    {
        delete(mapa[i]);
    }

    delete mapa;
    
}