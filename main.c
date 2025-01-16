#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "menu1.h"
#include "AddObject.h"
#include "leaderboard.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
Score scores[MAX_SCORES] = {0}; // Global scores array
int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "pacman_menu");
    SetTargetFPS(60);

    // Load scores at the start of the program
    loadScores(scores);

    while (!WindowShouldClose())
    {
        select_option();
        BeginDrawing();
        show_menu();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
