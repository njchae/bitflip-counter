#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>

// https://stackoverflow.com/questions/9596945/how-to-get-appropriate-timestamp-in-c-for-logs

char *get_time() 
{
  time_t current;
  struct tm *time_info;
  char *time_string = malloc(100);

  if (time_string == NULL) {
    perror("Failed to allocate memory for time_string");
    exit(EXIT_FAILURE);
  }

  time(&current);
  time_info = localtime(&current);
  strftime(time_string, 100, "%Y-%m-%d %H:%M:%S", time_info);
  return time_string;
}

int main() {
  FILE *out_file = fopen("/mnt/c/Users/nicoc/bitflip-counter/counter/counter_data.txt", "w");
  if (out_file == NULL)
  {
    printf("Error! Could not open file\n");
    exit(-1);
  }

  size_t bytes = 1073741824; // one gigabyte
  unsigned int tests = 0;
  unsigned char total = 0;

  fprintf(out_file, "=== Bitflipped ===\r");
  fprintf(out_file, "==================\r");
  fprintf(out_file, "Allocating a gigabyte ...\r");
  unsigned char *buffer = (unsigned char *)calloc(bytes, 1);

  // Ensure the buffer is actually resident in RAM
  mlock(buffer, bytes);
  memset(buffer, 0, bytes);

  char *start_time = get_time();
  fprintf(out_file, "Run started: %s\r", start_time);
  free(start_time); // Free the allocated memory for the start time

  fflush(out_file);
  while (total == 0) {
    // We aren't going to miss a bitflip by being slow
    sleep(30);

    // Naively walk through and tally all zero bytes
    for (size_t i = 0; i < bytes; ++i) {
      total += buffer[i];
    }

    // Keep the user sane that it isn't frozen :)
    fprintf(out_file, "\rTest run #%d", tests);
    ++tests;
  }

  char *error_time = get_time();
  fprintf(out_file, "--- !!! ---\r");
  fprintf(out_file, "Error detected: %s\r", error_time);
  fprintf(out_file, "Result should be 0 but is %d\r", total);
  fprintf(out_file, "Total tests run: %d\r", tests);
  free(error_time); // Free the allocated memory for the error time

  fflush(out_file);
  fclose(out_file);
  free(buffer);

//char *timestamp()
//{
//  time_t ltime; /* calendar time */
//  ltime = time(NULL); /* get current cal time */
//  return asctime(localtime(&ltime));
//}
}