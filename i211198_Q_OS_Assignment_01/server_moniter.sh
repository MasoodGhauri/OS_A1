#!/bin/bash

# Define thresholds
DISK_THRESHOLD=30
CPU_THRESHOLD=30
MEMORY_THRESHOLD=10
LOG_FILE_SIZE_THRESHOLD=10M

# Define log file path
LOG_FILE="./server_monitor.log"

# Function to log messages with timestamps
log() {
  echo "[$(date '+%Y-%m-%d %H:%M:%S')] $1" >> "$LOG_FILE"
}

# Function to check and alert on disk usage
check_disk_usage() {
  local disk_usage=$(df -h / | awk 'NR==2 {print $5}' | sed 's/%//')
  if [ "$disk_usage" -gt "$DISK_THRESHOLD" ]; then
    log "Disk usage is $disk_usage%, exceeding threshold of $DISK_THRESHOLD%."
    notify-send "Disk usage is $disk_usage%, exceeding threshold of $DISK_THRESHOLD%."
  fi
}

# Function to check and alert on CPU usage
check_cpu_usage() {
  local cpu_usage=$(top -b -n 1 | grep "%Cpu(s)" | awk '{print $2}' | cut -d. -f1)
  if [ "$cpu_usage" -gt "$CPU_THRESHOLD" ]; then
    log "CPU usage is $cpu_usage%, exceeding threshold of $CPU_THRESHOLD%."
    notify-send "CPU usage is $cpu_usage%, exceeding threshold of $CPU_THRESHOLD%."
  fi
}

# Function to check and alert on memory usage
check_memory_usage() {
  local free_mem=$(free | awk '/Mem/ {printf("%.2f", ($3/$2)*100)}')
  local free_memory=$(echo "$free_mem / 1" | bc)
  if [ "$free_memory" -lt "$MEMORY_THRESHOLD" ]; then
    log "Available memory is ${free_memory}MB, falling below threshold of ${MEMORY_THRESHOLD}MB free."
    notify-send "Available memory is ${free_memory}MB, falling below threshold of ${MEMORY_THRESHOLD}MB free."
  fi
}

# Function to rotate a log file
rotate_log_file() {
  local log_file_path="./server_monitor.log"
  if [ -f "$log_file_path" ] && [ "$(du -m "$log_file_path" | cut -f1)" -gt "$(echo "$LOG_FILE_SIZE_THRESHOLD" | sed 's/M//')" ]; then
    log "Rotating log file $log_file_path"
    mv "$log_file_path" "$log_file_path.$(date '+%Y%m%d%H%M%S').bak"
    touch "$log_file_path"
  fi
}

# Main script execution
{
  check_disk_usage
  check_cpu_usage
  check_memory_usage
  rotate_log_file
} >> "$LOG_FILE" 2>&1


exit 0
