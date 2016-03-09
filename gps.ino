datetimeInfo t;
unsigned int rtc;

char* gpsStream;
gpsSentenceInfoStruct info;
String data[13];
char *_token;
const char s[2] = ",";

int gps_on = 0; //This is used to check if the GPS needs to be turned off

String timeNow() {
  LDateTime.getTime(&t);
  String timeNow = String(t.year)
                   + "/" + String(t.mon)
                   + "/" + String(t.day)
                   + " " + String(t.hour)
                   + ":" + String(t.min)
                   + ":" + String(t.sec);
  Serial.print("Time: ");
  Serial.println(timeNow);
  return timeNow;
}

int rtcNow() {
  LDateTime.getRtc(&rtc);
  Serial.print(" Seconds since 1/1/1970 GMT: ");
  Serial.println(rtc);
  return rtc;
}

void rtcSync() {
  while (!rtcSynced()) {
    Serial.println("Syncing....");
    //Wait one second before getting new time.
    delay(1000);
  }
}

boolean rtcSynced() {
  getGPSData();
  t.sec   = (int(data[1][4]) - 48) * 10 + (int(data[1][5]) - 48);
  t.min   = (int(data[1][2]) - 48) * 10 + (int(data[1][3]) - 48);
  t.hour  = (int(data[1][0]) - 48) * 10 + (int(data[1][1]) - 48);
  t.year  = (int(data[9][4]) - 48) * 10 + (int(data[9][5]) - 48);
  if (t.year < 70 && t.year >= 15) {
    t.year += 2000;
  }

  t.mon   = (int(data[9][2]) - 48) * 10 + (int(data[9][3]) - 48);
  t.day   = (int(data[9][0]) - 48) * 10 + (int(data[9][1]) - 48);
  LDateTime.setTime(&t);
  LDateTime.getTime(&t);
  LDateTime.getRtc(&rtc);
  Serial.print("Current GMT: ");
  Serial.print(t.mon);
  Serial.print("/");
  Serial.print(t.day);
  Serial.print("/");
  Serial.print(t.year);
  Serial.print(" ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(":");
  Serial.print(t.sec);
  Serial.print(" Seconds since 1/1/1970 GMT: ");
  Serial.println(rtc);
  //Turning on the GPS syncs up the RTC with the GPS time.
  //If the battery is pulled, the RTC goes back a couple years.
  //Turning on the GPS syncs up the RTC with the GPS time.
  //If the GPS is needed, t.year will be 2004. This will start this loop.
  if ((gps_on != 1) && (t.year < 2015))
  {
    Serial.println("Using GPS to sync GMT. Please wait...");
    LGPS.powerOn();
    gps_on = 1;
  }
  //If the GPS has synced the RTC, the year will be 2015 or greater.
  if (t.year >= 2015)
  {
    LGPS.powerOff();
    Log("GPS OFF....");
    gps_on = 0;
    Serial.println("Synced! Turning off GPS. Please wait...");

    return true;
  }
  return false;
}

void getGPSData() {
  if (gps_on == 0) {
    Log("GPS ON....");
    LGPS.powerOn();
    gps_on = 1;
  }
  LGPS.getData(&info);
  gpsStream = (char*)info.GPRMC;
  Serial.println(gpsStream);


  _token = strtok(gpsStream, s);
  int k = 0;
  /* walk through other tokens */
  while ( _token != NULL )
  {
    data[k] = _token;
    _token = strtok(NULL, s);
    k++;
  }
}

String getLatitude() {
  if (data[3] == 0) {
    getGPSData();
  } else if (gps_on == 1) {
    LGPS.powerOff();
    Log("GPS OFF....");
    gps_on = 0;
  }
  double d;
  char c_lat[10];
  d = strtod(data[3].c_str(), NULL);
  int deg = d / 100;
  double _lat = deg + ((d / 100 - deg) * 100) / 60;
  sprintf(c_lat, "%Lf", _lat);

  String lat = c_lat;
  return lat;
}


String getLongitude() {
  if (data[5] == 0) {
    getGPSData();
  } else if (gps_on == 1) {
    LGPS.powerOff();
    Log("GPS OFF....");
    gps_on = 0;
  }
  double d;
  char c_lng[10];

  d = strtod(data[5].c_str(), NULL);
  int deg = d / 100;
  double _lng = deg + ((d / 100 - deg) * 100) / 60;
  sprintf(c_lng, "%Lf", _lng);

  String lng = c_lng;
  return lng;
}

