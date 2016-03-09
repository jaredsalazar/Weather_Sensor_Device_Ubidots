String jsonContruct() {
  //Device ID
  String devID = "LinkitOne-0001";

  //Battery Level and Charging
  String BatteryLv = String(LBattery.level()); //get battery charge level
  String charging = String(LBattery.isCharging());

  //Temperature and Humidity
  String* TH = HSM20GRead();

  //Rain Gauge Tips value
  String rainTips = String(tipsStat());

  //Time and GPS
  String currentTimestamp = String(rtcNow());
  String latitude = getLatitude();
  String longitude = getLongitude();

  Serial.println("got all variables. Generating payload..");

  //Contruct JSON
  String val = "{\"value\":"
               + rainTips
               + ",\"context\":{"
               + "\"Lat\":" + latitude + "," + "\"Lng\":" + longitude
               + ","
               + "\"Hum\":" + TH[1] + "," + "\"Tem\":" + TH[0]
               + ","
               + "\"BatLv\":" + BatteryLv + "," + "\"Charging\":" + charging
               + ","
               + "\"ID\":" + "\"" + devID + "\""
               + "}"
               + ",\"timestamp\":" + currentTimestamp + "000"
               + "}";

  return val;
}
