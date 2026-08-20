// SPDX-License-Identifier: MIT

#include "Game.h"

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <algorithm>
#include <cstdio>

// Inicializa o gerador aleatório usado pelas regras do jogo.
Game::Game() : rng_(std::random_device{}()) {}

// Garante a liberação dos recursos mantidos pelo jogo.
Game::~Game()
{
    shutdown();
}

// Prepara o Allegro, cria a janela e carrega os recursos necessários.
bool Game::initialize()
{
    allegroReady_ = al_init();
    imageReady_ = allegroReady_ && al_init_image_addon();
    primitivesReady_ = imageReady_ && al_init_primitives_addon();
    if (!primitivesReady_)
        return false;

    al_init_font_addon();
    if (!al_install_keyboard())
        return false;

    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
    display_ = al_create_display(ScreenSize, ScreenSize);
    timer_ = al_create_timer(1.0 / Fps);
    queue_ = al_create_event_queue();
    font_ = al_create_builtin_font();
    steve_ = al_load_bitmap("Sprites/Steve.png");
    zombie_ = al_load_bitmap("Sprites/Zombie.png");

    if (!display_ || !timer_ || !queue_ || !font_ || !steve_ || !zombie_)
        return false;

    // O fundo azul da folha de mobs funciona como chroma key.
    al_convert_mask_to_alpha(zombie_, al_map_rgb(147, 187, 236));
    al_set_window_title(display_, "MineMan - Gem Hunt");
    al_register_event_source(queue_, al_get_display_event_source(display_));
    al_register_event_source(queue_, al_get_keyboard_event_source());
    al_register_event_source(queue_, al_get_timer_event_source(timer_));

    map_ = std::make_unique<Mapa>(MapSize, MapSize);
    return true;
}

// Destrói os recursos do Allegro na ordem apropriada.
void Game::shutdown()
{
    // O mapa precisa liberar seus bitmaps antes de o display ser destruído.
    map_.reset();
    if (steve_)
        al_destroy_bitmap(steve_);
    if (zombie_)
        al_destroy_bitmap(zombie_);
    if (font_)
        al_destroy_font(font_);
    if (queue_)
        al_destroy_event_queue(queue_);
    if (timer_)
        al_destroy_timer(timer_);
    if (display_)
        al_destroy_display(display_);
    steve_ = zombie_ = nullptr;
    font_ = nullptr;
    queue_ = nullptr;
    timer_ = nullptr;
    display_ = nullptr;

    if (primitivesReady_)
        al_shutdown_primitives_addon();
    if (imageReady_)
        al_shutdown_image_addon();
    primitivesReady_ = imageReady_ = allegroReady_ = false;
}

// Converte o nível numérico de um equipamento em seu nome.
const char *Game::materialName(int tier)
{
    if (tier == 3)
        return "Diamante";
    if (tier == 2)
        return "Ferro";
    if (tier == 1)
        return "Ouro";
    return "Nenhum";
}

// Sorteia para um inimigo uma posição transitável fora da área inicial.
void Game::spawnEnemy(Enemy &enemy)
{
    do
    {
        enemy.x = 2 + static_cast<int>(rng_() % 28);
        enemy.y = 2 + static_cast<int>(rng_() % 28);
    } while (!map_->walkable(enemy.x, enemy.y) || (enemy.x < 8 && enemy.y < 8));
}

// Reinicia mapa, personagens, equipamentos, pontuação e temporizadores.
void Game::resetRound()
{
    map_->reset();
    player_ = {};
    enemies_.assign(5, Enemy{});
    for (Enemy &enemy : enemies_)
        spawnEnemy(enemy);

    armor_ = {};
    sword_ = {};
    state_ = State::Playing;
    effect_ = Effect::None;
    lives_ = 3;
    score_ = 0;
    enemyTicks_ = 0;
    effectTicks_ = 0;
}

