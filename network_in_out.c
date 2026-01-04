#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRSIZE 64

int main() {
/* Change enp5s0 to what your network interface is called */
  FILE *in_current = fopen("/sys/class/net/enp5s0/statistics/rx_bytes", "r");

  /* current = total bytes in since boot, diff = difference in total since boot and last time saved to tmp, last = bytes in tmp before overwriting */
  /* i is for checking if the if statements have triggered */
  char bytes_in_current[STRSIZE];
  char bytes_in_diff[STRSIZE];
  char bytes_in_last[STRSIZE];
  int i = 0;

  /* Get total bytes since boot, return NA if file wont open */
  if(in_current == NULL) {
    strncpy(bytes_in_diff, "NA\n", 3);
    i++;
  }
  else {
    fgets(bytes_in_current, STRSIZE, in_current);
    fclose(in_current);

    /* Get last checked bytes from tmp file, return NA and create file if it does not exist */
    FILE *in_last_r = fopen("./tmp/net_in_tmp", "r");

    if(in_last_r == NULL) {
      strncpy(bytes_in_diff, "NA\n", 3);
      FILE *in_last_w = fopen("./tmp/net_in_tmp", "w");
      fclose(in_last_w);
      i++;
    }
    else {
    fgets(bytes_in_last, STRSIZE, in_last_r);
    fclose(in_last_r);

    /* Write new last value to tmp file */
    FILE *in_last_w = fopen("./tmp/net_in_tmp", "w");
    fputs(bytes_in_current, in_last_w);
    fclose(in_last_w);

    }
  }

  /* Check if bytes_in_diff is still empty, and calculate diff */

  /*TODO! Check if value of i is 0, and if so convert last and current to long long, calculate diff and convert to K, otherwise print diff as is */
 
  /*DEBUG: */
//  printf("%d\n", i);
//  printf("%s/%s\n", bytes_in_current, bytes_in_last);

  return 0;
}
