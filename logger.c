#include "logger.h"

void logToFile(const char *format, ...)
{
  FILE *logFile = fopen("log.txt", "a"); // Open log file in append mode
  if (!logFile)
  {
    printf("Error: Unable to open log file\n");
    return;
  }

  va_list args;
  va_start(args, format);
  vfprintf(logFile, format, args); // Write formatted data to the log file
  va_end(args);

  fprintf(logFile, "\n"); // Add a newline at the end
  fclose(logFile);
}
