#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>

char *get_time() { // Get time in YYYY-MM-DD hh:mm:ss format
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
  FILE *out_file = fopen("counter_data.txt", "w");
  if (out_file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  size_t bytes = 1073741824; // one GB
  unsigned int tests = 0;
  unsigned char total = 0;

  fprintf(out_file, "=== Bitflip Counter ===\r");
  fprintf(out_file, "=======================\r");
  fprintf(out_file, "Allocating a gigabyte ...\r");
  unsigned char *buffer = (unsigned char *)calloc(bytes, 1);

  // Ensure the buffer is actually resident in RAM
  mlock(buffer, bytes);
  memset(buffer, 0, bytes);

  char *start_time = get_time();
  fprintf(out_file, "Run started: %s\r", start_time);
  free(start_time); // Free the allocated memory for the start time

  fflush(out_file);
  while (total < 100) {
    // delay bitflip count by 30 secs
    sleep(30);

    // Iterate through and tally all zero bytes
    for (size_t i = 0; i < bytes; ++i) {
      total += buffer[i];
    }

    // Update test #, bitflip count, and time
    char *curr_time = get_time();
    fprintf(out_file, "\rTest run #%d, ", tests);
    fprintf(out_file, "Bitflip count is %d, ", total);
    fprintf(out_file, "Test terminated at %s\r", curr_time);
    ++tests;
    fflush(out_file);
    free(curr_time);
  }
  fclose(out_file);
  free(buffer);
}