// Atualiza pontuação, equipamentos ou efeitos conforme o item coletado.
void Game::applyPickup(Mapa::Pickup pickup)
{
    switch (pickup)
    {
    case Mapa::Pickup::ArmorGold:
        armor_ = {1, EquipmentDuration, 1};
        break;
    case Mapa::Pickup::ArmorIron:
        armor_ = {2, EquipmentDuration, 2};
        break;
    case Mapa::Pickup::ArmorDiamond:
        armor_ = {3, EquipmentDuration, 3};
        break;
    case Mapa::Pickup::SwordGold:
        sword_ = {1, EquipmentDuration, 1};
        break;
    case Mapa::Pickup::SwordIron:
        sword_ = {2, EquipmentDuration, 2};
        break;
    case Mapa::Pickup::SwordDiamond:
        sword_ = {3, EquipmentDuration, 3};
        break;
    case Mapa::Pickup::Emerald:
        score_ += 5;
        break;
    case Mapa::Pickup::Diamond:
        score_ += 10;
        break;
    case Mapa::Pickup::SuspiciousStew:
    {
        const int roll = static_cast<int>(rng_() % 3);
        effect_ = roll == 0 ? Effect::Speed : (roll == 1 ? Effect::Invisible : Effect::Immune);
        effectTicks_ = EffectDuration;
        score_ += 50;
        break;
    }
    default:
        break;
    }
}

// Resolve ataque, defesa ou perda de vida durante uma colisão.
void Game::resolveCollision(Enemy &enemy)
{
    if (!enemy.alive || enemy.x != player_.x || enemy.y != player_.y)
        return;

    // Ataque tem prioridade sobre defesa quando jogador e mob dividem a casa.
    if (sword_.charges > 0 && sword_.remainingTicks > 0)
    {
        --sword_.charges;
        score_ += 300;
        enemy.alive = false;
        enemy.respawnTicks = RespawnDuration;
        if (sword_.charges == 0)
            sword_ = {};
        return;
    }
    if (effect_ == Effect::Immune && effectTicks_ > 0)
        return;
    if (armor_.charges > 0 && armor_.remainingTicks > 0)
    {
        --armor_.charges;
        if (armor_.charges == 0)
            armor_ = {};
    }
    else
    {
        --lives_;
        if (lives_ <= 0)
            state_ = State::Lost;
    }
    player_ = {};
}

// Trata comandos de saída, reinício e movimentação do jogador.
void Game::handleKey(int keycode)
{
    if (keycode == ALLEGRO_KEY_ESCAPE)
    {
        running_ = false;
        return;
    }
    if (state_ != State::Playing)
    {
        if (keycode == ALLEGRO_KEY_R)
            resetRound();
        return;
    }

    int dx = 0, dy = 0;
    if (keycode == ALLEGRO_KEY_LEFT || keycode == ALLEGRO_KEY_A)
        dx = -1;
    else if (keycode == ALLEGRO_KEY_RIGHT || keycode == ALLEGRO_KEY_D)
        dx = 1;
    else if (keycode == ALLEGRO_KEY_UP || keycode == ALLEGRO_KEY_W)
        dy = -1;
    else if (keycode == ALLEGRO_KEY_DOWN || keycode == ALLEGRO_KEY_S)
        dy = 1;
    else
        return;

    const int steps = effect_ == Effect::Speed && effectTicks_ > 0 ? 2 : 1;
    for (int step = 0; step < steps; ++step)
    {
        const int nextX = player_.x + dx;
        const int nextY = player_.y + dy;
        if (!map_->walkable(nextX, nextY))
            break;
        if (dx != 0)
            player_.facingLeft = dx < 0;
        player_.x = nextX;
        player_.y = nextY;
        applyPickup(map_->collect(nextX, nextY));
        for (Enemy &enemy : enemies_)
            resolveCollision(enemy);
    }
    if (map_->gemsRemaining() == 0)
        state_ = State::Won;
}

