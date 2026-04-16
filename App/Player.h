#pragma once
#include "Chessboard.h"
class Player{
private:
    Chessboard* bindChessboard=nullptr;
    bool isbind=false;
    Color selfcolor;
    placeAction selfaction;
    bool haveAction=false;
public://构造相关
    Player(Chessboard* board,Color color){
        bindBoard(board);
        setColor(color);
    }
public:
    inline void setColor(Color color){selfcolor =color;}
    inline void bindBoard(Chessboard* board){
        bindChessboard=board;
        isbind=true;
    }
    bool commitMove(placeAction action){
        if(haveAction) return false; 
        else{
            return true;
        }
    }
    bool comfirmSelection(bool iscomfirm){
        bool isSuccess=true;
        if(iscomfirm){

        }else{

        }
        return isSuccess;
    }
};