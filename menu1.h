//
// Created by Bank on 1/11/2025.
//
#include "leaderboard.h" // Include this to define Score
#ifndef PACMANPRO_MENU1_H
#define PACMANPRO_MENU1_H
typedef enum
{
    Play,
    Records,
    Exit
} menu;
void loadTexture();
void Draw();
void show_menu();
void getPlayerName();

void handle_options(menu current_option);
void select_option(Score scores[]);
#endif // PACMANPRO_MENU1_H
