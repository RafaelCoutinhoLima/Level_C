#include "button.h"
#include "raylib.h"

Button CreateButton(float x,float y,float width,float height,const char *text){
    Button button= {0};
    //zera todos os valores do butão 
    float posX = (x<0)?(GetScreenWidth()-width)/2.0f :x;
    float posY = (y<0)?(GetScreenHeight()-height)/2.0f:y;
    //faz a verificação e centraliza os limites de x e y
    button.bounds = (Rectangle){posX,posY,width,height};
    //pegando as dimensões do botão 
    button.text= text;

    button.baseColor=(Color){60,60,60,255};//cinza esscuro
    button.hoverColor=(Color){100,100,100,255};//cinza medio
    button.clickColor=(Color){30,30,30,255};//cinza escuro

    button.hovered =false;
    button.clicked=false;
    return button;
}
bool UpdateButton(Button *button){
    Vector2 mousePoint=GetMousePosition();
    bool hovered = CheckCollisionPointRec(mousePoint, button->bounds);
    bool fire = false;

    if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        button->clicked = true;
    }
    
    if (button->clicked && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
        button->clicked = false;
        if (hovered){
            fire = true;
        }
    }

    if (!hovered && !IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        button->clicked = false;
    }

    button->hovered = hovered;
    return fire;
}
void DrawButton(Button button){
    Color color=button.baseColor;
    if(button.hovered){
        color=button.hoverColor;//a cor de quando ta em cima o mouse
    }
    if(button.clicked){
        color=button.clickColor;//cor de quando clica
    }
    DrawRectangleRec(button.bounds,color);
    //desenhar o botão
    DrawRectangleLinesEx(button.bounds,2,button.hoverColor);// as bordas dos botões
    //botar o texto no centro
    int fontSize=20;
    int textWidth =MeasureText(button.text,fontSize);
    float textX=button.bounds.x +(button.bounds.width-textWidth)/2.0f;
    float textY =button.bounds.y+(button.bounds.height-fontSize)/2.0f;
    
    DrawText(button.text,(int)textX,(int)textY,fontSize,WHITE);
}