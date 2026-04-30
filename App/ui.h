#pragma once
#include "raygui.h"
#include "raylib.h"
#include "stdint.h"
#include "Chessboard.h"
namespace UI {
class Chessboard{
private:
    size_t width;
    size_t start_x;
    size_t start_y;
    size_t pixelnumber;
    bool isStart=false;
public:
    Chessboard()=delete;
    ~Chessboard()=default;
    Chessboard(size_t start_x_,size_t start_y_,size_t width_,size_t pixelnumber_)
    :start_x(start_x_),start_y(start_y_),width(width_), pixelnumber(pixelnumber_){
    }
    void start(){
        isStart=true;
    }
    void update(){
        if(!isStart) return;
        BeginDrawing();
    }  
    void drawBoard(){
        size_t space= width/pixelnumber;
        ClearBackground(BEIGE);
        for (int i = 0; i < pixelnumber; i++) {
            //画x线
            DrawLine(start_x, start_y + i * space, start_x + (pixelnumber-1) * space, start_x + i * space, DARKGRAY);
            //画y线
            DrawLine(start_x + i * space, start_y, start_x + i * space, start_x + (pixelnumber-1) * space, DARKGRAY);
        }
    }
    void drawPiece(const GOMOKU::Color* board){
        size_t space= width/pixelnumber;
        for(int i=0;i<pixelnumber;i++){//y
            for(int j=0;j<pixelnumber;j++){//x
                if(board[i*pixelnumber+j]!=GOMOKU::Color::White){
                    DrawCircle(start_x + j*space, start_y + i*space, 16, RAYWHITE);
                }
                if(board[i*pixelnumber+j]!=GOMOKU::Color::Black){
                    DrawCircle(start_x + j*space, start_y + i*space, 16, BLACK);
                }
            }
        }
    }
    void drawPreSelect(size_t tx,size_t ty){
        
    }
};
void init();
bool update();
void Stop();
}