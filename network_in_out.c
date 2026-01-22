/*
  Usage: ./network_in_out
  Example Output: NET: ↓ 12.3K / ↑ 8.7K

  Fetches network statistics from /sys/class/net/enp5s0/statistics/rx_bytes and
  /sys/class/net/enp5s0/statistics/tx_bytes, and saves values to /tmp/net_in_tmp
  and /tmp/net_out_tmp respectively.

  Assumes this program is run every 5 seconds, change the time_diff variable to
  match run interval. Assumes your network interface is enp5s0, change the paths
  if necessary.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRSIZE 32

int read_bytes(const char *file_path, char *buffer, size_t buffer_size) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  if (fgets(buffer, buffer_size, file) == NULL) {
    perror("Error reading file");
    fclose(file);
    return 2;
  }

  fclose(file);
  return 0;
}

int write_bytes(const char *file_path, const char *data) {
  FILE *file = fopen(file_path, "w");
  if (file == NULL) {
    perror("Error opening file for writing");
    return 1;
  }

  if (fputs(data, file) == EOF) {
    perror("Error writing to file");
    fclose(file);
    return 2;
  }

  fclose(file);
  return 0;
}

void compute_bytes_diff(const char *bytes_current, const char *bytes_last,
                        int time_diff, char *result) {
  unsigned long long current = strtoull(bytes_current, NULL, 10);
  unsigned long long last = strtoull(bytes_last, NULL, 10);
  double diff = (current > last ? current - last : 0);

  diff = diff / 1024 / time_diff;
  if (diff > 1024) {
    sprintf(result, "%.1fM", diff / 1024.0);
  } else {
    sprintf(result, "%.1fK", diff);
  }
}

int main() {
  const char *rx_current_path = "/sys/class/net/enp5s0/statistics/rx_bytes";
  const char *rx_last_path = "/tmp/net_in_tmp";
  const char *tx_current_path = "/sys/class/net/enp5s0/statistics/tx_bytes";
  const char *tx_last_path = "/tmp/net_out_tmp";

  char rx_current[STRSIZE], rx_last[STRSIZE], rx_diff[STRSIZE];
  char tx_current[STRSIZE], tx_last[STRSIZE], tx_diff[STRSIZE];
  int time_diff = 5;

  if (read_bytes(rx_current_path, rx_current, STRSIZE) != 0 ||
      read_bytes(rx_last_path, rx_last, STRSIZE) != 0) {
    fprintf(stderr, "Failed to read RX bytes\n");
    strcpy(rx_diff, "NA");
    write_bytes(rx_last_path, rx_current);
  } else {
    write_bytes(rx_last_path, rx_current);
    compute_bytes_diff(rx_current, rx_last, time_diff, rx_diff);
  }

  if (read_bytes(tx_current_path, tx_current, STRSIZE) != 0 ||
      read_bytes(tx_last_path, tx_last, STRSIZE) != 0) {
    fprintf(stderr, "Failed to read TX bytes\n");
    strcpy(tx_diff, "NA");
    write_bytes(tx_last_path, tx_current);
  } else {
    write_bytes(tx_last_path, tx_current);
    compute_bytes_diff(tx_current, tx_last, time_diff, tx_diff);
  }

  printf("NET: ↓ %s / ↑ %s\n", rx_diff, tx_diff);

  return 0;
}
