#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_proc_stat() {
  FILE *file = fopen("/proc/stat", "r");
  if (!file) {
    perror("Failed to open /proc/stat");
    return;
  }

  char line[256];
  unsigned long prev_total = 0, prev_idle = 0;
  FILE *tmp_file = fopen("./tmp", "r");
  if (tmp_file) {
    if (fscanf(tmp_file, "%lu %lu", &prev_total, &prev_idle) != 2) {
      perror("Failed to read previous values from temporary file");
    }
    fclose(tmp_file);
  }

  if (fgets(line, sizeof(line), file)) {
    /* Variables to hold CPU usage data */
    unsigned long user, nice, system, idle, iowait, irq, softirq, steal, guest,
        guest_nice;

    /* Parse the first line of /proc/stat into variables */
    sscanf(line, "cpu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu", &user, &nice,
           &system, &idle, &iowait, &irq, &softirq, &steal, &guest,
           &guest_nice);

    /* Save the values to a temporary file */
    unsigned long current_total = user + nice + system + irq + softirq + steal +
                                  guest + guest_nice + idle + iowait;
    unsigned long current_idle = idle + iowait;

    /* Print calculated CPU usage */
    printf("Prev Total CPU: %lu\n", prev_total);
    printf("Prev Idle CPU: %lu\n", prev_idle);
    printf("Total CPU: %lu\n", current_total);
    printf("Idle CPU: %lu\n", current_idle);

    unsigned long total_final = current_total - prev_total;
    printf("%lu\n", total_final);
    unsigned long total_idle = current_idle - prev_idle;
    printf("%lu\n", total_idle);
    double usage =
        ((double)(total_final - total_idle) * 100) / (double)total_final;

    printf("%.2f\n", usage);
    FILE *tmp_file = fopen("./tmp", "w");
    if (tmp_file) {
      fprintf(tmp_file, "%lu %lu\n", current_total, current_idle);
      fclose(tmp_file);
    } else {
      perror("Failed to open temporary file");
    }
  }

  fclose(file);
}

int main() {
  read_proc_stat();
  return 0;
}
