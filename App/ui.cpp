#include "ui.h"
#include "Player.h"
#include <iostream>
#define fpsNumber 60
using namespace UI;
bool isGameover=false;
bool have_selected =false;
int Gamecount=0;
//logic
GOMOKU::Chessboard chessboard;
GOMOKU::Player blackPlayer=GOMOKU::Player(&chessboard,GOMOKU::Color::Black);
GOMOKU::Player whitePlayer=GOMOKU::Player(&chessboard,GOMOKU::Color::White);
GOMOKU::Player* turnPlayer=&whitePlayer;
//ui
Chessboard chessboard_ui=Chessboard(40,40,600,15);
GameHistory gamehistory_ui=GameHistory(620,0,650,280,5);
//程序初始化
void UI::init(){
    InitWindow(900, 650, "Gomoku Game");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(fpsNumber);
    isGameover=false;
    chessboard.reset();
    chessboard.registerPlayer(GOMOKU::Color::Black);
    chessboard.registerPlayer(GOMOKU::Color::White);
}
//逻辑和UI更新
bool UI::update(){
    if(WindowShouldClose()) {UI::Stop();return false;}
    BeginDrawing();
    ClearBackground(BEIGE);
    GOMOKU::placeAction inputaction;
    if(!isGameover){
        inputaction=chessboard_ui.getPiece();
        if(inputaction.x!=-1){
            if(have_selected){
                if(turnPlayer->comfirmSelection(inputaction)==true){
                    have_selected=false;
                    turnPlayer= turnPlayer==&whitePlayer ? &blackPlayer: &whitePlayer;
                }else{
                    if(turnPlayer->commitMove(inputaction.x,inputaction.y))
                        chessboard_ui.drawPreSelect(inputaction.x, inputaction.y);
                }
            }else{
                    if(turnPlayer->commitMove(inputaction.x,inputaction.y)){
                        chessboard_ui.drawPreSelect(inputaction.x,inputaction.y);
                        have_selected=true;
                    }
            }
        }else{
            if(have_selected){chessboard_ui.drawPreSelect(turnPlayer->getAction().x,turnPlayer->getAction().y);}
        }
    }
    chessboard_ui.drawBoard();
    chessboard_ui.drawPiece(chessboard.getBoardData());
    gamehistory_ui.updatehistory(chessboard.gethistory());
    gamehistory_ui.updateTurn(chessboard.getStep(),turnPlayer->getColor());
    isGameover=gamehistory_ui.updateWin(chessboard.checkWin());
    if(isGameover){
        printf("GameCout:%d\n",++Gamecount);
        isGameover=false;
        chessboard.reset();
        chessboard.registerPlayer(GOMOKU::Color::Black);
        chessboard.registerPlayer(GOMOKU::Color::White);
        turnPlayer=&whitePlayer;
    }
    EndDrawing();
    return true;
}
//关闭程序
void UI::Stop(){
    CloseWindow();
}