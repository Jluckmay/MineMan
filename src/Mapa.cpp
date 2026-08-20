#include "Mapa.h"

#include <allegro5/allegro_primitives.h>

#include <algorithm>
#include <cstdio>
#include <random>

namespace
{
    constexpr int TILE = 16;
    constexpr int ITEM_SPRITE_SIZE = 16;
    constexpr int ITEM_SPRITE_STRIDE = 18;
    constexpr int ITEM_SPRITE_MARGIN = 1;
}

// Inicializa o mapa, carrega as texturas e gera a primeira rodada.
Mapa::Mapa(int w, int h) : width_(w), height_(h), tiles_(w * h), pickups_(w * h),
                           floor_(al_load_bitmap("Sprites/Stone_Block.bmp")), border_(al_load_bitmap("Sprites/Smooth_Stone.bmp")),
                           wall_(al_load_bitmap("Sprites/Cobble_Stone.png")), blocks_(al_load_bitmap("Sprites/Blocks.png")),
                           items_(al_load_bitmap("Sprites/Itens.png")), rng_(std::random_device{}())
{
    // As folhas possuem fundos sólidos usados como chroma key. Convertê-los
    // uma vez na carga evita retângulos coloridos em volta dos sprites.
    if (blocks_)
        al_convert_mask_to_alpha(blocks_, al_map_rgb(163, 73, 164));
    if (items_)
    {
        al_convert_mask_to_alpha(items_, al_map_rgb(112, 56, 97));
        al_convert_mask_to_alpha(items_, al_map_rgb(182, 69, 166));
    }
    if (!floor_ || !border_ || !wall_ || !blocks_ || !items_)
        std::fprintf(stderr, "Erro ao carregar texturas. Execute na raiz do projeto.\n");
    reset();
}

// Libera todas as texturas carregadas pelo mapa.
Mapa::~Mapa()
{
    if (floor_)
        al_destroy_bitmap(floor_);
    if (border_)
        al_destroy_bitmap(border_);
    if (wall_)
        al_destroy_bitmap(wall_);
    if (blocks_)
        al_destroy_bitmap(blocks_);
    if (items_)
        al_destroy_bitmap(items_);
}

// Altera o terreno de uma coordenada quando ela pertence ao mapa.
void Mapa::setTile(int x, int y, Tile t)
{
    if (x >= 0 && y >= 0 && x < width_ && y < height_)
        tiles_[index(x, y)] = t;
}

