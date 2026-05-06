#pragma once
#include "Chessboard.h"
namespace GOMOKU{
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
        if(!bindChessboard->check_piece(x, y)) return false;
        selfaction={.x=x,.y=y,.color=selfcolor}; 
        haveAction=true;
        return true;
    }
    bool comfirmSelection(placeAction a){
        if(!haveAction) return false;
        bool isSuccess=true;
        if(haveAction&&a.x==selfaction.x&&a.y==selfaction.y){
            bindChessboard->place_piece(selfaction);
            haveAction=false;
        }else{
            isSuccess=false;
            haveAction=false;
        }
        return isSuccess;
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
    const Color& getColor() const {return selfcolor;}
    const placeAction getAction() const {
        if(haveAction)
            return selfaction;
        return {.x=0,.y=0,.color=Color::None};
    }
};
}