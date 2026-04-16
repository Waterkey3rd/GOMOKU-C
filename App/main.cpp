#include <iostream>
#include "Player.h"
#include "Chessboard.h"
#include <cstdlib>
auto testboard=Chessboard();
auto blackPlayer=Player(&testboard,Color::Black);
auto whitePlayer=Player(&testboard,Color::White);
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void show_board(Chessboard& board){
    int w=board.getBoardW();
    int h=board.getBoardH();
    const Color* boardBegin=board.getBoardData();
    for(int i=0;i<w;i++)
        std::cout<<' '<<i;
    std::cout<<"\n";
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            if(j==0)
                std::cout<<i;
            else
                std::cout<<' ';
            if(boardBegin[i*w+j]!=Color::None){
                std::cout<<(int)(boardBegin[i*w+j]);
            }
            else std::cout<<(int)(boardBegin[i*w+j]);
        }
        std::cout<<std::endl;
    }
    printf("Black:1,White:2\n");
}
bool checkwin(Chessboard& board){
    if(board.checkWin()!=Color::None){
        std::cout<<"Winner:"<<(int)board.checkWin()<<std::endl;
        return true;
    }
    return false;
}
int main()
{
    // testboard.reset();
    int x,y;
    Color color;
    placeAction action;
    while (1) {
        clearScreen();
        show_board(testboard);
        printf("Input White:\n");
        scanf("%d%d",&x,&y);
        action={.x=x,.y=y,.color=Color::White};
        whitePlayer.commitMove(x,y);
        whitePlayer.comfirmSelection(true);
        if(checkwin(testboard))
            break;
        clearScreen();
        show_board(testboard);
        printf("Input Black:\n");
        scanf("%d%d",&x,&y);
        blackPlayer.commitMove(x,y);
        blackPlayer.comfirmSelection(true);
        if(checkwin(testboard))
            break;
        clearScreen();
    }
    system("pause");
}