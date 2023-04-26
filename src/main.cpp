#include <allegro5/allegro.h>
#include <stdio.h>

int main()
{
    bool sair = false;

    // Inicializa as bibliotecas Allegro
    if (!al_init())
    {
        printf("Erro ao inicializar as bibliotecas Allegro!");
        return -1;
    }

    ALLEGRO_DISPLAY *display = al_create_display(224, 288);
    if (!display)
    {
        printf("Erro ao criar o display!");
        return -1;
    }

    ALLEGRO_BITMAP *mapa = al_load_bitmap("mapa.jpg");
    if (!mapa)
    {
        printf("Erro ao carregar o mapa!");
        al_destroy_display(display);
        return -1;
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(mapa, 0, 0, 0);
    al_flip_display();

    // Aguarda 5 segundos
    al_rest(5);

    al_destroy_bitmap(mapa);
    al_destroy_display(display);

    return 0;
}
