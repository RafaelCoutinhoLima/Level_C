#include "button.h"
#include <string.h>
#include <raylib.h>

Button CreateButton(float x, float y, float width, float height, const char *text) {
    Button button = (Button){0};

    float posX = (x < 0) ? (GetScreenWidth()  - width)  / 2.0f : x;
    float posY = (y < 0) ? (GetScreenHeight() - height) / 2.0f : y;

    button.bounds = (Rectangle){ posX, posY, width, height };
    TextCopy(button.text, text ? text : "");

    button.baseColor  = (Color){ 60,  60,  60, 255 };
    button.hoverColor = (Color){100, 100, 100, 255 };
    button.clickColor = (Color){ 30,  30,  30, 255 };

    button.hovered = false;
    button.clicked = false;
    return button;
}

bool UpdateButton(Button *button) {
    if (!button) return false;

    Vector2 mousePoint = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mousePoint, button->bounds);
    bool fire = false;

    if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        button->clicked = true;
    }

    if (button->clicked && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        button->clicked = false;
        if (hovered) fire = true;
    }

    if (!hovered && !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        button->clicked = false;
    }

    button->hovered = hovered;
    return fire;
}

void DrawButton(Button button) {
    Color color = button.baseColor;
    if (button.hovered) color = button.hoverColor;
    if (button.clicked) color = button.clickColor;

    DrawRectangleRec(button.bounds, color);
    DrawRectangleLinesEx(button.bounds, 2, button.hoverColor);

    int fontSize = 20;
    int textWidth = MeasureText(button.text, fontSize);
    float textX = button.bounds.x + (button.bounds.width  - textWidth) / 2.0f;
    float textY = button.bounds.y + (button.bounds.height - fontSize)  / 2.0f;

    DrawText(button.text, (int)textX, (int)textY, fontSize, WHITE);
}

void SetButtonText(Button *btn, const char *text) {
    if (btn && text) TextCopy(btn->text, text);
}
