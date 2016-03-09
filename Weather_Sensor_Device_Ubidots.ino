#include <LGPS.h>
#include <LDateTime.h>
#include <LBattery.h>
#include <easyLStorage.h>
#include <LGPRS.h>      //include the base GPRS library
#include <LGPRSClient.h>
#include <LTask.h>
#include "vmthread.h"

int LastSendTime = 0, nowSending = 0, reboot_count = 0;
const int send_interval = 60000;

void setup() {
  delay(30000);
  Serial.begin(9600);

  blinkSetup();

  StorageStart();
  Log("Device Starting...");

  rtcSync();
  Log("RTC Synced");

  gprsStart();
  Log("GPRS Attached");

  Serial.println("Attaching rain sensor");
  rainSensorAttach();
  Log("Rain Sensor Interupt Attached..");

  blinker(3000);

  Serial.println("creating Thread...");
  delay(100);
  LTask.remoteCall(createThread, NULL);
  Serial.println("Thread created");
  delay(15000);
}

void loop() {
  delay(100);
}
boolean createThread(void* userdata) {
  // The priority can be 1 - 255 and default priority are 0
  // the arduino priority are 245
  vm_thread_create(send_tread, NULL, 0);
  return true;
}

VMINT32 send_tread(VM_THREAD_HANDLE thread_handle, void* user_data)
{
  for (;;)
  {
    if ((millis()) >= LastSendTime + send_interval) {
      LastSendTime = millis();
      Log("Sending now...");
      delay(100);
      String var = jsonContruct();
      Serial.println("sending now...");
      String response = gprsSend(var);
      Serial.println(response);
      checkResponse(var, response);
      blinker(100);
    } else if (!checkSD()) {
      Serial.println("sending from storage... done");
      int delay_time = send_interval - (millis() - LastSendTime);
      if(delay_time < 0){
        delay_time = 10;
      }
      Serial.printf("\ndelay: %d",delay_time);
      delay(delay_time);
    } else {
      Serial.println("delaying....");
      delay(10);
    }
  }
  return 0;
}

