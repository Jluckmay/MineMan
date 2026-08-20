# MineMan

[Português](#português) | [English](#english)

## Português

MineMan é um jogo de exploração de labirintos inspirado no universo de
Minecraft. Desenvolvido em C++17 com Allegro 5, o jogo combina mapas aleatórios,
coleta de gemas, equipamentos temporários e inimigos que perseguem o jogador.

### Objetivo

Explore o mapa e colete todas as esmeraldas e diamantes antes de perder suas
três vidas. Cada rodada escolhe um entre dez layouts e distribui aleatoriamente
minérios, gemas, equipamentos, sopas suspeitas e inimigos.

### Recursos

- Dez layouts de labirinto selecionados aleatoriamente.
- Janela redimensionável com escala proporcional.
- Gemas, minérios, armas, armaduras e efeitos temporários.
- Zumbis que renascem como husks cinco segundos após serem derrotados.
- HUD com mapa, gemas restantes, pontuação, vidas e equipamentos.
- Telas de vitória e derrota com opção de reinício.

### Pontuação

| Evento | Pontos |
|---|---:|
| Coletar esmeralda | 5 |
| Coletar diamante | 10 |
| Coletar sopa suspeita | 50 |
| Derrotar inimigo | 300 |

### Equipamentos e efeitos

Armaduras absorvem ataques, enquanto espadas derrotam inimigos por contato.
Ambas expiram após 60 segundos; coletar outro item da mesma categoria substitui
o equipamento atual.

| Material | Cargas |
|---|---:|
| Ouro | 1 |
| Ferro | 2 |
| Diamante | 3 |

A sopa suspeita concede um efeito aleatório durante 15 segundos:

- **Velocidade:** move o jogador duas casas por comando.
- **Invisibilidade:** faz os inimigos vagarem aleatoriamente; colisões ainda
  causam dano.
- **Imunidade:** impede dano, mas não interrompe a perseguição.

### Controles

| Tecla | Ação |
|---|---|
| Setas ou WASD | Mover o jogador |
| R | Reiniciar após vitória ou derrota |
| Esc | Sair |

### Dependências

- Compilador com suporte a C++17
- GNU Make
- Allegro 5 com os addons Image, Font e Primitives

No Ubuntu, Debian ou WSL:

```bash
sudo apt update
sudo apt install build-essential liballegro5-dev liballegro-image5-dev
```

### Compilação e execução

Na raiz do projeto:

```bash
make
./MineMan
```

Também é possível compilar e executar com um único comando:

```bash
make run
```

Para remover os arquivos gerados pela compilação:

```bash
make clean
```

Execute o jogo a partir da raiz do repositório, pois os caminhos dos sprites
são relativos a essa pasta.

### Estrutura do projeto

```text
MineMan/
|-- Sprites/        # Texturas, personagens e atlas de itens
|-- src/
|   |-- Game.*      # Ciclo principal, regras, inimigos e interface
|   |-- Mapa.*      # Geração, itens e renderização do mapa
|   `-- main.cpp    # Ponto de entrada
|-- Makefile        # Automação da compilação
|-- LICENSE         # Licença GNU GPLv3
`-- README.md
```

### Licença

O código-fonte é distribuído sob a GNU General Public License v3.0 ou
posterior. Consulte [LICENSE](LICENSE) para conhecer os termos completos.

Os nomes e recursos visuais associados a Minecraft pertencem aos respectivos
detentores de direitos. Este é um projeto independente, sem afiliação ou
endosso da Mojang Studios ou da Microsoft.

---

## English

MineMan is a maze exploration game inspired by the Minecraft universe. Built
with C++17 and Allegro 5, it combines randomized maps, gem collecting,
temporary equipment, and enemies that chase the player.

### Goal

Explore the map and collect every emerald and diamond before losing all three
lives. Each round selects one of ten layouts and randomly distributes ores,
gems, equipment, suspicious stews, and enemies.

### Features

- Ten randomly selected maze layouts.
- Resizable window with proportional scaling.
- Gems, ores, weapons, armor, and temporary effects.
- Zombies that respawn as husks five seconds after being defeated.
- HUD showing the map, remaining gems, score, lives, and equipment.
- Victory and game-over screens with a restart option.

### Scoring

| Event | Points |
|---|---:|
| Collect an emerald | 5 |
| Collect a diamond | 10 |
| Collect suspicious stew | 50 |
| Defeat an enemy | 300 |

### Equipment and effects

Armor absorbs attacks, while swords defeat enemies on contact. Both expire
after 60 seconds; collecting another item in the same category replaces the
current equipment.

| Material | Charges |
|---|---:|
| Gold | 1 |
| Iron | 2 |
| Diamond | 3 |

Suspicious stew grants one random effect for 15 seconds:

- **Speed:** moves the player two tiles per command.
- **Invisibility:** makes enemies wander randomly; collisions still deal
  damage.
- **Immunity:** prevents damage but does not stop enemies from chasing.

### Controls

| Key | Action |
|---|---|
| Arrow keys or WASD | Move the player |
| R | Restart after winning or losing |
| Esc | Quit |

### Requirements

- A compiler with C++17 support
- GNU Make
- Allegro 5 with the Image, Font, and Primitives addons

On Ubuntu, Debian, or WSL:

```bash
sudo apt update
sudo apt install build-essential liballegro5-dev liballegro-image5-dev
```

### Build and run

From the project root:

```bash
make
./MineMan
```

To build and run with a single command:

```bash
make run
```

To remove generated build files:

```bash
make clean
```

Run the game from the repository root because sprite paths are relative to
that directory.

### Project structure

```text
MineMan/
|-- Sprites/        # Textures, characters, and item atlas
|-- src/
|   |-- Game.*      # Main loop, rules, enemies, and interface
|   |-- Mapa.*      # Map generation, items, and rendering
|   `-- main.cpp    # Entry point
|-- Makefile        # Build automation
|-- LICENSE         # GNU GPLv3 license
`-- README.md
```

### License

The source code is distributed under the GNU General Public License v3.0 or
later. See [LICENSE](LICENSE) for the complete terms.

Minecraft-related names and visual assets belong to their respective rights
holders. This is an independent project and is not affiliated with or endorsed
by Mojang Studios or Microsoft.
