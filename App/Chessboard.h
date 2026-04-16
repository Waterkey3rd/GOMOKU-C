#pragma once
#include <vector>
#include "stdint.h"
#include <cstring>
enum class Color:uint8_t{
    None=0,
    Black,
    White
};

struct placeAction{
    int x,y;
    Color color;
};
struct Event{
    int id;
    union{
        placeAction palceaction;
    };
};
class Chessboard
{
private:
    static constexpr int H=15;
    static constexpr int W = 15;
    Color board[H][W];
    std::vector<Event> history;
    placeAction lastPlaceAction;
    Color Winner=Color::None;
    bool white_ready;
    bool black_ready;
    int step;
public://构造相关
    Chessboard(){
        reset();
    }
    Chessboard(const Chessboard&) =delete;
    ~Chessboard() =default;
public:
    void reset(){
        std::memset(board,0,sizeof(board));
        Winner=Color::None;
        step=0;
        lastPlaceAction= {.x=0,.y=0,.color=Color::None};
        history.clear();
    }
    inline bool isready(){return white_ready&&black_ready;}
    inline bool isfull(){return step==W*H;}
    inline const std::vector<Event>& gethistory(){return history;}
    inline const placeAction& get_last_move(){return lastPlaceAction;}
    inline Color  get_piece_at(int x, int y){return board[y][x];}
    inline const Color* getBoardData() const {return &board[0][0];}
    inline const int getBoardSize() const {return W*H;}
    inline const int& getBoardH() const {return H;}
    inline const int& getBoardW() const {return W;}
    //落子接口
    bool place_piece(int x,int y,Color color){
        if(board[x][y]!=Color::None||!isready()) return false;
        lastPlaceAction={.x=x,.y=y,.color=color};
        board[y][x]=color;
        step++;
        history.push_back({step,lastPlaceAction});
        return true;
    }
    bool place_piece(placeAction action){
        if(board[action.x][action.y]!=Color::None||!isready()) return false;
        board[action.y][action.x]=action.color;
        lastPlaceAction=action;
        step++;
        history.push_back({step,action});
        return true;
    }
    bool registerPlayer(Color color){
        if(color==Color::None) return false;
        if(color==Color::Black&&!black_ready)
        {
            black_ready=true;
            return true;
        }  
        if(color==Color::Black&&black_ready)
            return false;
        if(color==Color::White&&!white_ready)
        {
            white_ready=true;
            return true;
        }  
        if(color==Color::White&&white_ready)
            return false;
    }
    //检查胜利
    Color checkWin(){
        if(lastPlaceAction.color==Color::None)
            return Color::None;          
        int x=lastPlaceAction.x;
        int y=lastPlaceAction.y;
        Color color=lastPlaceAction.color;
        //横检查
        uint8_t count=1;
        for(int i=x-1;i>=x-4&&i>=0;i--){
            if(color==board[y][i])
                count++;
            else if(board[y][i]!=Color::None)
                break;
        }
        for(int i=x+1;i<=x+4&&i<W;i++){
            if(color==board[y][i])
                count++;
            else if(board[y][i]!=Color::None)
                break;
        }
        if(count==5)
            return color;
        //束检查
        count =1;
        for(int i=y-1;i>=y-4&&i>=0;i--){
            if(color==board[i][x])
                count++;
            else if(board[i][x]!=Color::None)
                break;
        }
        for(int i=y+1;i<=y+4&&i<H;i++){
            if(color==board[i][x])
                count++;
            else if(board[i][x]!=Color::None)
                break;
        }
        if(count==5)
            return color;
        //右斜检查
        count =1;
        for(int i=1;i<=4&&x+i<W&&y+i<H;i++){
            if(color==board[y+i][x+i])
                count++;
            else if(board[y+i][x+i]!=Color::None)
                break;
        }
        for(int i=1;i<4&&x-i>=0&&y-i>=0;i++){
            if(color==board[y-i][x-i])
                count++;
            else if(board[y-i][x-i]!=Color::None)
                break;
        }
        if(count==5)
            return color;
        //左斜检查
        count =1;
        for(int i=1;i<=4&&x-i>=0&&y+i<H;i++){
            if(color==board[y+i][x-i])
                count++;
            else if(board[y+i][x-i]!=Color::None)
                break;
        }
        for(int i=1;i<4&&x+i<W&&y-i>=0;i++){
            if(color==board[y-i][x+i])
                count++;
            else if(board[y-i][x+i]!=Color::None)
                break;
        }
        if(count==5)
            return color;
        return Color::None;
    }
};