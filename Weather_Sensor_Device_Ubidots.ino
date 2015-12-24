#include <LBattery.h>
#include <GPRSL.h>
#include <easyLStorage.h>
#include <Time.h>
#include <LGPS.h>
#include <Average.h>

//Device Info Global
String devID = "LinkitOne-0001";

//Battery GLOBAL
int BatteryLv;
boolean ChargeStat = false;

//DHT GLOBAL
String Humidity, Temperature;

//STORAGE GLOBAL
easyLStorage sd;
String tempStorage = "temp.log";
String Statlog = "stat.log";

//GPRS Global
GPRSL global;
String response;

//GPS GLOBAL
char cGPSLat[10], cGPSLng[10];
boolean timeSynced = false;
long currentTimestamp;

//Rain Global
float rainMM;
int currentTips;
float TIPSIZE = 0.2794; //millimeter of rain per tip
volatile int tips;
volatile unsigned long raintime, rainlast, raininterval, lastminute, currentminute, lastDHTtime, lastSendMinute, lastGPSSynctime;

void setup() {
  pinMode(7, OUTPUT);
  blinker(2000);


  //Serial Start
  Serial.begin(9600);
  Serial.println("Serial Start...");

  //DHT Start
  HSM20GRead(); //get initial value
  Serial.println("Temp and Humid Start...");


  //SD Start
  sd.start(tempStorage);
  sd.start(Statlog);

  //GPS Start
  LGPS.powerOn(); //start GPS Module
  Serial.println("LGPS Power on, and waiting ...");
  GPSsync(true);
  delay(3000);

  //GPRS Start
  boolean attach = global.START();
  if (attach == false) {
    Log("attach failed");
  }
  blinker(300);


  //Start Rain Gauge Setup
  pinMode(2, INPUT_PULLUP); // input from wind meters rain gauge sensor
  tips = 0;
  currentTips = 0;
  lastminute = 0;
  lastDHTtime = 0;
  lastSendMinute = 0;
  rainMM = 0.0;
  // attach external interrupt pins to IRQ functions
  attachInterrupt(0, rainIRQ, FALLING);
  // turn on interrupts
  interrupts();


  delay(15000);
}

void loop() {
  currentminute = millis();
  currentTimestamp = now();

  if (currentminute - lastGPSSynctime >= 900000) { // sync gps and time every 15mins or 900000 milliseconds.
    Log("GPS Resync On...");
    timeSynced = false;
    LGPS.powerOn(); //Turn on GPS
    if (GPSsync(false)) {
      LGPS.powerOff(); //Turn off GPS to save power
      timeSynced = true;
      lastGPSSynctime = millis();
    }
    delay(200);
  }

  if (currentminute - lastDHTtime >= 2000) { // read temperature and sensor every 2000 milliseconds
    //read sensor values
    HSM20GRead();
    Serial.print("Temp: " + Temperature );
    Serial.println("  Humid: " + Humidity + "%");
    lastDHTtime = currentminute;
  }

  if (currentminute - lastSendMinute >= 15000) {
    deviceInfo();
    rainCalculate();

    //prepare json
    String val = "{\"value\":"
                 + String(rainMM)
                 + ",\"context\":{"
                 + "\"Lat\":" + cGPSLat + "," + "\"Lng\":" + cGPSLng
                 + ","
                 + "\"Hum\":" + Humidity + "," + "\"Tem\":" + Temperature
                 + ","
                 + "\"BatLv\":" + BatteryLv + "," + "\"Charging\":" + ChargeStat
                 + ","
                 + "\"ID\":" + "\"" + devID + "\""
                 + "}"
                 + ",\"timestamp\":" + currentTimestamp + "000"
                 + "}";
    if (val != "") {
      response = send(val);
      if (response != "200" && response != "201" && response != "400") {
        sd.cacheData(val, tempStorage);
        Log(response);
      }
    }
    blinker(500);
    lastSendMinute = currentminute;
  }
  checkSD();
  delay(200);
}

void deviceInfo() {
  BatteryLv = LBattery.level();
  ChargeStat = LBattery.isCharging();
}

void rainCalculate() {
  currentTips = tips;
  Serial.print("current tips: ");
  Serial.println(currentTips);
  if (currentminute - lastminute >= 60000)
  {
    tips = 0; //reset tips every minute
    rainMM = currentTips * TIPSIZE;
    lastminute = currentminute;
  }
}

void rainIRQ()
// Count rain gauge bucket tips as they occur
// Activated by the magnet and reed switch in the rain gauge, attached to input D2
{
  raintime = millis(); // grab current time
  raininterval = raintime - rainlast; // calculate interval between this and last event

  if (raininterval > 10) // ignore switch-bounce glitches less than 10mS after initial edge
  {
    tips += 1; //Increase this minute's amount of rain

    rainlast = raintime; // set up for next event
  }
}

boolean checkSD() {
  String data = sd.getData(tempStorage);
  if (data == "") {
    return false;
  } else {
    String response = send(data);
    if (response == "200" || response == "201") {
      sd.deleteData(tempStorage);
      return true;
    } else if (response == "400") {
      Serial.println("Bad Request... Deleting Data");
      sd.deleteData(tempStorage);
      return true;
    } else {
      Log(response);
    }
  }
}

void Log(String _stat) {
  String stat = String(now()) + " : " + _stat;
  sd.cacheData(stat, Statlog);
}

