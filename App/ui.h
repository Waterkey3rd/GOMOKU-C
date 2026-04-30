#pragma once
#include "raygui.h"
#include "raylib.h"
#include "stdint.h"
namespace UI {
class Chessboard{
private:
    size_t width;
    size_t height;
    size_t pixelnumber;
    bool isStart=false;
public:
    void start(){
        InitWindow(width, height, "Gomoku Game");
        SetTargetFPS(60);
        isStart=true;
    }
    void update(){
        if(!isStart) return;
        BeginDrawing();
    }  
    void drawBoard(){
        ClearBackground(BEIGE);
        for (int i = 0; i < pixelnumber; i++) {
            DrawLine(40, 40 + i * 40, 40 + 14 * 40, 40 + i * 40, DARKGRAY);
            DrawLine(40 + i * 40, 40, 40 + i * 40, 40 + 14 * 40, DARKGRAY);
        }
    }
};
}