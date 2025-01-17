#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "leaderboard.h"
#include "logger.h"
#include "third-party/cJSON/cJSON.h"

void loadScores(Score scores[])
{
  FILE *file = fopen(FILENAME, "r");
  if (!file)
  {
    // Initialize empty scores if file doesn't exist
    for (int i = 0; i < MAX_SCORES; i++)
    {
      strcpy(scores[i].name, "EMPTY");
      scores[i].score = 0;
      strcpy(scores[i].timestamp, "N/A");
    }
    return;
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *data = (char *)malloc(fileSize + 1);
  fread(data, 1, fileSize, file);
  fclose(file);
  data[fileSize] = '\0';

  cJSON *json = cJSON_Parse(data);
  if (!json)
  {
    free(data);
    return;
  }

  cJSON *scoreArray = cJSON_GetObjectItem(json, "scores");
  int arraySize = cJSON_GetArraySize(scoreArray);

  for (int i = 0; i < MAX_SCORES; i++)
  {
    if (i < arraySize)
    {
      cJSON *item = cJSON_GetArrayItem(scoreArray, i);
      strcpy(scores[i].name, cJSON_GetObjectItem(item, "name")->valuestring);
      scores[i].score = cJSON_GetObjectItem(item, "score")->valueint;
      strcpy(scores[i].timestamp, cJSON_GetObjectItem(item, "timestamp")->valuestring);
    }
    else
    {
      strcpy(scores[i].name, "EMPTY");
      scores[i].score = 0;
      strcpy(scores[i].timestamp, "N/A");
    }
  }

  cJSON_Delete(json);
  free(data);
}

void saveScores(Score scores[])
{
  cJSON *json = cJSON_CreateObject();
  cJSON *scoreArray = cJSON_CreateArray();

  for (int i = 0; i < MAX_SCORES; i++)
  {
    cJSON *item = cJSON_CreateObject();
    cJSON_AddStringToObject(item, "name", scores[i].name);
    cJSON_AddNumberToObject(item, "score", scores[i].score);
    cJSON_AddStringToObject(item, "timestamp", scores[i].timestamp);
    cJSON_AddItemToArray(scoreArray, item);
  }

  cJSON_AddItemToObject(json, "scores", scoreArray);

  char *data = cJSON_Print(json);
  FILE *file = fopen(FILENAME, "w");
  if (file)
  {
    fputs(data, file);
    fclose(file);
  }

  cJSON_Delete(json);
  free(data);
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
