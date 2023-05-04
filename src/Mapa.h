#ifndef MAPA_H
#define MAPA_H

#include "Block.h"

class Mapa
{
    private:

        int comprimento;
        int altura;
        int cage[4];

        Block **mapa;

    public:
        Mapa();
        Mapa(int comp, int larg);
        ~Mapa();

        void createWall(int x, int y);
        void createWalls();
        void createCage(int num);
        void insertOres();
};

#endif
