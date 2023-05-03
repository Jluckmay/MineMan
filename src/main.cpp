#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include "Block.h"
#include "Mapa.h"

using namespace std;

int main()
{
    Mapa *mapa;
    ALLEGRO_DISPLAY *display;
    bool sair = false;

    // Inicializa as bibliotecas Allegro
    if (!al_init())
    {
        printf("Erro ao inicializar as bibliotecas Allegro!");
        return -1;
    }

    // Inicializa as bibliotecas de imagens Allegro
    if(!al_init_image_addon())
    {
        printf("Erro ao inicializar as bibliotecas de imagens do Allegro!");
        return -1;
    }

    // Cria o display
    display = al_create_display(512, 512);
    if (!display)
    {
        printf("Erro ao criar o display!");
        return -1;
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));

    mapa = new Mapa(32,32);
    mapa->createWalls();
    mapa->createCage(4);

    al_flip_display();

    getchar();

    al_destroy_display(display);

    // delete(mapa);

    return 0;
}
