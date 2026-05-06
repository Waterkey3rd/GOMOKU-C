#define RAYGUI_IMPLEMENTATION
#include "ui.h"
int main(){
    UI::init();
    printf("GAMESTART\n");
    while (UI::update()) {
    
    }
    printf("GAMEOVER\n");
    return 0;
}