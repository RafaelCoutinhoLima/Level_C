# 😈 Level C
### *"Um remake 2D do 'Level Devil' (mas em C)"*

![Gameplay do Level C](fazer tutorial)

**Level C** é um jogo de plataforma 2D do gênero *rage*, inspirado no notório **Level Devil**.  
O objetivo é simples: chegar ao final da fase.  
A dificuldade é... bom, você vai descobrir.

O jogo foi desenvolvido em **C puro** utilizando a biblioteca gráfica [Raylib](https://www.raylib.com/)  
para a disciplina de **Programação Imperativa Formativa (PIF)**.

---

## 👥 Equipe de Desenvolvimento

| Integrante | Função Principal |
|-------------|------------------|
| **João Eduardo Azevedo** | Design de níveis, lógica de jogo e testes |
| **Rafael Coutinho Lima** | Programação principal, física e integração Raylib |
| **Guilherme Tolentino Leitão De Melo** | Sistema de menus, assets e suporte técnico |

---

## 🛠️ Como Compilar e Rodar

### ✅ 1. Instale as Dependências (Raylib)

#### 🔸 Linux (Ubuntu/Debian)
Antes de seguir o guia oficial, tente o método mais simples:

```bash
sudo apt update

# Instala as dependências que a Raylib precisa
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev 
sudo apt install libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev 
sudo apt install libxkbcommon-dev

# Instala a biblioteca Raylib
sudo apt install libraylib-dev
