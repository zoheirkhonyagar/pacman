#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "menu1.h"
#include "AddObject.h"
#include "leaderboard.h"
#include "audio.h"
#include "raylib.h"
#include "logger.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
Score scores[MAX_SCORES] = {0}; // Global scores array
int main()
{
    // Initialization
    initAudio();
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "pacman_menu");
    SetTargetFPS(60);

    // Load scores at the start of the program
    loadScores(scores);

    while (!WindowShouldClose())
    {
        select_option(scores);
        BeginDrawing();
        show_menu();
        EndDrawing();
    }

    saveScores(scores); // Save scores before exiting
    // Cleanup
    cleanupAudio();
    CloseWindow();

    return 0;
}
