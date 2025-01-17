#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "leaderboard.h"
#include "logger.h"
#include "third-party/cJSON/cJSON.h"

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
    logToFile("No scores file found. Initializing empty scores.");
    saveScores(scores, MAX_SCORES); // Save the initialized empty scores to a file
    return;
  }

  // Load scores from the JSON file
  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *fileContent = malloc(fileSize + 1);
  if (!fileContent)
  {
    logToFile("Error: Memory allocation failed for file content.");
    fclose(file);
    return;
  }

  fread(fileContent, 1, fileSize, file);
  fileContent[fileSize] = '\0';
  fclose(file);

  cJSON *jsonArray = cJSON_Parse(fileContent);
  free(fileContent);

  if (!jsonArray)
  {
    logToFile("Error: Failed to parse JSON content.");
    return;
  }

  int i = 0;
  cJSON *jsonScore;
  cJSON_ArrayForEach(jsonScore, jsonArray)
  {
    cJSON *name = cJSON_GetObjectItemCaseSensitive(jsonScore, "name");
    cJSON *score = cJSON_GetObjectItemCaseSensitive(jsonScore, "score");
    cJSON *timestamp = cJSON_GetObjectItemCaseSensitive(jsonScore, "timestamp");

    if (cJSON_IsString(name) && cJSON_IsNumber(score) && cJSON_IsString(timestamp))
    {
      strncpy(scores[i].name, name->valuestring, sizeof(scores[i].name) - 1);
      scores[i].score = score->valueint;
      strncpy(scores[i].timestamp, timestamp->valuestring, sizeof(scores[i].timestamp) - 1);
      i++;
    }
    if (i >= MAX_SCORES)
      break;
  }

  cJSON_Delete(jsonArray);
}

void saveScores(Score scores[], int count)
{
  cJSON *jsonArray = cJSON_CreateArray();

  for (int i = 0; i < count; i++)
  {
    cJSON *jsonScore = cJSON_CreateObject();
    cJSON_AddStringToObject(jsonScore, "name", scores[i].name);
    cJSON_AddNumberToObject(jsonScore, "score", scores[i].score);
    cJSON_AddStringToObject(jsonScore, "timestamp", scores[i].timestamp);
    cJSON_AddItemToArray(jsonArray, jsonScore);
  }

  char *jsonString = cJSON_Print(jsonArray);
  FILE *file = fopen("scores.json", "w");
  if (file)
  {
    fputs(jsonString, file);
    fclose(file);
  }
  else
  {
    logToFile("Error: Unable to open scores.json for writing.");
  }

  cJSON_Delete(jsonArray);
  free(jsonString);
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