// Move cada inimigo em direção ao jogador ou aleatoriamente.
void Game::updateEnemies()
{
    for (Enemy &enemy : enemies_)
    {
        if (!enemy.alive)
            continue;

        int dx, dy;
        if (effect_ == Effect::Invisible && effectTicks_ > 0)
        {
            dx = static_cast<int>(rng_() % 3) - 1;
            dy = static_cast<int>(rng_() % 3) - 1;
        }
        else
        {
            dx = (player_.x > enemy.x) - (player_.x < enemy.x);
            dy = (player_.y > enemy.y) - (player_.y < enemy.y);
        }

        const bool horizontalFirst = rng_() % 2;
        int nextX = enemy.x + (horizontalFirst ? dx : 0);
        int nextY = enemy.y + (horizontalFirst ? 0 : dy);
        if (!map_->walkable(nextX, nextY))
        {
            nextX = enemy.x + (horizontalFirst ? 0 : dx);
            nextY = enemy.y + (horizontalFirst ? dy : 0);
        }
        if (map_->walkable(nextX, nextY))
        {
            if (nextX != enemy.x)
                enemy.facingLeft = nextX < enemy.x;
            enemy.x = nextX;
            enemy.y = nextY;
        }
        resolveCollision(enemy);
    }
}

// Avança a animação, os efeitos, equipamentos e inimigos.
void Game::update()
{
    ++animationTicks_;
    if (armor_.remainingTicks > 0 && --armor_.remainingTicks == 0)
        armor_ = {};
    if (sword_.remainingTicks > 0 && --sword_.remainingTicks == 0)
        sword_ = {};
    if (effectTicks_ > 0 && --effectTicks_ == 0)
        effect_ = Effect::None;

    for (Enemy &enemy : enemies_)
    {
        if (!enemy.alive && --enemy.respawnTicks == 0)
        {
            enemy.alive = true;
            enemy.husk = !enemy.husk;
            spawnEnemy(enemy);
        }
    }

    if (state_ == State::Playing && ++enemyTicks_ >= 30)
    {
        enemyTicks_ = 0;
        updateEnemies();
    }
}

