// #include <iostream>
// #include "Player.h"
// #include "Chessboard.h"
// #include <cstdlib>
// auto testboard=Chessboard();
// auto blackPlayer=Player(&testboard,Color::Black);
// auto whitePlayer=Player(&testboard,Color::White);
// void clearScreen() {
// #ifdef _WIN32
//     system("cls");
// #else
//     system("clear");
// #endif
// }
// void show_board(Chessboard& board){
//     int w=board.getBoardW();
//     int h=board.getBoardH();
//     const Color* boardBegin=board.getBoardData();
//     for(int i=0;i<w;i++)
//         std::cout<<' '<<i;
//     std::cout<<"\n";
//     for(int i=0;i<h;i++){
//         for(int j=0;j<w;j++){
//             if(j==0)
//                 std::cout<<i;
//             else
//                 std::cout<<' ';
//             if(boardBegin[i*w+j]!=Color::None){
//                 std::cout<<(int)(boardBegin[i*w+j]);
//             }
//             else std::cout<<(int)(boardBegin[i*w+j]);
//         }
//         std::cout<<std::endl;
//     }
//     printf("Black:1,White:2\n");
// }
// bool checkwin(Chessboard& board){
//     if(board.checkWin()!=Color::None){
//         std::cout<<"Winner:"<<(int)board.checkWin()<<std::endl;
//         return true;
//     }
//     return false;
// }
// int main()
// {
//     // testboard.reset();
//     int x,y;
//     Color color;
//     placeAction action;
//     while (1) {
//         clearScreen();
//         show_board(testboard);
//         printf("Input White:\n");
//         scanf("%d%d",&x,&y);
//         action={.x=x,.y=y,.color=Color::White};
//         whitePlayer.commitMove(x,y);
//         whitePlayer.comfirmSelection(true);
//         if(checkwin(testboard))
//             break;
//         clearScreen();
//         show_board(testboard);
//         printf("Input Black:\n");
//         scanf("%d%d",&x,&y);
//         blackPlayer.commitMove(x,y);
//         blackPlayer.comfirmSelection(true);
//         if(checkwin(testboard))
//             break;
//         clearScreen();
//     }
//     system("pause");
// }
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "Chessboard.h"
#include "Player.h"

int main() {
    GOMOKU::Chessboard board;
    GOMOKU::Player blackP(&board, GOMOKU::Color::Black);
    GOMOKU::Player whiteP(&board, GOMOKU::Color::White);
    GOMOKU::Color currentTurn = GOMOKU::Color::Black;

    int preX = -1, preY = -1;
    bool hasPreSelected = false;

    InitWindow(900, 650, "Gomoku Game");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // 1. 交互：只在点击时锁定预选位置
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 m = GetMousePosition();
            int tx = (m.x - 40 + 20) / 40;
            int ty = (m.y - 40 + 20) / 40;

            if (tx >= 0 && tx < 15 && ty >= 0 && ty < 15) {
                if (board.get_piece_at(tx, ty) == GOMOKU::Color::None) {
                    preX = tx; preY = ty;
                    hasPreSelected = true; // 锁定坐标
                }
            }
        }

        BeginDrawing();
        ClearBackground(BEIGE);

        // 绘制棋盘和已有的棋子 (省略重复代码...)
        for (int i = 0; i < 15; i++) {
                    DrawLine(40, 40 + i * 40, 40 + 14 * 40, 40 + i * 40, DARKGRAY);
                    DrawLine(40 + i * 40, 40, 40 + i * 40, 40 + 14 * 40, DARKGRAY);
                }

        // 绘制已落子
        for (int y = 0; y < 15; y++) {
            for (int x = 0; x < 15; x++) {
                GOMOKU::Color p = board.get_piece_at(x, y);
                if (p == GOMOKU::Color::Black) DrawCircle(40 + x*40, 40 + y*40, 16, BLACK);
                if (p == GOMOKU::Color::White) {
                    DrawCircle(40 + x*40, 40 + y*40, 16, RAYWHITE);
                    DrawCircleLines(40 + x*40, 40 + y*40, 16, BLACK);
                }
            }
        }
        // 2. 绘制固定的预选标记
        if (hasPreSelected) {
            DrawCircleLines(40 + preX * 40, 40 + preY * 40, 20, RED); // 红圈锁定感
        }
        // 3. UI 侧边栏（使用英文避开乱码）
        DrawRectangle(620, 0, 280, 650, Fade(BLACK, 0.1f));
        DrawText(TextFormat("Step: %d", board.getStep()), 650, 50, 20, DARKGRAY);
        DrawText(TextFormat("Turn: %s", (currentTurn == GOMOKU::Color::Black ? "BLACK" : "WHITE")), 650, 80, 20, BLACK);

        if (GuiButton({ 650, 130, 200, 50 }, "Confirm Move") && hasPreSelected) {
            if (currentTurn == GOMOKU::Color::Black) {
                if (blackP.commitMove(preX, preY)) {
                    blackP.comfirmSelection(true);
                    currentTurn = GOMOKU::Color::White;
                    hasPreSelected = false;
                }
            } else {
                if (whiteP.commitMove(preX, preY)) {
                    whiteP.comfirmSelection(true);
                    currentTurn = GOMOKU::Color::Black;
                    hasPreSelected = false;
                }
            }
        }

        // 4. 历史记录 (修正拼写为 placeaction)
        auto hist = board.gethistory();
        int displayCount = 0;
        for (int i = (int)hist.size() - 1; i >= 0 && displayCount < 5; i--, displayCount++) {
            auto act = hist[i].placeaction; // 修正后的拼写
            DrawText(TextFormat("#%d: (%d, %d)", hist[i].id, act.x, act.y), 650, 250 + displayCount * 25, 16, DARKGRAY);
        }
        if (board.checkWin() != GOMOKU::Color::None) {
            DrawRectangle(0, 0, 900, 650, Fade(RAYWHITE, 0.8f));
            DrawText("CHECKMATE!", 300, 300, 40, RED);
            if (GuiButton({ 350, 380, 200, 40 }, "Restart Game")) {
                board.reset();
                hasPreSelected = false;
                currentTurn = GOMOKU::Color::Black;
            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}