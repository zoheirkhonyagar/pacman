#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "menu1.h"
#include "AddObject.h"

#define screenwidth  1200
#define screenheight  900

 int main (){
    InitWindow(screenwidth , screenheight ,"pacman_menu");
    SetTargetFPS(60);
     while (!WindowShouldClose()) {
         select_option();
         BeginDrawing();
         show_menu();
         EndDrawing();
     }


    CloseWindow();
}

