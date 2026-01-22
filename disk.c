/*
  Usage: ./disk
  Example Output: DISK: 45%

  Not much to say here, statvfs is included in most distros by default.
  If you want to measure a different mount point, change the "/" in statvfs on
  line 15.
*/

#include <stdio.h>
#include <sys/statvfs.h>

int main() {
  struct statvfs stat;

  if (statvfs("/", &stat) != 0) {
    perror("statvfs failed");
    return 1;
  }

  unsigned long total_blocks = stat.f_blocks;
  unsigned long free_blocks = stat.f_bfree;
  double used_percentage = 100.0 * (1.0 - ((double)free_blocks / total_blocks));
  int rounded_used_percentage = (int)(used_percentage + 0.5);

  printf("DISK: %d%%\n", rounded_used_percentage);

  return 0;
}
