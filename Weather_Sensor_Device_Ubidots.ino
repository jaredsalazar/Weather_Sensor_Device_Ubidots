#include <LGPS.h>
#include <LDateTime.h>
#include <LBattery.h>
#include <GPRSL.h>
#include <easyLStorage.h>

GPRSL global;
int LastSendTime = 0;

void setup() {
  Serial.begin(9600);
  blinkSetup();
  
  Serial.println("RTC Sync...");
  rtcSync();

  global.START();
  
  StorageStart();
  
  rainSensorAttach();
  
  blinker(3000);

  delay(15000);
}

void loop() {
  if ((millis()) - LastSendTime > 15000) {
    LastSendTime = millis();
    String var = jsonContruct();
    String response = gprsSend(var);
    Serial.println(response);
    checkResponse(var, response);
    blinker(100);
  }
  if(!checkSD()){
    int delayTime = 15000 - millis() + LastSendTime;
    if(delayTime < 0){
      delayTime = 0;
    }
    Serial.print("delay Time: ");
    Serial.println(delayTime);
    delay(delayTime);
  }else{
    delay(200);
  }
}

