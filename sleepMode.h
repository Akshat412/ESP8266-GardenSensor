#define RTCMEM_FLAG 64 // Position of toggleFlag, needed for checking memory state
#define RTCMEM_START 65 // Start position as one bucket taken up by toggleFlag
#define RTCMEM_LEN 75 // Last bucket used in program

extern "C" {
#include "user_interface.h" // this is for the RTC memory read/write functions
}

// Data structure for storage in buckets
typedef struct {
  int lightData; // Variable for saving 
  int sleepTime;
} rtcStore;

rtcStore rtcMem; // Instance of rtcStore for usage in code, used as temp variable
rtcStore sleepMemory[5]; // Array of rtcStore for rewriting data, first element to be sent over MQTT

int buckets; // Number of buckets occupied by every structure element
int timeMultiplier; // Time multiplier for sleep time
bool toggleFlag; // toggleFlag to check if data already exists

int mapSleepTime(int lightData) {
  return 30000000; // Default value of 30 seconds
}

void RTC_init() {
  
}

void writeData() {
  // First time setup function for program
  for (int i = 0; i < 5; i++) {
    toggleFlag = false;
    system_rtc_mem_write(RTCMEM_FLAG, &toggleFlag, 4); 
    
    rtcMem.lightData = random(700, 1000); // Typical LDR values for middle of the day
    rtcMem.sleepTime = mapSleepTime(rtcMem.lightData);
    int pos = RTCMEM_START + i*buckets; 
    system_rtc_mem_write(pos, &rtcMem, buckets * 4); 
  }
}

void readData() {
  // Reading data for computation
  for (int i = 0; i < 5; i++) {
    toggleFlag = false;
    system_rtc_mem_write(RTCMEM_FLAG, &toggleFlag, 4); 
    
    int pos = RTCMEM_START + i*buckets; 
    system_rtc_mem_read(pos, &rtcMem, buckets * 4); 

    sleepMemory[i] = rtcMem;

    Serial.print("i: ");
    Serial.print(i);
    Serial.print(" Position: ");
    Serial.print(pos);
    Serial.print(", lightData: ");
    Serial.print(sleepMemory[i].lightData); 
    Serial.print(", sleepTime: ");
    Serial.println(sleepMemory[i].sleepTime);
    yield();
  }
}

void rewriteData() {
  // Rewriting data by replacing first variable
  for(int i = 4; i > 0; i--) {
    sleepMemory[i] = sleepMemory[i-1];
  }

  rtcMem.lightData = analogRead(A0); 
  rtcMem.sleepTime = mapSleepTime(rtcMem.lightData); 
  sleepMemory[0] = rtcMem;

  for (int i = 0; i < 5; i++) {
    int pos = RTCMEM_START + i*buckets; 
    rtcMem = sleepMemory[i];
    system_rtc_mem_write(pos, &rtcMem, buckets * 4); 
    
    Serial.print("i: ");
    Serial.print(i);
    Serial.print(" Position: ");
    Serial.print(pos);
    Serial.print(", lightData: ");
    Serial.print(sleepMemory[i].lightData); 
    Serial.print(", sleepTime: ");
    Serial.println(sleepMemory[i].sleepTime);
    yield();
  }
}
