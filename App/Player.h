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
        setColor(color);
        bindBoard(board);
    }
public:
    inline void setColor(Color color){selfcolor =color;}
    inline void bindBoard(Chessboard* board){
        bindChessboard=board;
        isbind=true;
        board->registerPlayer(selfcolor);
    }
    bool commitMove(placeAction action){
        if(haveAction) return false;
        selfaction=action; 
        return true;
    }
    bool commitMove(int x,int y){
        if(haveAction) return false;
        selfaction={.x=x,.y=y,.color=selfcolor}; 
        haveAction=true;
        return true;
    }
    bool comfirmSelection(bool iscomfirm){
        if(!haveAction) return false;
        bool isSuccess=true;
        if(iscomfirm){
            bindChessboard->place_piece(selfaction);
            haveAction=false;
        }else{
            haveAction=false;
        }
        return isSuccess;
    }
};