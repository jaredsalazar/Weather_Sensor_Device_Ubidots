char* gpsStream;
long double GPSLat = 0, GPSLng = 0;
String sGPSLat, sGPSLng;
gpsSentenceInfoStruct info;
String data[13];

boolean GPSsync(boolean timesync) {

  int second, minute, hour, year, month, day;
  int SyncTimeStart = millis();

  while (timeSynced == false) {

    LGPS.getData(&info);
    gpsStream = (char*)info.GPRMC;
    Serial.println(gpsStream);
    dhtRead();                    //read temperature and humidity values while in-sync
    parser(gpsStream);            // parse gpsStream

    second = (int(data[1][4]) - 48) * 10 + (int(data[1][5]) - 48);
    minute = (int(data[1][2]) - 48) * 10 + (int(data[1][3]) - 48);
    hour   = (int(data[1][0]) - 48) * 10 + (int(data[1][1]) - 48);
    year   = (int(data[9][4]) - 48) * 10 + (int(data[9][5]) - 48);
    month  = (int(data[9][2]) - 48) * 10 + (int(data[9][3]) - 48);
    day    = (int(data[9][0]) - 48) * 10 + (int(data[9][1]) - 48);

    double d;
    d = strtod(data[3].c_str(), NULL);
    int deg = d / 100;
    double lat = deg + ((d / 100 - deg) * 100) / 60;

    d = strtod(data[5].c_str(), NULL);
    deg = d / 100;
    double lng = deg + ((d / 100 - deg) * 100) / 60;

    if (year < 70 && year >= 15 && lng > 0 && timesync == true) {
      setTime(hour, minute, second, day, month, 2000 + year);
      Log("Time Synced");
    }


    if (year < 70 && year >= 15 && lng > 0) {
      dhtRead();
      if (lat != 0.0 || lng != 0.0) {
        sprintf(cGPSLat, "%Lf", lat);
        sprintf(cGPSLng, "%Lf", lng);
        sGPSLat = cGPSLat;
        sGPSLng = cGPSLng;
        Serial.println("Lat: " + sGPSLat + " Lng: " + sGPSLng);
        //Serial.printf("Lat: %Lf  Lng: %Lf\n", lat, lng)
        Log("Resync Success");
        return true;
        break;
      }
    }

    int currentSyncTime = millis();
    if (currentSyncTime - SyncTimeStart >= 300000) {
      Log("Resync Failed...");
      return false;
      break;
    }

    delay(200);
  }
}

String* parser(char* gpsStream) {
  char *token;
  const char s[2] = ",";

  token = strtok(gpsStream, s);
  int k = 0;
  /* walk through other tokens */
  while ( token != NULL )
  {
    data[k] = token;
    token = strtok(NULL, s);
    k++;
  }

  return data;
}
