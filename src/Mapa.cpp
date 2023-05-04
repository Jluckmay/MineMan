#include "Mapa.h"
#include "Ore.h"
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
                mapa[i][j].wall();
            }
            else if(j == 0 || j == (comp-1))
            {
                mapa[i][j].setImage("Sprites/Smooth_Stone.bmp");
                mapa[i][j].wall();
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
    mapa[y][x].wall();

    if(x==(comprimento-2))
    {
        mapa[y][x-1].setImage("Sprites/Cobble_Stone.png");
        mapa[y][x-1].setTexture();
        mapa[y][x-1].draw();
        mapa[y][x-1].wall();
    }
    else if(x==1)
    {
        mapa[y][x+1].setImage("Sprites/Cobble_Stone.png");
        mapa[y][x+1].setTexture();
        mapa[y][x+1].draw();
        mapa[y][x+1].wall();
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

    cage[1] = (altura-4);
    cage[0] = (altura - (margem+2));
    for (i = cage[1]; (i > cage[0]); i--)
    {
        
        if(aux==0)
        {
            aux = comprimento/2;
            for ( j = aux-1; (aux-j) < (num+6); j--, aux++)
            {
                this->createWall(j,i);
                this->createWall(aux,i);
            }

            cage[2] = j+3;
            cage[3] = aux -3;

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

void Mapa::insertOres()
{
    Ore* aux;
    int margem = 0.2*altura;
    for (int i = 0; i < altura; i++)
    {
        for (int j = 0; j < comprimento; j++)
        {
            if(!mapa[i][j].isWall() && !((i<cage[1] && i>cage[0]) && (j<cage[3] && j>cage[2])))
            {
                aux = new Ore();

                aux->setCoord(j,i);
                aux->setImage("Sprites/Diamond.png");
                aux->setPoints();
                aux->setTexture();
                aux->draw();

                mapa[i][j].setOre(aux);

            }
        }
        
    }
    
}