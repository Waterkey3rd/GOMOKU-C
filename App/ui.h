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
                if(board[i*pixelnumber+j]==GOMOKU::Color::White){
                    DrawCircle(start_x + j*space, start_y + i*space, (size_t)(space/2), RAYWHITE);
                }
                if(board[i*pixelnumber+j]==GOMOKU::Color::Black){
                    DrawCircle(start_x + j*space, start_y + i*space, (size_t)(space/2), BLACK);
                }
            }
        }
    }
    void drawPreSelect(size_t tx,size_t ty){
        size_t space= width/pixelnumber;
        DrawCircle(tx*space+start_x,ty*space+start_y, (size_t)(space/2),RED);
    }
    GOMOKU::placeAction getPiece(){
        GOMOKU::placeAction action={.x=-1,.y=-1,.color=GOMOKU::Color::None};
        size_t space= width/pixelnumber;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 m = GetMousePosition();
            action.x = (m.x - start_x + (int)(space/2)) / space;
            action.y = (m.y - start_y + (int)(space/2)) / space;
        }
        return action;
    }
};
class GameHistory{
private:
int startx,starty;
int height,width;
int displayCount;
public:
GameHistory(int x,int y,int h,int w,int dispalycount):startx(x),starty(y),width(w),height(h),displayCount(dispalycount){}
void updatehistory(const vector<GOMOKU::Event>& hist){
    int dis=0;
    for (int i = (int)hist.size() - 1; i >= 0 && dis < displayCount; i--, dis++) {
        auto act = hist[i].placeaction; 
        DrawText(TextFormat("#%d: (%d, %d)", hist[i].id, act.x, act.y), startx, starty+120 + dis * 25, 16, DARKGRAY);
    }
}
void updateTurn(int step,GOMOKU::Color c){
    DrawRectangle(startx, starty, width, height, Fade(BLACK, 0.1f));
    DrawText(TextFormat("Step: %d", step), startx, 50+starty, 20, DARKGRAY);
    DrawText(TextFormat("Turn: %s", (c == GOMOKU::Color::Black ? "BLACK" : "WHITE")), startx, 80+starty, 20, BLACK);
}
bool updateWin(GOMOKU::Color c){
    if(c==GOMOKU::Color::None)
        return false;
    else {
        Vector2 pos = GetWindowPosition();
        DrawRectangle(pos.x, pos.y, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
        DrawText(TextFormat("%s WIN!",c==GOMOKU::Color::Black?"BLACK":"WHITE"), 300, 300, 40, RED);
        if (GuiButton({ 350, 380, 200, 40 }, "Restart Game"))
            return true;
    }
    return false;
}
void setSize(int w,int h){width=w;height=h;}
void setStart(int x,int y){startx=x,starty=y;}
void setDisplayCount(int n){displayCount=n;}
};
void init();
bool update();
void Stop();
}