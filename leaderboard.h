#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#define MAX_SCORES 10
#define FILENAME "scores.json"

typedef struct
{
  char name[50];
  int score;
  char timestamp[20];
} Score;

void loadScores(Score scores[]);
void saveScores(Score scores[]);
void addScore(Score scores[], const char *name, int score);
void displayScores(const Score scores[]);

#endif // LEADERBOARD_H
