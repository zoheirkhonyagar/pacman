//
// Created by Bank on 1/12/2025.
//
#include "leaderboard.h" // Include this to define Score

#ifndef PACMANPRO_ADDOBJECT_H
#define PACMANPRO_ADDOBJECT_H

void showmovement(Score scores[], const char *playerName);
extern int score;
void checkStarCollision();
void replaceStars();
void initializeStars();
#endif // PACMANPRO_ADDOBJECT_H
