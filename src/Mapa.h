#ifndef MAPA_H
#define MAPA_H
#include <allegro5/allegro.h>
#include <vector>
#include <random>

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

    Mapa(int width, int height);
    ~Mapa();
    Mapa(const Mapa &) = delete;
    Mapa &operator=(const Mapa &) = delete;

    void reset();
    void draw() const;
    bool walkable(int x, int y) const;
    Pickup collect(int x, int y);
    int gemsRemaining() const;
    int layoutNumber() const { return layout_ + 1; }
    void drawPickupIcon(Pickup item, int pixelX, int pixelY, int size = 16, int flags = 0) const;

private:
    // O mapa usa vetores lineares; index() converte uma coordenada (x, y) para
    // a posição correspondente. pickups_ é uma camada independente do piso.
    int width_, height_;
    std::vector<Tile> tiles_;
    std::vector<Pickup> pickups_;
    ALLEGRO_BITMAP *floor_, *border_, *wall_, *blocks_, *items_;
    // Um gerador persistente evita repetir a mesma sequência entre resets.
    std::mt19937 rng_;
    int layout_ = 0;

    int index(int x, int y) const { return y * width_ + x; }
    void setTile(int x, int y, Tile tile);
    void buildMaze();
    void placePickups();
};
#endif
