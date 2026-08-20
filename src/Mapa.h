// SPDX-License-Identifier: MIT

#ifndef MAPA_H
#define MAPA_H

#include <allegro5/allegro.h>

#include <random>
#include <vector>

// Representa o cenário, seus terrenos e os itens coletáveis.
class Mapa
{
public:
    // Tile descreve o terreno. Minérios são transitáveis; apenas bordas e
    // paredes bloqueiam o movimento.
    enum class Tile
    {
        Floor,
        Border,
        Wall,
        CoalOre,
        IronOre,
        GoldOre,
        DiamondOre,
        EmeraldOre
    };
    enum class Pickup
    {
        None,
        Emerald,
        Diamond,
        SuspiciousStew,
        ArmorGold,
        ArmorIron,
        ArmorDiamond,
        SwordGold,
        SwordIron,
        SwordDiamond
    };

    // Cria um mapa com as dimensões informadas e carrega seus recursos visuais.
    Mapa(int width, int height);
    // Libera as imagens utilizadas para desenhar o mapa.
    ~Mapa();
    // Impede a cópia de mapas que possuem recursos gráficos exclusivos.
    Mapa(const Mapa &) = delete;
    // Impede a atribuição por cópia entre mapas.
    Mapa &operator=(const Mapa &) = delete;

    // Gera um novo labirinto e distribui seus itens.
    void reset();
    // Desenha o terreno e os itens do mapa.
    void draw() const;
    // Informa se uma coordenada pode ser atravessada.
    bool walkable(int x, int y) const;
    // Recolhe e remove o item presente em uma coordenada.
    Pickup collect(int x, int y);
    // Retorna a quantidade de gemas ainda disponíveis.
    int gemsRemaining() const;
    // Retorna o número do layout atual para exibição.
    int layoutNumber() const { return layout_ + 1; }
    // Desenha o ícone de um item na posição e no tamanho indicados.
    void drawPickupIcon(Pickup item, int pixelX, int pixelY, int size = 16, int flags = 0) const;

private:
    // O mapa usa vetores lineares; index() converte uma coordenada (x, y) para
    // a posição correspondente. pickups_ é uma camada independente do piso.
    int width_;
    int height_;
    std::vector<Tile> tiles_;
    std::vector<Pickup> pickups_;
    ALLEGRO_BITMAP *floor_;
    ALLEGRO_BITMAP *border_;
    ALLEGRO_BITMAP *wall_;
    ALLEGRO_BITMAP *blocks_;
    ALLEGRO_BITMAP *items_;
    // Um gerador persistente evita repetir a mesma sequência entre resets.
    std::mt19937 rng_;
    int layout_ = 0;

    // Converte uma coordenada bidimensional em um índice linear.
    int index(int x, int y) const { return y * width_ + x; }
    // Define o tipo de terreno de uma coordenada válida.
    void setTile(int x, int y, Tile tile);
    // Constrói aleatoriamente um dos layouts de labirinto disponíveis.
    void buildMaze();
    // Distribui os itens coletáveis pelas casas livres.
    void placePickups();
};

#endif
