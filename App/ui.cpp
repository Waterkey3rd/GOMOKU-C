#include "ui.h"
#define fpsNumber 60
void UI::init(){
    InitWindow(900, 650, "Gomoku Game");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(fpsNumber);
}
bool UI::update(){
    if(WindowShouldClose()) {UI::Stop();return false;}
    BeginDrawing();
    ClearBackground(BEIGE);
    EndDrawing();
    return true;
}
void UI::Stop(){
    CloseWindow();
}