#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <memory>
#include <random>
#include <vector>
#include "Mapa.h"

class Game
{
public:
    Game();
    ~Game();
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    int run();

private:
    static constexpr int TileSize = 16;
    static constexpr int MapSize = 32;
    static constexpr int ScreenSize = TileSize * MapSize;
    static constexpr int Fps = 60;
    static constexpr int EquipmentDuration = 60 * Fps;
    static constexpr int EffectDuration = 15 * Fps;
    static constexpr int RespawnDuration = 5 * Fps;

    struct Player
    {
        int x = 2;
        int y = 2;
        bool facingLeft = false;
    };

    struct Enemy
    {
        int x = 0;
        int y = 0;
        bool facingLeft = false;
        bool alive = true;
        bool husk = false;
        int respawnTicks = 0;
    };

    struct Equipment
    {
        int charges = 0;
        int remainingTicks = 0;
        int tier = 0;
    };

    enum class State { Playing, Won, Lost };
    enum class Effect { None, Speed, Invisible, Immune };

    ALLEGRO_DISPLAY *display_ = nullptr;
    ALLEGRO_TIMER *timer_ = nullptr;
    ALLEGRO_EVENT_QUEUE *queue_ = nullptr;
    ALLEGRO_FONT *font_ = nullptr;
    ALLEGRO_BITMAP *steve_ = nullptr;
    ALLEGRO_BITMAP *zombie_ = nullptr;
    std::unique_ptr<Mapa> map_;

    Player player_;
    std::vector<Enemy> enemies_;
    Equipment armor_;
    Equipment sword_;
    State state_ = State::Playing;
    Effect effect_ = Effect::None;
    std::mt19937 rng_;
    int lives_ = 3;
    int score_ = 0;
    int enemyTicks_ = 0;
    int animationTicks_ = 0;
    int effectTicks_ = 0;
    bool running_ = true;
    bool redraw_ = true;
    bool allegroReady_ = false;
    bool imageReady_ = false;
    bool primitivesReady_ = false;

    bool initialize();
    void shutdown();
    void resetRound();
    void spawnEnemy(Enemy &enemy);
    void applyPickup(Mapa::Pickup pickup);
    void resolveCollision(Enemy &enemy);
    void handleKey(int keycode);
    void update();
    void updateEnemies();
    void render();

    static const char *materialName(int tier);
};

#endif