// Renderiza o estado atual do jogo e sua interface na janela.
void Game::render()
{
    const int width = al_get_display_width(display_);
    const int height = al_get_display_height(display_);
    const float scale = std::min(width / static_cast<float>(ScreenSize), height / static_cast<float>(ScreenSize));
    const float offsetX = (width - ScreenSize * scale) / 2.0f;
    const float offsetY = (height - ScreenSize * scale) / 2.0f;

    ALLEGRO_TRANSFORM identity;
    al_identity_transform(&identity);
    al_use_transform(&identity);
    al_clear_to_color(al_map_rgb(8, 8, 12));
    ALLEGRO_TRANSFORM viewport;
    al_build_transform(&viewport, offsetX, offsetY, scale, scale, 0);
    al_use_transform(&viewport);

    map_->draw();
    const int frame = (animationTicks_ / 10) % 4;
    for (const Enemy &enemy : enemies_)
    {
        if (!enemy.alive)
            continue;
        const int sourceX = (enemy.husk ? 177 : 11) + frame * 24;
        al_draw_scaled_bitmap(zombie_, sourceX, 119, 24, 24, enemy.x * TileSize,
                              enemy.y * TileSize - 3, TileSize, TileSize + 4,
                              enemy.facingLeft ? ALLEGRO_FLIP_HORIZONTAL : 0);
    }
    al_draw_scaled_bitmap(steve_, 24 + frame * 24, 96, 24, 24, player_.x * TileSize,
                          player_.y * TileSize - 3, TileSize, TileSize + 4,
                          player_.facingLeft ? ALLEGRO_FLIP_HORIZONTAL : 0);

    if (sword_.charges > 0)
    {
        const Mapa::Pickup icon = sword_.tier == 3 ? Mapa::Pickup::SwordDiamond
            : (sword_.tier == 2 ? Mapa::Pickup::SwordIron : Mapa::Pickup::SwordGold);
        const int handX = player_.x * TileSize + (player_.facingLeft ? -5 : 10);
        map_->drawPickupIcon(icon, handX, player_.y * TileSize + 4, 11,
                             player_.facingLeft ? ALLEGRO_FLIP_HORIZONTAL : 0);
    }

    al_draw_filled_rectangle(3, 3, 509, 29, al_map_rgba(0, 0, 0, 220));
    al_draw_textf(font_, al_map_rgb(255, 255, 255), 7, 6, 0,
                  "Mapa:%d  Gemas:%d  Pontos:%d  Vidas:%d",
                  map_->layoutNumber(), map_->gemsRemaining(), score_, lives_);
    al_draw_textf(font_, al_map_rgb(100, 210, 255), 245, 6, 0,
                  "Armadura:%s %d (%ds)", materialName(armor_.tier), armor_.charges,
                  (armor_.remainingTicks + 59) / 60);
    al_draw_textf(font_, al_map_rgb(255, 230, 80), 245, 17, 0,
                  "Espada:%s %d (%ds)", materialName(sword_.tier), sword_.charges,
                  (sword_.remainingTicks + 59) / 60);

    if (effect_ != Effect::None)
    {
        const char *name = effect_ == Effect::Speed ? "Velocidade"
            : (effect_ == Effect::Invisible ? "Invisivel" : "Imunidade");
        const ALLEGRO_COLOR color = effect_ == Effect::Speed ? al_map_rgb(80, 255, 120)
            : (effect_ == Effect::Invisible ? al_map_rgb(180, 180, 255) : al_map_rgb(255, 130, 80));
        al_draw_textf(font_, color, 7, 18, 0, "Efeito:%s (%ds)", name, (effectTicks_ + 59) / 60);
    }

    al_draw_filled_rectangle(3, 492, 509, 509, al_map_rgba(0, 0, 0, 220));
    al_draw_text(font_, al_map_rgb(235, 235, 235), ScreenSize / 2, 497,
                 ALLEGRO_ALIGN_CENTER, "SETAS/WASD: mover   ESC: sair   R: reiniciar apos o fim");

    if (state_ != State::Playing)
    {
        al_draw_filled_rectangle(75, 205, 437, 307, al_map_rgba(0, 0, 0, 225));
        const bool won = state_ == State::Won;
        al_draw_text(font_, won ? al_map_rgb(80, 255, 100) : al_map_rgb(255, 80, 80),
                     ScreenSize / 2, 230, ALLEGRO_ALIGN_CENTER, won ? "VOCE VENCEU!" : "FIM DE JOGO");
        al_draw_textf(font_, al_map_rgb(255, 255, 255), ScreenSize / 2, 252,
                      ALLEGRO_ALIGN_CENTER, "Pontuacao: %d", score_);
        al_draw_text(font_, al_map_rgb(220, 220, 220), ScreenSize / 2, 278,
                     ALLEGRO_ALIGN_CENTER, "R reinicia  |  ESC sai");
    }
    al_flip_display();
}

// Processa eventos e quadros até que o jogador encerre a aplicação.
int Game::run()
{
    if (!initialize())
    {
        std::fprintf(stderr, "Falha ao inicializar recursos do jogo.\n");
        return 1;
    }

    resetRound();
    al_start_timer(timer_);
    while (running_)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue_, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running_ = false;
        else if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
        {
            al_acknowledge_resize(display_);
            redraw_ = true;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            handleKey(event.keyboard.keycode);
            redraw_ = true;
        }
        else if (event.type == ALLEGRO_EVENT_TIMER)
        {
            update();
            redraw_ = true;
        }

        if (redraw_ && al_is_event_queue_empty(queue_))
        {
            redraw_ = false;
            render();
        }
    }
    return 0;
}
