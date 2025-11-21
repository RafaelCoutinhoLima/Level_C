#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

typedef struct{
    Rectangle bounds;//guarda o tamanho altura ,essas coisas
    char text[64];
    Color baseColor;
    Color hoverColor;
    Color clickColor;
    bool hovered;
    bool clicked; //feedback visual do botão
}Button;

Button CreateButton(float x,float y ,float width,float height,const char *text);
//criando o botão 
bool UpdateButton(Button *button);
//para atualizar o estado do botão e saber se parou de precionar e tudo  mais 
void DrawButton(Button button);
void SetButtonText(Button *btn, const char *text);
#endif