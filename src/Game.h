#ifndef GAME_H
#define GAME_H

#include "Mapa.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include <memory>
#include <random>
#include <vector>

// Coordena a inicialização, as regras e o ciclo principal do jogo.
class Game
{
public:
    // Cria o jogo e prepara seu gerador de números aleatórios.
    Game();
    // Encerra o jogo e libera os recursos utilizados.
    ~Game();
    // Impede a cópia de uma instância que controla recursos do Allegro.
    Game(const Game &) = delete;
    // Impede a atribuição por cópia do jogo.
    Game &operator=(const Game &) = delete;

    // Executa o ciclo principal do jogo até seu encerramento.
    int run();

private:
    static constexpr int TileSize = 16;
    static constexpr int MapSize = 32;
    static constexpr int ScreenSize = TileSize * MapSize;
    static constexpr int Fps = 60;
    static constexpr int EquipmentDuration = 60 * Fps;
    static constexpr int EffectDuration = 15 * Fps;
    static constexpr int RespawnDuration = 5 * Fps;

    // Armazena a posição e a orientação visual do jogador.
    struct Player
    {
        int x = 2;
        int y = 2;
        bool facingLeft = false;
    };

    // Armazena o estado de movimentação, vida e renascimento de um inimigo.
    struct Enemy
    {
        int x = 0;
        int y = 0;
        bool facingLeft = false;
        bool alive = true;
        bool husk = false;
        int respawnTicks = 0;
    };

    // Armazena a durabilidade, a duração e o material de um equipamento.
    struct Equipment
    {
        int charges = 0;
        int remainingTicks = 0;
        int tier = 0;
    };

    enum class State
    {
        Playing,
        Won,
        Lost
    };

    enum class Effect
    {
        None,
        Speed,
        Invisible,
        Immune
    };

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

    // Inicializa o Allegro, a janela e os recursos gráficos.
    bool initialize();
    // Libera de forma segura todos os recursos do jogo.
    void shutdown();
    // Restaura o estado inicial e prepara uma nova rodada.
    void resetRound();
    // Posiciona um inimigo em uma casa válida longe do jogador.
    void spawnEnemy(Enemy &enemy);
    // Aplica ao jogador o efeito do item coletado.
    void applyPickup(Mapa::Pickup pickup);
    // Resolve o combate quando um inimigo encontra o jogador.
    void resolveCollision(Enemy &enemy);
    // Interpreta uma tecla e atualiza o movimento ou o estado do jogo.
    void handleKey(int keycode);
    // Atualiza temporizadores, efeitos e renascimentos a cada quadro.
    void update();
    // Move os inimigos e verifica suas colisões com o jogador.
    void updateEnemies();
    // Desenha o mapa, os personagens e a interface do jogo.
    void render();

    // Retorna o nome do material correspondente ao nível informado.
    static const char *materialName(int tier);
};

#endif
