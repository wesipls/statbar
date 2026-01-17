#include <inttypes.h>
#include <rocm_smi/rocm_smi.h>
#include <stdint.h>
#include <stdio.h>

int main() {
  rsmi_status_t status;
  uint32_t gpu_busy_percent, gpu_id = 0;

  if ((status = rsmi_init(RSMI_INIT_FLAG_ALL_GPUS)) == RSMI_STATUS_SUCCESS) {
    if ((status = rsmi_dev_busy_percent_get(gpu_id, &gpu_busy_percent)) ==
        RSMI_STATUS_SUCCESS) {
      printf("GPU %u Usage: %u%%\n", gpu_id, gpu_busy_percent);
    } else {
      fprintf(stderr, "Failed to retrieve GPU usage for device %u\n", gpu_id);
    }

    uint64_t temp;
    if ((status = rsmi_dev_temp_metric_get(gpu_id, 0, RSMI_TEMP_CURRENT,
                                           (int64_t *)&temp)) ==
        RSMI_STATUS_SUCCESS) {
      printf("GPU %u Temperature: %lu°C\n", gpu_id,
             temp / 1000); // Temperature is usually reported in millidegrees C
    } else {
      fprintf(stderr, "Failed to retrieve GPU temperature for device %u\n",
              gpu_id);
    }
    rsmi_shut_down();
  } else {
    fprintf(stderr, "Failed to initialize ROCm SMI (Error Code: %d)\n", status);
    return -1;
  }

  return 0;
}
