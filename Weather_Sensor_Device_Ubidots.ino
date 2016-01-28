#include <LGPS.h>
#include <LDateTime.h>
#include <LBattery.h>
#include <GPRSL.h>
#include <easyLStorage.h>

GPRSL global;
int LastSendTime = 0,nowSending = 0, reboot_count = 0;
const int send_interval = 60000;

void setup() {
  Serial.begin(9600);
  blinkSetup();

  LTask.begin();

  Serial.println("RTC Sync...");
  rtcSync();

  global.START();

  StorageStart();
  Log("Device Starting....");

  rainSensorAttach();

  blinker(3000);

  LTask.remoteCall(createThread, NULL);
  delay(15000);
}

void loop() {
  if ((millis()) - LastSendTime > send_interval) {
    Log("Sending now...");
    nowSending = 1;
    LastSendTime = millis();
    String var = jsonContruct();
    String response = gprsSend(var);
    Serial.println(response);
    checkResponse(var, response);
    blinker(100);
    delay(100);
  }
  if (!checkSD()) {
    int delayTime = send_interval - millis() + LastSendTime;
    if (delayTime < 0) {
      delayTime = 0;
    }
    Serial.print("delay Time: ");
    Serial.println(delayTime);
    nowSending = 0;
    delay(delayTime);
  } else {
    delay(10);
  }
}

boolean createThread(void* userdata) {
  vm_thread_create(blinker_thread, NULL, 0);
  return true;
}

boolean call_reboot(void* userdata) {
  vm_reboot_normal_start();
  return true;
}

VMINT32 blinker_thread(VM_THREAD_HANDLE thread_handle, void* user_data) {
  for (;;)
  {
    if(nowSending == 0){
      Serial.printf("\nblinker thread... still running %d \n", reboot_count);
    if (reboot_count == 360) {
      Serial.printf("\nReboot...\n");
      Log("Device reboot...");
      LTask.remoteCall(call_reboot, NULL);
    }
    blinker(100);
    reboot_count++;
    delay(10000);
    }else{
      delay(100);
    }
  }
  return 0;
}

