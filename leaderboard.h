#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#define MAX_SCORES 10
#define FILENAME "scores.json"

// Struct to represent a score entry
typedef struct
{
  char name[50];      // Player's name
  int score;          // Player's score
  char timestamp[20]; // Timestamp of the score
} Score;

// Functions for leaderboard management
void loadScores(Score scores[]);
void saveScores(Score scores[], int count); // Updated to include count
void addScore(Score scores[], const char *name, int score);
void displayScores(const Score scores[]);

#endif // LEADERBOARD_H
