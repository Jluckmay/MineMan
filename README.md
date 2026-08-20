# MineMan

MineMan é um jogo de labirinto inspirado em Minecraft, desenvolvido em C++ com
Allegro 5. Explore mapas aleatórios, colete gemas, equipe armas e armaduras e
evite os mobs hostis.

## Funcionalidades

- 10 estruturas de mapa sorteadas a cada rodada.
- Distribuição aleatória de gemas, equipamentos, sopas, mobs e minérios.
- Entre dois e cinco tipos de minério por rodada.
- Janela redimensionável com interface e mapa escalados proporcionalmente.
- Zumbis que retornam como husks cinco segundos depois de eliminados.
- Telas de vitória e derrota, pontuação, vidas e reinício.

## Objetivo e pontuação

Colete todas as gemas para vencer:

- Esmeralda: 5 pontos.
- Diamante: 10 pontos.
- Mob eliminado: 300 pontos.
- Sopa suspeita coletada: 50 pontos.

## Equipamentos

Armaduras absorvem ataques e espadas eliminam mobs por contato:

| Material | Cargas |
|---|---:|
| Ouro | 1 |
| Ferro | 2 |
| Diamante | 3 |

Cada equipamento expira após 60 segundos, mesmo que ainda tenha cargas. Pegar
outro item da mesma categoria substitui o anterior. A espada ativa aparece na
mão do personagem; a armadura é indicada somente no HUD.

## Sopa suspeita

A sopa concede aleatoriamente um efeito durante 15 segundos:

- Velocidade: move duas casas por comando.
- Invisibilidade: os mobs perdem o alvo e vagam aleatoriamente; colisões ainda
  causam dano.
- Imunidade: bloqueia dano, mas os mobs continuam perseguindo o jogador.

## Controles

| Tecla | Ação |
|---|---|
| Setas ou WASD | Mover |
| R | Reiniciar após vitória ou derrota |
| Esc | Sair |

Os controles também permanecem visíveis no rodapé durante o jogo.

## Dependências

No Ubuntu ou WSL:

```bash
sudo apt update
sudo apt install build-essential liballegro5-dev liballegro-image5-dev
```

## Compilar e executar

Na raiz do projeto:

```bash
make run
```

Também é possível executar as etapas separadamente:

```bash
make
./MineMan
```

Para remover os artefatos de compilação:

```bash
make clean
```

Os caminhos dos sprites são relativos à raiz; execute o jogo a partir desta
pasta para que os recursos sejam encontrados.

## Estrutura

```text
MineMan/
├── Makefile
├── README.md
├── LICENSE
├── Sprites/
│   ├── Blocks.png
│   ├── Cobble_Stone.png
│   ├── Itens.png
│   ├── Smooth_Stone.bmp
│   ├── Steve.png
│   ├── Stone_Block.bmp
│   └── Zombie.png
└── src/
    ├── Game.cpp
    ├── Game.h
    ├── main.cpp
    ├── Mapa.cpp
    └── Mapa.h
```
