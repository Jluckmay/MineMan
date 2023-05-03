#include "Mapa.h"
#include <stdlib.h>

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
            mapa[i][j].setCoord(j,i);
            mapa[i][j].draw();
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

void Mapa::createWall(int x, int y)
{
    mapa[y][x].setImage("Sprites/Cobble_Stone.png");
    mapa[y][x].setTexture();
    mapa[y][x].draw();

    if(x==(comprimento-2))
    {
        mapa[y][x-1].setImage("Sprites/Cobble_Stone.png");
        mapa[y][x-1].setTexture();
        mapa[y][x-1].draw();
    }
    else if(x==1)
    {
        mapa[y][x+1].setImage("Sprites/Cobble_Stone.png");
        mapa[y][x+1].setTexture();
        mapa[y][x+1].draw();
    }
}

void Mapa::createWalls()
{
    int j,k;
    int aux;
    int margemY, margemX;

    margemY = 0.2*altura;
    margemX = 0.2*comprimento;


    for (int i = 1; i < (altura - (margemY+2)); i++)
    {
       
        if(i==3)
        {
            aux = comprimento-3;
            for(j=2;aux>j;j+=(k+2), aux-=(k+2))
            {
                for(k=0;k<4;k++)
                {
                    this->createWall(j+k,i);
                    this->createWall(aux-k,i);
                }
            }
        }
        else if (i%3==0)
        {
            if(i%2==0 && i!=(comprimento-2))
            {
                aux = comprimento-2;
                for (j = 1, k =3; aux>=margemX; j+=k, aux-=k)
                {

                    this->createWall(j,i);
                    this->createWall(aux,i);

                }
            }
            else
            {
                aux = comprimento-3;
                for(j=2;aux>j;j+=(k+1), aux-=(k+1))
                {
                    for(k=0;k<2;k++)
                    {
                        this->createWall(j+k,i);
                        this->createWall(aux-k,i);
                    }
                }
            }
            
        }
        
        
        
    }
    
}

void Mapa::createCage(int num)
{
    int i,j;
    int aux=0;
    int margem;

    margem = 0.2*altura;

    for (i = (altura-4); i > (altura - (margem+2)); i--)
    {
        
        if(aux==0)
        {
            aux = comprimento/2;
            for ( j = aux-1; (aux-j) < (num+6); j--, aux++)
            {
                this->createWall(j,i);
                this->createWall(aux,i);
            }

            for(j-=1,aux+=1;j>1;j-=4,aux+=4)
            {
                for(int k=0;k<3;k++)
                {
                    this->createWall(j-k,i);
                    this->createWall(aux+k,i);
                }
            }
        }
        else
        {
            aux = comprimento/2;
            this->createWall(aux+num/2,i);
            this->createWall(aux-(num/2+1),i);
        }
        
        
    }
    
    aux = comprimento/2+1;
    for ( j = aux-3; (aux-j) < (num+3); j--, aux++)
    {
        this->createWall(j,i+1);
        this->createWall(aux,i+1);
    }

    for(j-=1,aux+=1;j>1;j-=4,aux+=4)
    {
        for(int k=0;k<3;k++)
        {
            this->createWall(j-k,i);
            this->createWall(aux+k,i);
        }
    }
}