// Monta o terreno, as paredes e os minérios de um novo labirinto.
void Mapa::buildMaze()
{
    // Cada rodada começa vazia, recebe uma borda fixa e depois um dos dez
    // padrões de paredes. O lambda wall protege a área inicial e as bordas.
    std::fill(tiles_.begin(), tiles_.end(), Tile::Floor);
    std::fill(pickups_.begin(), pickups_.end(), Pickup::None);
    for (int x = 0; x < width_; ++x)
    {
        setTile(x, 0, Tile::Border);
        setTile(x, height_ - 1, Tile::Border);
    }
    for (int y = 0; y < height_; ++y)
    {
        setTile(0, y, Tile::Border);
        setTile(width_ - 1, y, Tile::Border);
    }
    layout_ = std::uniform_int_distribution<int>(0, 9)(rng_);
    auto wall = [&](int x, int y) {
        if (x > 1 && y > 1 && x < width_ - 2 && y < height_ - 2)
            setTile(x, y, Tile::Wall);
    };
    if (layout_ == 0)
    { // Barras horizontais alternadas
        for (int y = 4; y < height_ - 3; y += 4)
            for (int x = 2; x < width_ - 2; ++x)
                if (((y / 4) % 2 == 0) ? x >= 6 : x <= width_ - 7)
                    wall(x, y);
    }
    else if (layout_ == 1)
    { // Barras verticais alternadas
        for (int x = 5; x < width_ - 3; x += 5)
            for (int y = 2; y < height_ - 2; ++y)
                if (((x / 5) % 2 == 0) ? y >= 7 : y <= height_ - 8)
                    wall(x, y);
    }
    else if (layout_ == 2)
    { // Cruz com quatro portais
        for (int x = 3; x < width_ - 3; ++x)
            if (x != 8 && x != 23)
                wall(x, height_ / 2);
        for (int y = 3; y < height_ - 3; ++y)
            if (y != 8 && y != 23)
                wall(width_ / 2, y);
    }
    else if (layout_ == 3)
    { // Grade de salas abertas
        for (int x = 8; x < width_ - 3; x += 8)
            for (int y = 2; y < height_ - 2; ++y)
                if (y % 7 != 3)
                    wall(x, y);
        for (int y = 8; y < height_ - 3; y += 8)
            for (int x = 2; x < width_ - 2; ++x)
                if (x % 7 != 3)
                    wall(x, y);
    }
    else if (layout_ == 4)
    { // Zigue-zague
        for (int y = 5; y < height_ - 4; y += 5)
            for (int x = 3; x < width_ - 3; ++x)
                if ((y / 5) % 2 ? x < width_ - 8 : x > 7)
                    wall(x, y);
    }
    else if (layout_ == 5)
    { // Campo de pilares
        for (int y = 5; y < height_ - 4; y += 5)
            for (int x = 5; x < width_ - 4; x += 5)
            {
                wall(x, y);
                wall(x + 1, y);
                wall(x, y + 1);
                wall(x + 1, y + 1);
            }
    }
    else if (layout_ == 6)
    { // Dois anéis com entradas opostas
        for (int x = 5; x < width_ - 5; ++x)
        {
            if (x != 8)
                wall(x, 5);
            if (x != width_ - 9)
                wall(x, height_ - 6);
        }
        for (int y = 5; y < height_ - 5; ++y)
        {
            if (y != height_ - 9)
                wall(5, y);
            if (y != 8)
                wall(width_ - 6, y);
        }
        for (int x = 10; x < width_ - 10; ++x)
        {
            if (x != width_ / 2)
                wall(x, 10);
            if (x != width_ / 2)
                wall(x, height_ - 11);
        }
    }
    else if (layout_ == 7)
    { // Diagonais interrompidas
        for (int i = 4; i < width_ - 4; ++i)
        {
            if (i % 6 != 0)
                wall(i, i);
            if (i % 7 != 0)
                wall(width_ - 1 - i, i);
        }
    }
    else if (layout_ == 8)
    { // Pequenas barras alternadas
        for (int y = 4; y < height_ - 3; y += 4)
            for (int x = 3 + (y % 8); x < width_ - 3; x += 8)
            {
                wall(x, y);
                wall(x + 1, y);
                wall(x + 2, y);
            }
    }
    else
    { // Corredores em pente
        for (int x = 5; x < width_ - 4; x += 5)
            for (int y = 4; y < height_ - 4; ++y)
                if ((x / 5) % 2 ? y > 8 : y < height_ - 9)
                    wall(x, y);
    }
    // Minérios são uma decoração transitável. Primeiro sorteamos as casas,
    // depois quantos e quais tipos estarão disponíveis nesta rodada.
    std::vector<int> floorCells;
    for (int y = 1; y < height_ - 1; ++y)
        for (int x = 1; x < width_ - 1; ++x)
            if (tiles_[index(x, y)] == Tile::Floor)
                floorCells.push_back(index(x, y));
    std::shuffle(floorCells.begin(), floorCells.end(), rng_);
    std::uniform_int_distribution<int> amountRoll(60, 110);
    const int oreCount = std::min(amountRoll(rng_), (int)floorCells.size());
    std::vector<Tile> orePool = {Tile::CoalOre, Tile::IronOre, Tile::GoldOre, Tile::DiamondOre, Tile::EmeraldOre};
    std::shuffle(orePool.begin(), orePool.end(), rng_);
    std::uniform_int_distribution<int> kindCountRoll(2, (int)orePool.size());
    const int activeKinds = kindCountRoll(rng_);
    std::uniform_int_distribution<int> oreType(0, activeKinds - 1);
    for (int i = 0; i < oreCount; ++i)
        tiles_[floorCells[i]] = orePool[oreType(rng_)];
}

// Posiciona aleatoriamente gemas, equipamentos e efeitos no mapa.
void Mapa::placePickups()
{
    // A região próxima ao spawn fica livre. Embaralhar tanto as casas quanto o
    // conteúdo torna a distribuição independente do layout escolhido.
    std::vector<int> free;
    for (int y = 2; y < height_ - 2; ++y)
        for (int x = 2; x < width_ - 2; ++x)
            if (walkable(x, y) && !(x < 5 && y < 5))
                free.push_back(index(x, y));
    std::shuffle(free.begin(), free.end(), rng_);
    std::vector<Pickup> content = {Pickup::Emerald, Pickup::Diamond, Pickup::Emerald, Pickup::Diamond,
                                   Pickup::Emerald, Pickup::Diamond, Pickup::Emerald, Pickup::Diamond, Pickup::Emerald, Pickup::Diamond,
                                   Pickup::SuspiciousStew, Pickup::SuspiciousStew,
                                   Pickup::ArmorGold, Pickup::ArmorIron, Pickup::ArmorDiamond, Pickup::SwordGold, Pickup::SwordIron, Pickup::SwordDiamond};
    std::shuffle(content.begin(), content.end(), rng_);
    for (int i = 0; i < (int)content.size() && i < (int)free.size(); ++i)
        pickups_[free[i]] = content[i];
}

