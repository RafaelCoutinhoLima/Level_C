# 😈 Level C

**"Um remake 2D do 'Level Devil' (mas em C)"**


Level C é um jogo de plataforma 2D do gênero *rage* (feito para passar raiva), inspirado no notório **Level Devil**.  
O objetivo é simples: chegar ao final da fase. A dificuldade é... bom, você vai descobrir.

O jogo foi desenvolvido em **C puro** utilizando a biblioteca gráfica **Raylib** para a disciplina de *Programação Imperativa Formativa (PIF)* na CESAR School.

---

## 👥 Equipe de Desenvolvimento

Projeto desenvolvido por:

- Guilherme Tolentino Leitão De Melo  
- João Eduardo Azevedo  
- Rafael Coutinho Lima

---

## 🎬 Vídeo Demonstrativo

Confira o screencast do jogo e veja as mecânicas (e o sofrimento) em ação:

🎥 **Screencast no YouTube:**  
👉 https://www.youtube.com/watch?v=lugar de botar deposi de gravar

Ou clique no botão abaixo:

[![Assistir no YouTube](https://www.youtube.com/watch?v=lugar de botar deposi de gravar)

---

## 🛠️ Compilando e Rodando

### ✅ 1. Instale as Dependências (Raylib)

### 🔸 Linux (Ubuntu/Debian)

Antes de seguir o guia oficial, tente o método mais simples via terminal:

    sudo apt update

    # Instala as dependências gráficas e de áudio necessárias
    sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev \
    libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev \
    libxkbcommon-dev

    # Instala a biblioteca Raylib
    sudo apt install libraylib-dev

💡 *Se estiver usando outra distribuição ou SO (Windows/Mac), consulte o Guia Oficial da Raylib.*

---

### 📥 2. Clone o repositório

    git clone https://github.com/SEU_USUARIO/level-c.git
    cd level-c

---

### ▶️ 3. Compile e execute o jogo
Dentro da pasta do projeto:

#### Usando Makefile:

    make clean && make run

#### Ou compilação manual (GCC):

    gcc main.c -o game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 && ./game

---

## 🎮 Modos de Jogo

### 🧍‍♂️ Modo Campanha
O jogador deve atravessar uma série de níveis sequenciais.  
Cada nível introduz uma nova mecânica de trollagem ou desafio de física.

⚠️ **Não existem checkpoints**: morreu, volta pro início.

---

## 🎛️ Menu de Seleção

No menu principal, você encontrará:
  
- **Level Select (menu de níveis)** — escolha fases já desbloqueadas.  
- **Voltar** — retorna ao menu anterior.

---

## 🕹️ Como Jogar
O objetivo é guiar o personagem até a porta de saída sem morrer para as armadilhas invisíveis.

### Controles Básicos
- ⬅️ **Esquerda / A** — mover para trás  
- ➡️ **Direita / D** — mover para frente  
- ⬆️ **Cima / W / Espaço** — pular  

### Mecânica de Pulo
A altura do pulo é dinâmica: segure para pular mais alto, toque rápido para pulo curto.

---

## 🧠 Detalhes Extras da Jogabilidade

### 🧱 Plataformas Fantasmas
- Algumas desaparecem quando tocadas.  
- Outras são invisíveis e só aparecem ao colidir.

### ⚙️ Espinhos
Espinhos posicionados estrategicamente que exigem precisão e cuidado ao se movimentar.

---

## 🧐 Curiosidade

O nome **Level C** surgiu como uma brincadeira dupla:

1. Referência direta ao jogo original **Level Devil**.  
2. O jogo foi desenvolvido **inteiramente em C**, durante a disciplina de **Programação Iperativa Funcional (PIF)**.  

Seguindo a tradição da equipe em criar jogos com nomes baseados na tecnologia utilizada.

---
