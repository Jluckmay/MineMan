# MineMan

[Português](#português) | [English](#english)

**Jogue online / Play online:** [jluckmay.github.io/mineman](https://jluckmay.github.io/mineman/)

## Português

MineMan é um jogo de exploração de labirintos inspirado no universo de
Minecraft. Desenvolvido em C++17 com Allegro 5, o jogo combina mapas aleatórios,
coleta de gemas, equipamentos temporários e inimigos que perseguem o jogador.

O projeto foi desenvolvido originalmente como requisito da disciplina de
Programação Orientada a Objetos da Universidade Federal de Viçosa (UFV),
Campus Rio Paranaíba, e aprimorado posteriormente.

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

### Versão web

O arquivo `index.html` oferece a mesma experiência diretamente no navegador.
Você pode jogar a versão publicada em
[jluckmay.github.io/mineman](https://jluckmay.github.io/mineman/).

Para evitar restrições do navegador ao carregar os sprites locais, inicie um
servidor HTTP na raiz do projeto:

```bash
python3 -m http.server 8000
```

Depois, acesse `http://localhost:8000`. Os controles e as regras são os mesmos
da versão em C++.

### Ícones da aplicação

O personagem principal é usado como favicon da versão web e como ícone dos
pacotes nativos. No Windows, o Makefile incorpora `Icons/MineMan.ico`
automaticamente ao `MineMan.exe`. Para pacotes Linux, use
`Icons/MineMan.png` junto de `packaging/linux/MineMan.desktop`.

### Executáveis gerados

Os artefatos de distribuição são gravados em `dist/` e não são versionados:

- `MineMan-Linux-x86_64.AppImage`: aplicativo Linux portátil com bibliotecas e
  sprites incorporados.
- `MineMan-Windows-x64.zip`: pacote Windows portátil com o executável, DLLs e
  sprites necessários.

### Estrutura do projeto

```text
MineMan/
|-- Icons/          # Ícones PNG e ICO do personagem principal
|-- packaging/      # Metadados para pacotes nativos
|-- resources/      # Recurso de ícone do executável Windows
|-- Sprites/        # Texturas, personagens e atlas de itens
|-- src/
|   |-- Game.*      # Ciclo principal, regras, inimigos e interface
|   |-- Mapa.*      # Geração, itens e renderização do mapa
|   `-- main.cpp    # Ponto de entrada
|-- index.html      # Versão web executada com Canvas 2D
|-- ASSETS.md       # Origem e condições dos recursos visuais
|-- Makefile        # Automação da compilação
|-- LICENSE         # Licença MIT do código-fonte
`-- README.md
```

### Licença

O código-fonte e os arquivos de compilação são distribuídos sob a licença
MIT. Consulte [LICENSE](LICENSE) para conhecer os termos completos. Essa
licença não abrange os recursos de terceiros presentes em `Sprites/`.

### Créditos dos sprites

Os sprites utilizados neste projeto foram obtidos no
[The Spriters Resource](https://www.spriters-resource.com/) e disponibilizados
na plataforma pelos usuários **Striker212** e **DogToon64**. O crédito pelo
upload e pela extração dos recursos pertence a esses colaboradores; os direitos
sobre os elementos originais permanecem com seus respectivos titulares.
Consulte [ASSETS.md](ASSETS.md) para conhecer o escopo e as condições de uso.

Os nomes e recursos visuais associados a Minecraft pertencem aos respectivos
detentores de direitos. Este é um projeto independente, sem afiliação ou
endosso da Mojang Studios ou da Microsoft.

---

## English

MineMan is a maze exploration game inspired by the Minecraft universe. Built
with C++17 and Allegro 5, it combines randomized maps, gem collecting,
temporary equipment, and enemies that chase the player.

The project was originally developed as a requirement for the Object-Oriented
Programming course at the Federal University of Viçosa (UFV), Rio Paranaíba
Campus, and was subsequently improved.

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

### Web version

The `index.html` file provides the same experience directly in a web browser.
You can play the published version at
[jluckmay.github.io/mineman](https://jluckmay.github.io/mineman/).

To avoid browser restrictions when loading local sprites, start an HTTP server
from the project root:

```bash
python3 -m http.server 8000
```

Then open `http://localhost:8000`. Controls and rules are the same as in the
C++ version.

### Application icons

The main character is used as the web favicon and native package icon. On
Windows, the Makefile automatically embeds `Icons/MineMan.ico` into
`MineMan.exe`. For Linux packages, use `Icons/MineMan.png` together with
`packaging/linux/MineMan.desktop`.

### Generated executables

Distribution artifacts are written to `dist/` and are not committed:

- `MineMan-Linux-x86_64.AppImage`: portable Linux application with bundled
  libraries and sprites.
- `MineMan-Windows-x64.zip`: portable Windows package with the executable,
  required DLLs, and sprites.

### Project structure

```text
MineMan/
|-- Icons/          # PNG and ICO icons featuring the main character
|-- packaging/      # Native package metadata
|-- resources/      # Windows executable icon resource
|-- Sprites/        # Textures, characters, and item atlas
|-- src/
|   |-- Game.*      # Main loop, rules, enemies, and interface
|   |-- Mapa.*      # Map generation, items, and rendering
|   `-- main.cpp    # Entry point
|-- index.html      # Canvas 2D web version
|-- ASSETS.md       # Visual asset sources and usage conditions
|-- Makefile        # Build automation
|-- LICENSE         # MIT License for the source code
`-- README.md
```

### License

The source code and build files are distributed under the MIT License. See
[LICENSE](LICENSE) for the complete terms. This license does not cover the
third-party resources under `Sprites/`.

### Sprite credits

The sprites used in this project were obtained from
[The Spriters Resource](https://www.spriters-resource.com/) and uploaded to the
platform by **Striker212** and **DogToon64**. Credit for uploading and extracting
the assets belongs to these contributors; rights to the original material
remain with their respective owners.
See [ASSETS.md](ASSETS.md) for the scope and usage conditions.

Minecraft-related names and visual assets belong to their respective rights
holders. This is an independent project and is not affiliated with or endorsed
by Mojang Studios or Microsoft.
