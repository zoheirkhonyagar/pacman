#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "leaderboard.h"
#include <stdarg.h>
#include "logger.h"

void loadScores(Score scores[])
{
  // Initialize scores array to default values
  for (int i = 0; i < MAX_SCORES; i++)
  {
    scores[i].score = 0;
    snprintf(scores[i].name, sizeof(scores[i].name), "EMPTY");
    snprintf(scores[i].timestamp, sizeof(scores[i].timestamp), "N/A");
  }

  FILE *file = fopen("scores.json", "r");

  if (!file)
  {
    // File doesn't exist, create an empty one
    logToFile("No scores file found. Initializing empty scores.\n");
    saveScores(scores); // Save the initialized empty scores to a file
    return;
  }

  // Load scores from the file
  for (int i = 0; i < MAX_SCORES && !feof(file); i++)
  {
    fscanf(file, "%49[^,],%d,%19[^\n]\n", scores[i].name, &scores[i].score, scores[i].timestamp);
    printf("Loaded score: %s - %d (%s)\n", scores[i].name, scores[i].score, scores[i].timestamp);
  }

  fclose(file);
}

void saveScores(Score scores[])
{
  FILE *file = fopen("scores.json", "w");
  if (!file)
  {
    printf("Error: Unable to save scores to file.\n");
    return;
  }

  for (int i = 0; i < MAX_SCORES; i++)
  {
    if (scores[i].score > 0)
    { // Only save scores greater than 0
      fprintf(file, "%s,%d,%s\n", scores[i].name, scores[i].score, scores[i].timestamp);
    }
  }

  fclose(file);
  printf("Scores saved successfully.\n");
}

void addScore(Score scores[], const char *name, int score)
{
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

  for (int i = 0; i < MAX_SCORES; i++)
  {
    if (score > scores[i].score)
    {
      for (int j = MAX_SCORES - 1; j > i; j--)
      {
        scores[j] = scores[j - 1];
      }
      strncpy(scores[i].name, name, sizeof(scores[i].name) - 1);
      scores[i].name[sizeof(scores[i].name) - 1] = '\0';
      scores[i].score = score;
      strncpy(scores[i].timestamp, timestamp, sizeof(scores[i].timestamp) - 1);
      scores[i].timestamp[sizeof(scores[i].timestamp) - 1] = '\0';
      break;
    }
  }
}

void displayScores(const Score scores[])
{
  printf("\nLeaderboard:\n");
  printf("%-20s %-10s %-20s\n", "Name", "Score", "Timestamp");
  printf("---------------------------------------------\n");
  for (int i = 0; i < MAX_SCORES && scores[i].score > 0; i++)
  {
    printf("%-20s %-10d %-20s\n", scores[i].name, scores[i].score, scores[i].timestamp);
  }
}