// Reinicia o mapa com um novo labirinto e novos itens.
void Mapa::reset()
{
    buildMaze();
    placePickups();
}

// Verifica se uma casa existe e não contém borda ou parede.
bool Mapa::walkable(int x, int y) const
{
    if (x < 0 || y < 0 || x >= width_ || y >= height_)
        return false;
    Tile t = tiles_[index(x, y)];
    return t != Tile::Border && t != Tile::Wall;
}

// Retorna o item de uma casa e o remove para evitar nova coleta.
Mapa::Pickup Mapa::collect(int x, int y)
{
    if (x < 0 || y < 0 || x >= width_ || y >= height_)
        return Pickup::None;
    // Retirar o item da camada garante que a mesma coleta ocorra uma única vez.
    Pickup p = pickups_[index(x, y)];
    pickups_[index(x, y)] = Pickup::None;
    return p;
}

// Conta as esmeraldas e os diamantes que ainda não foram coletados.
int Mapa::gemsRemaining() const
{
    return (int)std::count_if(pickups_.begin(), pickups_.end(), [](Pickup p)
                              { return p == Pickup::Emerald || p == Pickup::Diamond; });
}

// Recorta e desenha no mapa o sprite correspondente a um item.
void Mapa::drawPickupIcon(Pickup p, int x, int y, int size, int flags) const
{
    // O atlas organiza sprites de 16 px separados por 2 px, com margem de 1 px.
    int col = 0;
    int row = 0;
    switch (p)
    {
    case Pickup::Emerald:
        col = 12;
        row = 7;
        break;
    case Pickup::Diamond:
        col = 20;
        row = 6;
        break;
    case Pickup::SuspiciousStew:
        col = 6;
        row = 17;
        break;
    case Pickup::ArmorGold:
        col = 3;
        row = 9;
        break;
    case Pickup::ArmorIron:
        col = 2;
        row = 10;
        break;
    case Pickup::ArmorDiamond:
        col = 0;
        row = 7;
        break;
    case Pickup::SwordGold:
        col = 10;
        row = 9;
        break;
    case Pickup::SwordIron:
        col = 12;
        row = 10;
        break;
    case Pickup::SwordDiamond:
        col = 7;
        row = 7;
        break;
    default:
        return;
    }

    const int sourceX = col * ITEM_SPRITE_STRIDE + ITEM_SPRITE_MARGIN;
    const int sourceY = row * ITEM_SPRITE_STRIDE + ITEM_SPRITE_MARGIN;
    al_draw_scaled_bitmap(items_, sourceX, sourceY, ITEM_SPRITE_SIZE, ITEM_SPRITE_SIZE,
                          x, y, size, size, flags);
}

// Renderiza cada casa do mapa e o item que estiver sobre ela.
void Mapa::draw() const
{
    // Ordem de desenho por célula: piso/parede, veio de minério e item. Assim o
    // item sempre fica visível acima do cenário.
    for (int y = 0; y < height_; ++y)
        for (int x = 0; x < width_; ++x)
        {
            Tile t = tiles_[index(x, y)];
            ALLEGRO_BITMAP *b = t == Tile::Border ? border_ : (t == Tile::Wall ? wall_ : floor_);
            if (b)
                al_draw_scaled_bitmap(b, 0, 0, al_get_bitmap_width(b), al_get_bitmap_height(b), x * TILE, y * TILE, TILE, TILE, 0);
            if (t >= Tile::CoalOre)
            {
                if (t == Tile::CoalOre && blocks_)
                    al_draw_bitmap_region(blocks_, 59, 2, 16, 16, x * TILE, y * TILE, 0);
                else
                {
                    ALLEGRO_COLOR ore = t == Tile::IronOre ? al_map_rgb(215, 175, 140) : (t == Tile::GoldOre ? al_map_rgb(255, 210, 35) : (t == Tile::DiamondOre ? al_map_rgb(45, 225, 230) : al_map_rgb(40, 205, 80)));
                    const int px = x * TILE, py = y * TILE;
                    al_draw_filled_rectangle(px + 3, py + 3, px + 5, py + 5, ore);
                    al_draw_filled_rectangle(px + 10, py + 2, px + 12, py + 4, ore);
                    al_draw_filled_rectangle(px + 7, py + 8, px + 9, py + 10, ore);
                    al_draw_filled_rectangle(px + 2, py + 12, px + 4, py + 14, ore);
                    al_draw_filled_rectangle(px + 12, py + 11, px + 14, py + 13, ore);
                }
            }
            drawPickupIcon(pickups_[index(x, y)], x * TILE, y * TILE);
        }
}
