//STORAGE GLOBAL
easyLStorage sd;
String tempStorage = "temp.txt";
String Statlog = "DevStat.log";

void StorageStart() {
  delay(100);
  sd.start("temporary.log");
  delay(100);
  sd.start(Statlog);
  delay(100);
  sd.start("temporary.log");
}

void checkResponse(String val, String response) {
  if (response != "200" && response != "201" && response != "400") {
    sd.cacheData(val, tempStorage);
    Log(response + " : caching data");
  } else if (response == "200" || response == "201") {
    Log("Sent...");
  }
}

void Log(String _stat) {
  delay(10);
  String stat = String(timeNow()) + " : " + _stat;
  sd.cacheData(stat, Statlog);
}

boolean checkSD() {
  delay(10);
  String data = "";
  data = sd.getData(tempStorage);
  if (data == "") {
    return false;
  } else {

    String response = gprsSend(data);
    if (response == "200" || response == "201") {
      Log("Cached data Sent...");
      sd.deleteData(tempStorage);
      return true;
    } else if (response == "400") {
      Serial.println("Bad Request... Deleting Data");
      sd.deleteData(tempStorage);
      return true;
    } else {
      Log(response);
      return true;
    }
  }
}

