// src/ui/button.h
#ifndef BUTTON_H
#define BUTTON_H

#include <raylib.h>
#include <stdbool.h>

typedef struct {
    Rectangle bounds;
    char  text[64];
    Color baseColor;
    Color hoverColor;
    Color clickColor;
    bool  hovered;
    bool  clicked;
} Button;

Button CreateButton(float x, float y, float width, float height, const char *text);
bool   UpdateButton(Button *button);
void   DrawButton(Button button);
void   SetButtonText(Button *btn, const char *text);

#endif // BUTTON_H
